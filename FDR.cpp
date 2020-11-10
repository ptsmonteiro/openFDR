//
//  FDR.cpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include "FDR.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "utitilies.h"

FDR::FDR() {
    running = false;
    config.Load();
}

FDR::~FDR() {
}

bool FDR::simulatorIsPaused() {
    return (bool) readDataI("sim/time/paused");
}

DataPoint* FDR::update(float elapsedMe, float elapsedSim, int counter) {
    if (simulatorIsPaused() || elapsedSim < 1) {
        return NULL;
    }
    
    if (readDataI("sim/operation/prefs/replay_mode")) {
        return NULL;
    }

    if (!aircraftNumberOfEngines) {
        aircraftNumberOfEngines = readDataI("sim/aircraft/engine/acf_num_engines");
    }
    
    int flight_time = round(readDataF("sim/time/total_flight_time_sec"));
    runStatus(flight_time);
    if (!running) return NULL;

    char buffer[1024];
    sprintf(buffer, "openFDR: recording data point at %ds\n", flight_time);
    XPLMDebugString(buffer);
    dataPoints.push_back(DataPoint(flight_time));

	return &dataPoints.back();
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

/* Start flight / end flight logic */
bool FDR::runStatus(int flightTime) {
    
    if (running && flightTime < dataPoints.back().elapsedFlightTime) {
        XPLMDebugString("openFDR: Flight was reset.\n");
        endFlight();
    }
    
    if (running) {
        if (AircraftOnGround() && !OneEngineRunning() && AircraftStopped()) {
            endFlight();
        }
    } else {
        if (AircraftOnGround() && OneEngineRunning() && AircraftStopped() && flightTime > 0) {
            startFlight(flightTime);
        }
    }

    return running;
}

void FDR::startFlight(int flightTime) {
    XPLMDebugString("openFDR: Starting recording.\n");
    running = true;
}

void FDR::endFlight() {
    XPLMDebugString("openFDR: Stopping recording.\n");
    running = false;
}

DataPoint* FDR::getLastDataPoint() {
	if (dataPoints.size() > 0) {
		return &dataPoints.back();
	}
	else {
		return NULL;
	}
}