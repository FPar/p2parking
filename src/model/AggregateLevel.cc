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

#include <AggregateLevel.h>

#define ENTRY_TTL 500

using namespace std;

void AggregateLevel::update(AggregateInformation& aggregate) {
    string key = aggregate.posStr();
    auto it = _aggregates.find(key);
    if (it != _aggregates.end()) {
        _aggregates.erase(key);
    }
    _aggregates.insert(pair<string, AggregateInformation>(key, aggregate));
}

void AggregateLevel::cleanup(simtime_t& time) {
    for (auto it = _aggregates.begin(); it != _aggregates.end(); ++it) {
        if ((time - it->second.too).dbl() > ENTRY_TTL) {
            _aggregates.erase(it);
        }
    }
}
