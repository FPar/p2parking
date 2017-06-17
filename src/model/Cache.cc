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

#define ENTRY_TTL 500
#define MAXIMUM_REPORT_COUNT 139

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

ResourceReport* Cache::getReport(Coord& position) {
    cleanup();

    vector<AtomicInformation*> at;
    vector<AggregateInformation*> agg;

    for (auto it = _atomics.begin(); it != _atomics.end(); ++it) {
        at.push_back(&it->second);
        it->second.saveRelevance(position);
    }

    for (auto it = _levels.begin(); it != _levels.end(); ++it) {
        for (auto it2 = it->_aggregates.begin(); it2 != it->_aggregates.end();
                ++it2) {
            agg.push_back(&it2->second);
            it2->second.saveRelevance(position);
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

void Cache::cleanup() {
    time_t now = time(NULL);
    for (auto it = _atomics.begin(); it != _atomics.end(); ++it) {
        if (difftime(now, it->second.too) > ENTRY_TTL) {
            _atomics.erase(it);
        }
    }

    for (auto it = _levels.begin(); it != _levels.end(); ++it) {
        it->cleanup();
    }
}
