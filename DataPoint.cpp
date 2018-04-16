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
#include "utitilies.h"

DataPoint::DataPoint(float ElapsedSim) {
    
    simulatorElapsedTimeSec = round(ElapsedSim);
    
    // flight data
    headingDeg = round(readDataF("sim/cockpit2/gauges/indicators/heading_vacuum_deg_mag_pilot"));
    heightFt = round(readDataF("sim/cockpit2/gauges/indicators/radio_altimeter_height_ft_pilot"));
    altitudeFt = round(readDataF("sim/flightmodel/misc/h_ind"));
    verticalSpeedFPM = round(readDataF("sim/flightmodel/position/vh_ind_fpm"));
    
    speedIAS = round(readDataF("sim/flightmodel/position/indicated_airspeed"));
    speedMach = readDataF("sim/flightmodel/misc/machno");
    
    pitchDeg = readDataF("sim/flightmodel/position/true_theta");
    bankDeg = readDataF("sim/flightmodel/position/true_phi");
    
    alpha = readDataF("sim/flightmodel/position/alpha");
    loadFactorG = readDataF("sim/flightmodel/misc/g_total");
    
    // navigation data
    float trueTrack = readDataF("sim/flightmodel/position/hpath");
    float magVar = readDataF("sim/flightmodel/position/magnetic_variation");
    
    trackDeg = round(trueTrack + magVar);
    speedGS = round(readDataF("sim/cockpit/radios/gps_dme_speed_kts"));
    
    latitudeDeg = readDataF("sim/flightmodel/position/latitude");
    longitudeDeg = readDataF("sim/flightmodel/position/longitude");

    // weather
    windDeg = round(readDataF("sim/cockpit2/gauges/indicators/wind_heading_deg_mag"));
    windKt = round(readDataF("sim/cockpit2/gauges/indicators/wind_speed_kts"));
    
    // engines
    
    float tr_engines[8];
    readDataVF("sim/flightmodel/engine/ENGN_thro", tr_engines, 8);
    engineLever1 = round(tr_engines[0] * 100);
    engineLever2 = round(tr_engines[1] * 100);
    engineLever3 = round(tr_engines[2] * 100);
    engineLever4 = round(tr_engines[3] * 100);
    engineLever5 = round(tr_engines[4] * 100);
    engineLever6 = round(tr_engines[5] * 100);
    engineLever7 = round(tr_engines[6] * 100);
    engineLever8 = round(tr_engines[7] * 100);

    // engine power
    float pwr_engines[8];
    readDataVF("sim/flightmodel/engine/ENGN_power", pwr_engines, 8);
    enginePower1 = round(pwr_engines[0] * 100);
    enginePower2 = round(pwr_engines[1] * 100);
    enginePower3 = round(pwr_engines[2] * 100);
    enginePower4 = round(pwr_engines[3] * 100);
    enginePower5 = round(pwr_engines[4] * 100);
    enginePower6 = round(pwr_engines[5] * 100);
    enginePower7 = round(pwr_engines[6] * 100);
    enginePower8 = round(pwr_engines[7] * 100);
    
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
    
    // weights
    weightKg = round(readDataF("sim/flightmodel/weight/m_total"));
    fuelQuantityKg = round(readDataF("sim/flightmodel/weight/m_fuel_total"));
    
    // cabin
    cabinAltFt = round(readDataF("sim/cockpit2/pressurization/indicators/cabin_altitude_ft"));
    cabinVsFPM = round(readDataF("sim/cockpit2/pressurization/indicators/cabin_vvi_fpm"));

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

