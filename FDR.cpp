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
#include "utitilies.h"

FDR::FDR() {
    running = false;
}

FDR::~FDR() {
    
}

bool FDR::simulatorIsPaused() {
    return (bool) readDataI("sim/time/paused");
}

void FDR::update(float elapsedMe, float elapsedSim, int counter) {
    if (simulatorIsPaused()) {
        return;
    }

    if (!aircraftNumberOfEngines) {
        aircraftNumberOfEngines = readDataI("sim/aircraft/engine/acf_num_engines");
    }
    
    running = getRunningStatus();
    if (!running) return;

    int flight_time = round(readDataF("sim/time/total_flight_time_sec"));

    char buffer[1024];
    sprintf(buffer, "openFDR: recording data point at %d sec (%d points)\n", flight_time, dataPoints.size());
    XPLMDebugString(buffer);
    dataPoints.push_back(DataPoint(flight_time));
}

bool FDR::AircraftOnGround() {
    int h = round(readDataF("sim/flightmodel/position/y_agl"));
    return h <= 0;
}

bool FDR::OneEngineRunning() {
    int engine_running[8];
    readDataVI("sim/flightmodel/engine/ENGN_running", engine_running, 8);
    for (int i = 0; i < aircraftNumberOfEngines; i++) {
        if (engine_running[i] == 1) return true;
    }
    return false;
}

bool FDR::AircraftStopped() {
    int gs = round(readDataF("sim/flightmodel/position/groundspeed"));
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
