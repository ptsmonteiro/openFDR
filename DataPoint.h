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

public:

    float flightTimeStamp = 0;
    
    // flight data
    int headingDeg = 0;
    
    int heightFt = 0;
    int altitudeFt = 0;
    int verticalSpeedFPM = 0;

    int speedIAS = 0;
    float speedMach = 0;

    float pitchDeg = 0;
    float bankDeg = 0;
    
    float alpha = 0;
    float loadFactorG = 0;
    
    // navigation data
    int trackDeg = 0;
    int speedGS = 0;
    
    int windDeg = 0;
    int windKt = 0;
    
    int oat = 0;
    
    float latitudeDeg = 0;
    float longitudeDeg = 0;
        
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
    int brakeLeft = 0;
    int brakeRight = 0;
    int parkingBrake = 0;
    
    bool gearLeverDown = 0;
    
    int pitchControl = 0;
    int rollControl = 0;
    int yawControl = 0;

    // control ratios
    int flapLever = 0;
    int speedBrakeLever = 0;
    
    // telemetry
    bool autopilotOn = 0;
    
    // weights
    int weightKg = 0;
    int fuelQuantityKg = 0;
    
    // cabin
    int cabinAltFt = 0;
    int cabinVsFPM = 0;
    
    // Radio nav information
    char nav1Ident[4];
    int nav1CourseDeg = 0;
    float nav1SlopeDeg = 0;
    float nav1DME = 0;
    float locDevDots = 0;
    float glideDevDots = 0;
    
    // VOR information TODO
    
    float elapsedFlightTime = 0;
    
    DataPoint(float);
    ~DataPoint();
    
    std::string toCSV(bool);
};
