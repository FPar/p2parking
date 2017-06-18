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

#ifndef MODEL_CACHEHIT_H_
#define MODEL_CACHEHIT_H_

struct CacheHit {
    int occupancy;
    int level;
    bool miss;

    CacheHit() : miss(true){}
    CacheHit(int occupancy, int level) : occupancy(occupancy), level(level), miss(false){}
};

#endif /* MODEL_CACHEHIT_H_ */
