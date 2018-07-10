//
//  Flight.h
//  openFDR
//
//  Created by Pedro on 28/05/2018.
//

#include <stdio.h>
#include <string>

using namespace std;

class Flight {
    
public:
    static const int PHASE_RAMP = 0;
    static const int PHASE_TAXI = 1;
    static const int PHASE_CLIMB = 2;
    static const int PHASE_CRUISE = 3;
    static const int PHASE_DESCENT = 4;
    static const int PHASE_LANDED = 5;

    int simulatorVersion = 0;

    string aircraftType = "";
    string aircraftRegistration = "";
    int  aircraftEmptyWeight = 0;
    int  aircraftMaxWeight = 0;

    string flightNumber = "";
    string originICAO = "";
    string destinationICAO = "";
    string alternateICAO = "";
    int  cruiseAltitude = 0;
    string route = "";
    
    string rules = "IFR"; // IFR/VFR
    string eobt = "";
    string eet = "";
    string comments = "";
    
    string online = "";
    string onlineID = "";
    int pax = 0;
    int cargoKg = 0;
    
    time_t timeOut;
    time_t timeOff;
    time_t timeOn;
    time_t timeIn;
    
    float outLat = 0.0;
    float outLong = 0.0;
    int outAlt = 0;

    int zfw = 0;
    int tow = 0;
    int lw = 0;
    int blockFuel = 0;
    int usedFuel = 0;

    float inLat = 0.0;
    float inLong = 0.0;
    int inAlt = 0;
    
    int maxClimb = 0;
    int maxDescent = 0;
    
    int maxIAS = 0;
    int maxGS = 0;
    
    int phase = PHASE_RAMP;
    
    Flight();
    ~Flight();
    
    void toCSV(std::string);
    void reset();
    void planeLoaded();
    bool flightPlanSet();
    
    string getFlightTime();
    string getBlockTime();
};
