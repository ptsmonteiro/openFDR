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
#include "XACARS.h"

class FDR
{
private:
    Flight *flight;
    std::list<DataPoint> dataPoints;
    bool running;

    XACARS      *xacars;
    
    Config      config;
    
    bool        aircraftHasRetrLandingGear;
    int         aircraftNumberOfEngines = 0;
    
public:
    FDR();
    ~FDR();
    
    bool simulatorIsPaused();
    void update(float, float, int);
    void startFlight(int);
    void endFlight();
    bool runStatus(int);
    bool AircraftStopped();
    bool AircraftOnGround();
    bool OneEngineRunning();
    void toCSV();
};
