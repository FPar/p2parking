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

#ifndef MODEL_RESOURCEINFORMATION_H_
#define MODEL_RESOURCEINFORMATION_H_

#include <ctime>
#include "Coord.h"

class ResourceInformation {
public:
    /**
     * Time of origin.
     */
    std::time_t too;

    /**
     * Point of origin.
     */
    Coord poo;

    unsigned short capacity;

    unsigned short occupancy;

    double _relevance;

    ResourceInformation() {
    }

    ResourceInformation(std::time_t too, Coord& poo, unsigned short capacity,
            unsigned short occupancy) :
            too(too), poo(poo), capacity(capacity), occupancy(occupancy) {
    }

    void saveRelevance(Coord& position);

    virtual double relevance(Coord& position) = 0;
};

#endif /* MODEL_RESOURCEINFORMATION_H_ */
