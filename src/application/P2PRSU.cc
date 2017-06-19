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

#include <ctime>
#include "P2PRSU.h"

Define_Module(P2PRSU);

std::vector<P2PRSU*> P2PRSU::RSUs;

void P2PRSU::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        RSUs.push_back(this);

        capacity = rand() % 15 + 10;
        occupancy = rand() % (capacity + 1);

        recordScalar("#capacity", capacity);

        occupancyVector.setName("occupancy");
        occupancyVector.record(occupancy);

        broadcastPPIEvt = new BroadcastParkingPlaceInformationEvt();
        scheduleAt(simTime() + 10 + normal(0, 5.0), broadcastPPIEvt);
    }
}

void P2PRSU::handleSelfMsg(cMessage* msg) {
    if (dynamic_cast<BroadcastParkingPlaceInformationEvt*>(msg)) {
        occupancy += normal(0, capacity * 0.1);
        if (occupancy > capacity)
            occupancy = capacity;
        else if (occupancy < 0)
            occupancy = 0;
        occupancyVector.record(occupancy);

        ResourceReport* report = generateReport();
        sendDown(report);
        scheduleAt(simTime() + 10, broadcastPPIEvt);
    } else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}

ResourceReport* P2PRSU::generateReport() {
    ResourceReport* report = new ResourceReport();
    populateWSM(report);

    report->setAtomicsArraySize(1);

    AtomicInformation information(myId, simTime(), curPosition, capacity,
            occupancy);
    report->setAtomics(0, information);

    return report;
}
