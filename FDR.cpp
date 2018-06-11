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
    readDataB("sim/version/xplane_internal_version", simulatorVersion, sizeof(simulatorVersion));
    
    config.Load();
}

FDR::~FDR() {
}

bool FDR::simulatorIsPaused() {
    return (bool) readDataI("sim/time/paused");
}

void FDR::update(float elapsedMe, float elapsedSim, int counter) {
    if (simulatorIsPaused() || elapsedSim < 1) {
        return;
    }
    
    if (readDataI("sim/operation/prefs/replay_mode")) {
        return;
    }

    if (!aircraftNumberOfEngines) {
        aircraftNumberOfEngines = readDataI("sim/aircraft/engine/acf_num_engines");
    }
    
    int flight_time = round(readDataF("sim/time/total_flight_time_sec"));
    runStatus(flight_time);
    if (!running) return;

    char buffer[1024];
    sprintf(buffer, "openFDR: recording data point at %ds\n", flight_time);
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
            startFlight();
        }
    }

    return running;
}

void FDR::startFlight() {
    readDataB("sim/aircraft/view/acf_ICAO", aircraftType, sizeof(aircraftType));
    aircraftEmptyWeight = round(readDataF("/sim/aircraft/weight/acf_m_empty"));
    aircraftMaxWeight = round(readDataF("/sim/aircraft/weight/acf_m_max"));
    XPLMDebugString("openFDR: Starting recording.\n");
    running = true;
}

void FDR::endFlight() {
    XPLMDebugString("openFDR: Stopping recording.\n");
    running = false;
    toCSV();
}

void FDR::toCSV() {
    char timestamp[100];
    std::time_t t = std::time(nullptr);
    std:strftime(timestamp, sizeof(timestamp), "%F %T", std::gmtime(&t));
    
    char filename[1024];
    sprintf(filename, "openFDR %s %s.csv", aircraftType, timestamp);

    char message[1024];
    sprintf(message, "openFDR: Writing CSV file %s\n", filename);
    XPLMDebugString(message);

    std::ofstream outfile;
    outfile.open(filename);
    outfile << dataPoints.front().toCSV(true);
    for (DataPoint dp : dataPoints) outfile << dp.toCSV(false);
    outfile.close();
}
