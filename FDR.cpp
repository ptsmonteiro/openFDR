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
    flight = new Flight();
    config.Load();
}

FDR::~FDR() {
}

void FDR::planeLoaded() {
    aircraftNumberOfEngines = readDataI("sim/aircraft/engine/acf_num_engines");
    flight->planeLoaded();
}

bool FDR::simulatorIsPaused() {
    return (bool) readDataI("sim/time/paused");
}

void FDR::xacarsReport(int oldPhase, int newPhase) {
    if (flight->phase == Flight::PHASE_CRUISE) {
        xacars->acarsReportPosition(flight, getLastDatapoint());
        return;
    }
    if (flight->phase == Flight::PHASE_CLIMB || flight->phase == Flight::PHASE_DESCENT) {
        xacars->acarsReportAltitude(flight, getLastDatapoint());
        return;
    }

    if (oldPhase == Flight::PHASE_RAMP && newPhase == Flight::PHASE_TAXI) {
        xacars->acarsReportOut(flight, getLastDatapoint());
    }
    else if (oldPhase == Flight::PHASE_TAXI && newPhase == Flight::PHASE_CLIMB) {
        xacars->acarsReportOff(flight, getLastDatapoint());
    }
    else if (oldPhase == Flight::PHASE_DESCENT && newPhase == Flight::PHASE_LANDED) {
        xacars->acarsReportOn(flight, getLastDatapoint());
    }
    else if (oldPhase == Flight::PHASE_LANDED && newPhase == Flight::PHASE_RAMP) {
        xacars->acarsReportIn(flight, getLastDatapoint());
    }
}

void FDR::update(float elapsedMe, float elapsedSim, int counter) {
    if (simulatorIsPaused() || elapsedSim < 1) {
        return;
    }
    
    if (readDataI("sim/operation/prefs/replay_mode")) {
        return;
    }

    int flight_time = round(readDataF("sim/time/total_flight_time_sec"));
    runStatus(flight_time);
    if (!running) return;
    
    xacarsReport(flight->phase, updatePhase());
    
    char buffer[1024];
    sprintf(buffer, "openFDR: recording data point at %ds\n", flight_time);
    XPLMDebugString(buffer);
    dataPoints.push_back(DataPoint(flight_time));
}

DataPoint FDR::getLastDatapoint() {
    return dataPoints.empty() ? DataPoint(0) : dataPoints.back();
}

bool FDR::AircraftOnGround() {
    float gearForceY = readDataI("sim/flightmodel/forces/fnrml_gear");
    return gearForceY > 0;
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
    flight->reset();
    if (!xacars->beginFlight(flight, DataPoint(flightTime))) {
        // TODO pop plugin window to remind user of missing flight data
        devConsole("Unable to track flight. Please set flight data.");
    }
}

void FDR::endFlight() {
    XPLMDebugString("openFDR: Stopping recording.\n");
    running = false;
    toCSV();
}

int FDR::updatePhase() {
    int oldPhase = flight->phase;
    
    if (flight->phase == Flight::PHASE_RAMP) {
        if (OneEngineRunning() && !AircraftStopped()) {
            flight->phase = Flight::PHASE_TAXI;
        }
    }
    else if (flight->phase == Flight::PHASE_TAXI) {
        if (getLastDatapoint().verticalSpeedFPM > 100 && getLastDatapoint().heightFt >= 50) {
            flight->phase = Flight::PHASE_CLIMB;
        }
        else if (AircraftOnGround() && AircraftStopped() && !OneEngineRunning() ) {
            flight->phase = Flight::PHASE_RAMP;
        }
    }
    else if (flight->phase == Flight::PHASE_CLIMB) {
        if (abs(getLastDatapoint().verticalSpeedFPM) < 100) {
            flight->phase = Flight::PHASE_CRUISE;
        }
        else if(getLastDatapoint().verticalSpeedFPM < -100) {
            flight->phase = Flight::PHASE_DESCENT;
        }
    }
    else if (flight->phase == Flight::PHASE_CRUISE) {
        if (getLastDatapoint().verticalSpeedFPM > 100) {
            flight->phase = Flight::PHASE_CLIMB;
        }
        else if (getLastDatapoint().verticalSpeedFPM < -100) {
            flight->phase = Flight::PHASE_DESCENT;
        }
    }
    else if (flight->phase == Flight::PHASE_DESCENT) {
        if (AircraftOnGround()) {
            flight->phase = Flight::PHASE_LANDED;
        }
    }
    else if (flight->phase == Flight::PHASE_LANDED) {
        if (AircraftOnGround() && !OneEngineRunning() && AircraftStopped()) {
            flight->phase = Flight::PHASE_RAMP;
        }
    }
    if (oldPhase != flight->phase) {
        char buffer[1024] = "";
        sprintf(buffer, "Flight phase changed %d -> %d)", oldPhase, flight->phase);
        devConsole(string(buffer));
        xacarsReport(oldPhase, flight->phase);
    }
    return flight->phase;
}

void FDR::toCSV() {
    char timestamp[100];
    std::time_t t = std::time(nullptr);
    std:strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M", std::gmtime(&t));

    char flightname[1024];
    sprintf(flightname, "%s.%s", flight->aircraftType.c_str(), timestamp);
    
    char filename[1024];
    sprintf(filename, "%s.openfdr.data.csv", flightname);
    
    char message[1024];
    sprintf(message, "openFDR: Writing data for flight %s\n", flightname);
    XPLMDebugString(message);

    flight->toCSV(std::string(flightname));
    
    std::ofstream outfile;
    outfile.open(filename);
    outfile << dataPoints.front().toCSV(true);
    for (DataPoint dp : dataPoints) outfile << dp.toCSV(false);
    outfile.close();
}
