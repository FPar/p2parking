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

#include <cmath>
#include <AggregateInformation.h>

#define AVG_SPEED (40.0 / 3.6)

std::string AggregateInformation::posStr(AtomicInformation& info) {
    int i = info.poo.x / AGGREGATE_EDGE_LENGTH + 1;
    int j = info.poo.z / AGGREGATE_EDGE_LENGTH + 1;
    return std::to_string(i) + ";" + std::to_string(j);
}

AggregateInformation::AggregateInformation(Coord& coord, unsigned short level) :
        ResourceInformation(0, 0), level(level), n(0) {
    i = (coord.x / AGGREGATE_EDGE_LENGTH + 1);
    j = (coord.z / AGGREGATE_EDGE_LENGTH + 1);
    poo.x = (i - 1) * (coord.x / AGGREGATE_EDGE_LENGTH + 1);
    poo.z = (j - 1) * (coord.z / AGGREGATE_EDGE_LENGTH + 1);
}

double AggregateInformation::relevance(Coord& position) {
    double age_s = difftime(time(NULL), too);

    if (isWithin(position)) {
        return -age_s;
    } else {
        double distance = position.distance(poo);
        return 1.0 / double(n) * (-distance / AVG_SPEED - age_s);
    }
}

void AggregateInformation::add(ResourceInformation& info) {
    too = (n * info.too + info.too) / (n + 1);
    ++n;
    capacity += info.capacity;
    occupancy += occupancy;
}

bool AggregateInformation::isWithin(Coord& position) {
    double current_edge_length = pow(2, level) * AGGREGATE_EDGE_LENGTH;
    double horizontal_min = current_edge_length * (i - 1);
    double horizontal_max = horizontal_min + current_edge_length;
    double vertical_min = current_edge_length * (j - 1);
    double vertical_max = vertical_min + current_edge_length;
    return position.x >= horizontal_min && position.x < horizontal_max
            && position.z >= vertical_min && position.z < vertical_max;
}

std::string AggregateInformation::posStr() {
    return std::to_string(i) + ";" + std::to_string(j);
}
