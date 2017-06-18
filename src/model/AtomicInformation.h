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

#ifndef MODEL_ATOMICINFORMATION_H_
#define MODEL_ATOMICINFORMATION_H_

#include <ctime>
#include "veins/base/utils/Coord.h"
#include "ResourceInformation.h"

class AtomicInformation: public ResourceInformation {
public:
    unsigned int id;

    AtomicInformation() {
    }

    AtomicInformation(unsigned int id, simtime_t too, Coord& poo,
            unsigned short capacity, unsigned short occupancy) :
            ResourceInformation(too, poo, capacity, occupancy), id(id) {
    }

    double relevance(Coord& position, simtime_t& time);
};

#endif /* MODEL_ATOMICINFORMATION_H_ */
