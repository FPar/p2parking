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

#ifndef MODEL_AGGREGATEINFORMATION_H_
#define MODEL_AGGREGATEINFORMATION_H_

#include <ctime>
#include "veins/base/utils/Coord.h"
#include "ResourceInformation.h"

#define AGGREGATE_EDGE_LENGTH 75

class AggregateInformation: public ResourceInformation {
public:
    /**
     * Horizontal grid position.
     */
    unsigned short i;

    /**
     * Vertical grid position.
     */
    unsigned short j;

    unsigned short level;

    unsigned short n;

    AggregateInformation() {
    }

    double relevance(Coord& position);

    void add(ResourceInformation& info);

private:
    bool isWithin(Coord& position);
};

#endif /* MODEL_AGGREGATEINFORMATION_H_ */
