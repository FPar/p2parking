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

#ifndef APPLICATION_P2P_H_
#define APPLICATION_P2P_H_

#include <BaseWaveApplLayer.h>
#include "messages/BroadcastParkingPlaceInformationEvt_m.h"
#include "model/Cache.h"

class P2P: public BaseWaveApplLayer {
protected:
    void initialize(int stage);
    void finish();

    void onWSM(WaveShortMessage* wsm);
    void handleSelfMsg(cMessage* msg);

    BroadcastParkingPlaceInformationEvt* broadcastPPIEvt;

private:
    Cache _cache;

    int cacheHits = 0;
    int cacheMisses = 0;
    cDoubleHistogram accuracy;
    cLongHistogram hitlevel;
    cLongHistogram reportSize;

    void measureCorrectness();
};

#endif /* APPLICATION_P2P_H_ */
