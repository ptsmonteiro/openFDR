//
//  FDR.cpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <cmath>
#include "FDR.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"

FDR::FDR() {
    running = false;
}

FDR::~FDR() {
    
}

bool FDR::simulatorIsPaused() {
    return (bool) XPLMGetDatai(XPLMFindDataRef("sim/time/paused"));
}

void FDR::update(float elapsedMe, float elapsedSim, int counter) {
    if (simulatorIsPaused()) {
        return;
    }
    
    running = getRunningStatus();
    if (!running) return;

    XPLMDebugString("openFDR: recording data point\n");
    dataPoints.push_back(DataPoint(elapsedSim));
}

bool FDR::AircraftOnGround() {
    int h = round(XPLMGetDataf(XPLMFindDataRef("sim/flightmodel/position/y_agl")));
    return h == 0;
}

bool FDR::OneEngineRunning() {
    int engines = XPLMGetDatai(XPLMFindDataRef("sim/aircraft/engine/acf_num_engines"));
    int engine_running[8];
    XPLMGetDatavi(XPLMFindDataRef("sim/flightmodel/engine/ENGN_running"), engine_running, 0, 7);
    for (int i = 0; i < engines; i++) {
        if (engine_running[i] == 1) return true;
    }
    return false;
}

bool FDR::AircraftStopped() {
    int gs = round(XPLMGetDataf(XPLMFindDataRef("sim/flightmodel/position/groundspeed")));
    return gs == 0;
}

bool FDR::getRunningStatus() {
    if (running) {
        if (AircraftOnGround() && !OneEngineRunning() && AircraftStopped()) {
            XPLMDebugString("openFDR: Stopping recording.\n");
            return false;
        }
    } else {
        if (AircraftOnGround() && OneEngineRunning() && AircraftStopped()) {
            XPLMDebugString("openFDR: Starting recording.\n");
            return true;
        }
    }
    return running;
}
