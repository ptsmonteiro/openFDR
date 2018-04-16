//
//  FDR.hpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <stdio.h>
#include <list>
#include "DataPoint.h"

class FDR
{
    
private:
    std::list<DataPoint> dataPoints;
    bool running;
    
    const char  *aircraftType;
    bool        aircraftHasRetrLandingGear;
    int         aircraftNumberOfEngines;

    
public:
    FDR();
    ~FDR();
    
    bool simulatorIsPaused();
    void update(float, float, int);
    bool getRunningStatus();
    bool AircraftStopped();
    bool AircraftOnGround();
    bool OneEngineRunning();
};
