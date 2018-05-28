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
#include "DataPoint.h"
#include "Config.h"

class FDR
{
private:
    std::list<DataPoint> dataPoints;
    bool running;

    Config      config;
    
    char        simulatorVersion[255];
    char        aircraftType[255];
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
