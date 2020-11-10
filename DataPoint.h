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
	string aircraftRegistration;
	int aircraftEmptyWeightKg;
	int aircraftMaxWeightKg;

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
        
    // engine
    int engineLever1 = 0;
    int engineLever2 = 0;
    int engineLever3 = 0;
    int engineLever4 = 0;
    int engineLever5 = 0;
    int engineLever6 = 0;
    int engineLever7 = 0;
    int engineLever8 = 0;

    int enginePower1 = 0;
    int enginePower2 = 0;
    int enginePower3 = 0;
    int enginePower4 = 0;
    int enginePower5 = 0;
    int enginePower6 = 0;
    int enginePower7 = 0;
    int enginePower8 = 0;

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
    
    float elapsedFlightTime;
    
    DataPoint(float);
    ~DataPoint();
    
    string getXACARSFormattedLocation();
    
	string toJSON();
};
