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

#include "P2P.h"

Define_Module(P2P);

void P2P::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        broadcastPPIEvt = new BroadcastParkingPlaceInformationEvt();
        scheduleAt(simTime() + 10 + normal(1.0, 1.0), broadcastPPIEvt);
    }
}

void P2P::onWSM(WaveShortMessage* wsm) {
    if (ResourceReport* report = dynamic_cast<ResourceReport*>(wsm)) {
        _cache.update(*report);
    } else {
        BaseWaveApplLayer::onWSM(wsm);
    }
}

void P2P::handleSelfMsg(cMessage* msg) {
    if (dynamic_cast<BroadcastParkingPlaceInformationEvt*>(msg)) {
        ResourceReport* report = _cache.getReport();
        populateWSM(report);

        sendDown(report);

        scheduleAt(simTime() + 10, broadcastPPIEvt);
    } else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}
