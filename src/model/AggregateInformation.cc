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

#include <AggregateInformation.h>

#define AVG_SPEED (40.0 / 3.6)

double AggregateInformation::relevance(Coord& position) {
    double age_s = difftime(time(NULL), too);

    if (isWithin(position)) {
        return -age_s;
    } else {
        double distance = position.distance(poo);
        return 1.0 / double(n) * (-distance / AVG_SPEED - age_s);
    }
}

bool AggregateInformation::isWithin(Coord& position) {

}