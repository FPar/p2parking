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
#include "P2PRSU.h"
#include <cmath>
#include <iostream>
#include <iomanip>

Define_Module(P2P);

void P2P::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        accuracy.setName("accuracy");
        accuracy.setRange(0, 1);
        hitlevel.setName("hitlevel");
        hitlevel.setRange(0, CACHE_LEVELS);
        reportSize.setName("reportSize");
        reportSize.setRange(0, 140);

        broadcastPPIEvt = new BroadcastParkingPlaceInformationEvt();
        scheduleAt(simTime() + 10 + normal(0, 1.0), broadcastPPIEvt);
    }
}

void P2P::finish() {
    BaseWaveApplLayer::finish();

    recordScalar("cacheHits", cacheHits);
    recordScalar("cacheMisses", cacheMisses);
    recordScalar("cacheHitRatio",
            double(cacheHits) / (cacheHits + cacheMisses));
    accuracy.recordAs("accuracy");
    hitlevel.recordAs("hitlevel");
    reportSize.recordAs("reportSize");
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
        simtime_t time = simTime();
        ResourceReport* report = _cache.getReport(curPosition, time);
        populateWSM(report);

        int rs = report->getAggregatesArraySize()
                + report->getAtomicsArraySize();
        reportSize.collect(rs);

        measureCorrectness();

        sendDown(report);

        scheduleAt(simTime() + 10, broadcastPPIEvt);
    } else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}

void P2P::measureCorrectness() {
    int foundRsus = 0;
    double caccuracy = 0;

    for (int i = 0; i < P2PRSU::RSUs.size(); ++i) {
        P2PRSU* rsu = P2PRSU::RSUs[i];
        CacheHit c = _cache.occupancy(rsu);

        if (c.miss) {
            ++cacheMisses;
            continue;
        }

        ++cacheHits;

        double miss = abs(rsu->occupancy - c.occupancy) / double(rsu->capacity);
        double acc = 1 - miss;

        caccuracy += acc;
        ++foundRsus;

        accuracy.collect(acc);
        hitlevel.collect(c.level);
    }

    if (foundRsus > 0) {
        caccuracy = caccuracy / foundRsus;

        int r = 255 * (1 - caccuracy);
        int g = 255 * caccuracy;
        r = r < 0 ? 0 : r > 255 ? 255 : r;
        g = g < 0 ? 0 : g > 255 ? 255 : g;
        std::stringstream color;
        color << "r=10,#" << std::hex << std::setw(2) << std::setfill('0') << r
                << std::hex << std::setw(2) << std::setfill('0') << g << "00";
        findHost()->getDisplayString().updateWith(color.str().data());
    }
}
