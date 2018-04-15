//
//  DataPoint.hpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <stdio.h>

class DataPoint {

    int simulatorElapsedTimeSec;
    
    // flight data
    int headingDeg;
    
    int heightFt;
    int altitudeFt;
    int verticalSpeedFPM;

    int speedIAS;
    float speedMach;

    float pitchDeg;
    float bankDeg;
    
    float loadFactorG;
    
    // navigation data
    int trackDeg;
    int speedGS;
    
    int windDeg;
    int windKt;
    
    float latitudeDeg;
    float longitudeDeg;
    
    // control positions between 0 and 100%
    int throttle1;
    int throttle2;
    int throttle3;
    int throttle4;

    int brakeLeft;
    int brakeRight;
    int parkingBrake;
    
    bool gearLeverDown;
    
    int pitchControl;
    int rollControl;
    int yawControl;

    // control discreet positions
    int flapLever;
    int speedBrakeLever;
    
    // telemetry
    bool autopilotOn;
    bool flightDirectorOn;
    int weightKg;
    int fuelQuantityKg;
    int cabinAltFt;
    
    bool oneEngineRunning;
    bool aircraftOnGround;
    
    // Radio nav information
    char nav1Ident[4];
    int nav1CourseDeg;
    float nav1SlopeDeg;
    float nav1DME;
    float locDevDots;
    float glideDevDots;
    
    // VOR information TODO
    
protected:
    int readDataI(const char *);
    float readDataF(const char *);
    int readDataVF(const char *, float *, int);
    int readDataB(const char *, void *, int);

public:
    DataPoint(float);
    ~DataPoint();
    
};
