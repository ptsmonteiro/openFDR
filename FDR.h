//
//  FDR.hpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <sstream>
#include <string>
#include <stdio.h>
#include <list>
#include "Config.h"
#include "DataPoint.h"
#include "Flight.h"

class FDR
{
private:
    Flight *flight;
    std::list<DataPoint> dataPoints;
    bool running;

    Config      config;
    
    bool        aircraftHasRetrLandingGear;
    int         aircraftNumberOfEngines = 0;
    
public:
    FDR();
    ~FDR();
    
    bool simulatorIsPaused();
    void update(float, float, int);
    void startFlight();
    void endFlight();
    bool runStatus(int);
    bool AircraftStopped();
    bool AircraftOnGround();
    bool OneEngineRunning();
    void toCSV();
};
