//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <Cache.h>
#include <ctime>
#include <vector>
#include <algorithm>

#define ENTRY_TTL 50
#define MAXIMUM_REPORT_COUNT 5

using namespace std;

void Cache::update(ResourceReport& report) {
    for (int i = 0; i < report.getAtomicsArraySize(); ++i) {
        AtomicInformation& info = report.getAtomics(i);
        auto it = _atomics.find(info.id);
        if (it != _atomics.end()) {
            _atomics.erase(info.id);
        }
        _atomics.insert(pair<int, AtomicInformation>(info.id, info));
    }

    for (int i = 0; i < report.getAggregatesArraySize(); ++i) {
        AggregateInformation& info = report.getAggregates(i);
        AggregateLevel& aggregateLevel = _levels[info.level];
        aggregateLevel.update(info);
    }
}

bool compareReports(ResourceInformation* a, ResourceInformation* b) {
    return a->_relevance > b->_relevance;
}

ResourceReport* Cache::getReport(Coord& position, simtime_t& time) {
    cleanup(time);
    updateAggregates();

    vector<AtomicInformation*> at;
    vector<AggregateInformation*> agg;

    for (auto it = _atomics.begin(); it != _atomics.end(); ++it) {
        at.push_back(&it->second);
        it->second.saveRelevance(position, time);
    }

    for (auto it = _levels.begin(); it != _levels.end(); ++it) {
        for (auto it2 = it->_aggregates.begin(); it2 != it->_aggregates.end();
                ++it2) {
            agg.push_back(&it2->second);
            it2->second.saveRelevance(position, time);
        }
    }

    sort(at.begin(), at.end(), compareReports);
    sort(agg.begin(), agg.end(), compareReports);

    ResourceReport* r = new ResourceReport();

    int ati = 0, aggi = 0;

    r->setAtomicsArraySize(MAXIMUM_REPORT_COUNT);
    r->setAggregatesArraySize(MAXIMUM_REPORT_COUNT);

    for (int i = 0; i < MAXIMUM_REPORT_COUNT; ++i) {
        if (ati == at.size() && aggi == agg.size()) {
            break;
        }

        if (ati == at.size()) {
            r->setAggregates(aggi, *agg.at(aggi));
            ++aggi;
        } else if (aggi == agg.size()) {
            r->setAtomics(ati, *at.at(ati));
            ++ati;
        } else if (at.at(ati)->_relevance > agg.at(aggi)->_relevance) {
            r->setAtomics(ati, *at.at(ati));
            ++ati;
        } else {
            r->setAggregates(aggi, *agg.at(aggi));
            ++aggi;
        }
    }

    r->setAtomicsArraySize(ati);
    r->setAggregatesArraySize(aggi);

    return r;
}

void Cache::cleanup(simtime_t& time) {
    for (auto it = _atomics.begin(); it != _atomics.end();) {
        if ((time - it->second.too).dbl() > ENTRY_TTL) {
            _atomics.erase(it++);
        } else {
            ++it;
        }
    }

    for (auto it = _levels.begin(); it != _levels.end(); ++it) {
        it->cleanup(time);
    }
}

void Cache::updateAggregates() {
    AggregateLevel* curLevel = &_levels[0];

    for (auto it = _atomics.begin(); it != _atomics.end(); ++it) {
        string key = AggregateInformation::posStr(it->second.poo);

        auto aggit = curLevel->_aggregates.find(key);
        if (aggit != curLevel->_aggregates.end() && !aggit->second.isNew) {
            curLevel->_aggregates.erase(aggit);
            AggregateInformation a(it->second.poo, 1);
            curLevel->_aggregates[key] = a;
        } else if (aggit == curLevel->_aggregates.end()) {
            AggregateInformation a(it->second.poo, 1);
            curLevel->_aggregates[key] = a;
        }

        curLevel->_aggregates[key].add(it->second);
    }

    for (int i = 1; i < _levels.size(); ++i) {
        curLevel = &_levels[i];
        AggregateLevel* prevLevel = &_levels[i - 1];

        for (auto it = prevLevel->_aggregates.begin(); it != prevLevel->_aggregates.end(); ++it) {
            string key = it->second.posStr();

            auto curit = curLevel->_aggregates.find(key);
            if (curit != curLevel->_aggregates.end() && !curit->second.isNew) {
                curLevel->_aggregates.erase(curit);
                AggregateInformation a(it->second.poo, i);
                curLevel->_aggregates[key] = a;
            } else if (curit == curLevel->_aggregates.end()) {
                AggregateInformation a(it->second.poo, i);
                curLevel->_aggregates[key] = a;
            }

            curLevel->_aggregates[key].add(it->second);
            it->second.isNew = false;
        }
    }
}

CacheHit Cache::occupancy(P2PRSU* rsu) {
    auto atomic = _atomics.find(rsu->id());
    if(atomic != _atomics.end()) {
        return CacheHit(atomic->second.occupancy, 0);
    }

    for(int i = 0; i < _levels.size(); ++i) {
        string key = AggregateInformation::posStr(rsu->pos());
        auto agg = _levels[i]._aggregates.find(key);
        if(agg != _levels[i]._aggregates.end()) {
            int estimated = rsu->capacity * (double(agg->second.occupancy) / agg->second.capacity);
            return CacheHit(estimated, i);
        }
    }

    return CacheHit();
}

