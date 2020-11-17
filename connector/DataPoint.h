//
//  DataPoint.hpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

class DataPoint {

public:

	// properties
	int simulatorVersion;
	string aircraftType;
	int aircraftNumberOfEngines;
	string aircraftRegistration;
	int aircraftEmptyWeightKg;
	int aircraftMaxWeightKg;

	float elapsedFlightTime;
	float totalFlightTimeSec;
    float flightTimeStamp;
    
    // flight data
    int headingDeg;
    
    int heightFt;
    int altitudeFt;
    int verticalSpeedFPM;

    int speedIAS;
    float speedMach;

    float pitchDeg;
    float bankDeg;
    
    float alpha;
    float loadFactorG;
    
    // navigation data
    int trackDeg;
    int speedGS;
    
    int windDeg;
    int windKt;
    
    int oat;
    
    float latitudeDeg;
    float longitudeDeg;

	string nearestAirportId;
	string nearestAirportName;
        
    // engine
	int engineRunning[8];
	int engineLever[8];
	int enginePowerWatt[8];

	// brake
    int brakeLeft;
    int brakeRight;
    int parkingBrake;
    
    bool gearLeverDown;
    
    int pitchControl;
    int rollControl;
    int yawControl;

    // control ratios
    int flapLever;
    int speedBrakeLever;
    
    // telemetry
    bool autopilotOn;
    
    // weights
    int weightKg;
    int fuelQuantityKg;
    
    // cabin
    int cabinAltFt;
    int cabinVsFPM;
    
    // Radio nav information
    string nav1Id;
    int nav1CourseDeg;
    float nav1SlopeDeg;
    float nav1DME;
    float locDevDots;
    float glideDevDots;
    
    // VOR information TODO
        
    DataPoint(float);
    ~DataPoint();
    
    string getXACARSFormattedLocation();
    
	string toJSON();
};
