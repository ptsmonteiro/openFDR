//
//  DataPoint.cpp
//  openFDR
//
//  Created by Pedro on 15/04/2018.
//

#include <cmath>
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "DataPoint.h"

DataPoint::DataPoint(float ElapsedSim) {
    
    simulatorElapsedTimeSec = round(ElapsedSim);
    
    // flight data
    headingDeg = round(readDataF("sim/cockpit2/gauges/indicators/heading_vacuum_deg_mag_pilot"));
    heightFt = round(readDataF("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot"));
    altitudeFt = round(readDataF("sim/cockpit2/gauges/altitude_ft_pilot"));
    verticalSpeedFPM = round(readDataF("sim/cockpit2/gauges/vvi_fpm_pilot"));
    
    speedIAS = round(readDataF("sim/cockpit2/gauges/airspeed_kts_pilot"));
    speedMach = readDataF("sim/flightmodel/misc/machno");
    
    pitchDeg = readDataF("sim/flightmodel/position/true_theta");
    bankDeg = readDataF("sim/flightmodel/position/true_phi");
    
    loadFactorG = readDataF("sim/flightmodel/misc/g_total");
    
    // navigation data
    trackDeg = round(readDataF("sim/flightmodel/position/hpath"));
    speedGS = round(readDataF("sim/flightmodel/position/groundspeed"));
    
    latitudeDeg = readDataF("sim/flightmodel/position/latitude");
    longitudeDeg = readDataF("sim/flightmodel/position/longitude");

    // weather
    windDeg = round(readDataF("sim/weather/wind_direction_degt"));
    windKt = round(readDataF("sim/weather/wind_speed_kt"));
    
    // control positions between 0 and 100%
    float tr_engines[4];
    readDataVF("sim/weather/wind_speed_kt", tr_engines, 4);
    throttle1 = round(tr_engines[0] * 100);
    throttle2 = round(tr_engines[1] * 100);
    throttle3 = round(tr_engines[2] * 100);
    throttle4 = round(tr_engines[3] * 100);
    
    brakeLeft = round(readDataF("sim/cockpit2/controls/left_brake_ratio") * 100);
    brakeRight = round(readDataF("sim/cockpit2/controls/right_brake_ratio") * 100);
    parkingBrake = round(readDataF("sim/cockpit2/controls/parking_brake_ratio") * 100);

    gearLeverDown = (bool) readDataI("sim/cockpit2/controls/gear_handle_down");
    
    pitchControl = round(readDataF("sim/cockpit2/controls/yoke_pitch_ratio") * 100);
    rollControl = round(readDataF("sim/cockpit2/controls/yoke_roll_ratio") * 100);
    yawControl = round(readDataF("sim/cockpit2/controls/yoke_heading_ratio") * 100);
    
    // control ratios
    flapLever = round(readDataF("sim/cockpit2/controls/flap_ratio") * 100);
    speedBrakeLever = round(readDataF("sim/cockpit2/controls/speedbrake_ratio") * 100);
    
    // telemetry
    autopilotOn = readDataI("sim/cockpit/autopilot/autopilot_mode") == 2;
    flightDirectorOn = readDataI("sim/cockpit/autopilot/autopilot_mode") == 1;
    weightKg = round(readDataF("sim/flightmodel/weight/m_total"));
    fuelQuantityKg = round(readDataF("sim/flightmodel/weight/m_fuel_total"));
    cabinAltFt = round(readDataF("sim/cockpit2/pressurization/indicators/cabin_altitude_ft"));
    
    // ILS information
    readDataB("sim/cockpit2/radios/indicators/nav1_nav_id", nav1Ident, 4);
    nav1CourseDeg = round(readDataF("sim/cockpit/radios/nav1_course_degm"));
    nav1SlopeDeg = readDataF("sim/cockpit/radios/nav1_slope_degt");
    nav1DME = readDataF("sim/cockpit/radios/nav1_dme_dist_m");
    locDevDots = readDataF("sim/cockpit/radios/nav1_hdef_dot");
    glideDevDots = readDataF("sim/cockpit/radios/nav1_vdef_dot");

}

DataPoint::~DataPoint() {
    
}

int DataPoint::readDataI(const char *dataref){
    int value = 0;
    value = XPLMGetDatai(XPLMFindDataRef(dataref));
    char buffer[1024];
    sprintf(buffer, "Read %s = %d\n", dataref, value);
    XPLMDebugString(buffer);
    return value;
}

float DataPoint::readDataF(const char *dataref) {
    float value = 0;
    value = XPLMGetDataf(XPLMFindDataRef(dataref));
    char buffer[1024];
    sprintf(buffer, "Read %s = %f\n", dataref, value);
    XPLMDebugString(buffer);
    return value;
}

int DataPoint::readDataB(const char *dataref, void *buf, int size) {
   int read = 0;
   read = XPLMGetDatab(XPLMFindDataRef(dataref), buf, 0, size - 1);
   char buffer[1024];
   sprintf(buffer, "Read %s = %s\n", dataref, buf);
   XPLMDebugString(buffer);
   return read;
}

                       
int DataPoint::readDataVF(const char *dataref, float *vector, int size) {
    int i = 0;
    for (i=0; i<size; i++) {
        vector[i] = 0;
    }
    
    int readValues = 0;
    readValues = XPLMGetDatavf(XPLMFindDataRef(dataref), vector, 0, size-1);
    char buffer[1024];
    for (i=0; i<readValues; i++) {
        sprintf(buffer, "Read %s[%d] = %f\n", dataref, i, vector[i]);
        XPLMDebugString(buffer);
    }
    
    return readValues;
}
