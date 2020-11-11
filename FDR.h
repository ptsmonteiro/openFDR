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
#include "Flight.h"

class FDR
{
private:

    std::list<DataPoint> dataPoints;
    bool running;
    
    XACARS      *xacars;
    Config      config;
    
    int         aircraftNumberOfEngines = 0;

    DataPoint getLastDatapoint();
    
public:
    Flight *flight;
    
    FDR();
    ~FDR();
    
    bool simulatorIsPaused();
    void update(float, float, int);
    int updatePhase();
    void xacarsReport(int, int);
    void startFlight(int);
    void endFlight();
    void planeLoaded();
    bool runStatus(int);
    bool AircraftStopped();
    bool AircraftOnGround();
    bool OneEngineRunning();
    void toCSV();
    
};
