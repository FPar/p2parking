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

ResourceReport* Cache::getReport() {

    return new ResourceReport();
}

void Cache::cleanup() {

}
