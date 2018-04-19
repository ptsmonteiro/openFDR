//
//  DataPoint.hpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <stdio.h>
#include <string>
#include <sstream>

class DataPoint {

    int flightTimeStamp;
    
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
    char nav1Ident[4];
    int nav1CourseDeg;
    float nav1SlopeDeg;
    float nav1DME;
    float locDevDots;
    float glideDevDots;
    
    // VOR information TODO
    
public:
    int elapsedFlightTime;
    
    DataPoint(float);
    ~DataPoint();
    
    std::string toCSV(bool);
};
