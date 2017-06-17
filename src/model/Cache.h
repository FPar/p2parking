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

#ifndef MODEL_CACHE_H_
#define MODEL_CACHE_H_

#define CACHE_LEVELS 6

#include <array>
#include <map>
#include "messages/ResourceReport_m.h"
#include "AggregateLevel.h"

class Cache {
public:
    void update(ResourceReport& report);
    ResourceReport* getReport(Coord& position);

private:
    std::array<AggregateLevel, CACHE_LEVELS> _levels;
    std::map<int, AtomicInformation> _atomics;

    void cleanup();
};

#endif /* MODEL_CACHE_H_ */
