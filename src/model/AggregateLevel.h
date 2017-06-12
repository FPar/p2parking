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

#ifndef MODEL_AGGREGATELEVEL_H_
#define MODEL_AGGREGATELEVEL_H_

#include <map>
#include <string>
#include "AggregateInformation.h"

class AggregateLevel {
public:
    void update(AggregateInformation& aggregate);

private:
    std::map<std::string, AggregateInformation> _aggregates;
};

#endif /* MODEL_AGGREGATELEVEL_H_ */
