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
    
    bool        aircraftHasRetrLandingGear;
    int         aircraftNumberOfEngines = 0;
    
public:
    FDR();
    ~FDR();
    
    bool simulatorIsPaused();
    DataPoint* update(float, float, int);
    void startFlight(int);
    void endFlight();
    bool runStatus(int);
    bool AircraftStopped();
    bool AircraftOnGround();
    bool OneEngineRunning();

	DataPoint* getLastDataPoint();
};
