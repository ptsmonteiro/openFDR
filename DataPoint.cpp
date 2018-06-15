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

using namespace std;

DataPoint::DataPoint(float elapsed) {
    
    elapsedFlightTime = elapsed;
    flightTimeStamp = readDataF("sim/time/zulu_time_sec");
    
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
    
    oat = round(readDataF("sim/cockpit2/temperature/outside_air_temp_degc"));

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

std::string DataPoint::toCSV(bool headers = false) {
    
    std::stringstream CSV;
    
    if (headers) CSV << "ElapsedFlightTime"; else CSV << elapsedFlightTime;
    CSV << ';';
    
    if (headers) CSV << "FlightTimeStamp"; else CSV << flightTimeStamp;
    CSV << ';';
    
    if (headers) CSV << "HeadingDeg"; else CSV << headingDeg;
    CSV << ';';

    if (headers) CSV << "HeightFt"; else CSV << heightFt;
    CSV << ';';
    
    if (headers) CSV << "AltitudeFt"; else CSV << altitudeFt;
    CSV << ';';
    
    if (headers) CSV << "VerticalSpeedFPM"; else CSV << verticalSpeedFPM;
    CSV << ';';

    if (headers) CSV << "SpeedIAS"; else CSV << speedIAS;
    CSV << ';';
    
    if (headers) CSV << "SpeedMach"; else CSV << speedMach;
    CSV << ';';

    if (headers) CSV << "PitchDeg"; else CSV << pitchDeg;
    CSV << ';';
    
    if (headers) CSV << "BankDeg"; else CSV << bankDeg;
    CSV << ';';

    if (headers) CSV << "Alpha"; else CSV << alpha;
    CSV << ';';
    
    if (headers) CSV << "LoadFactorG"; else CSV << loadFactorG;
    CSV << ';';

    if (headers) CSV << "TrackDeg"; else CSV << trackDeg;
    CSV << ';';
    
    if (headers) CSV << "SpeedGS"; else CSV << speedGS;
    CSV << ';';

    if (headers) CSV << "LatitudeDeg"; else CSV << latitudeDeg;
    CSV << ';';

    if (headers) CSV << "LongitudeDeg"; else CSV << longitudeDeg;
    CSV << ';';

    // weather
    if (headers) CSV << "WindDeg"; else CSV << windDeg;
    CSV << ';';
    if (headers) CSV << "WindKt"; else CSV << windKt;
    CSV << ';';
    if (headers) CSV << "OAT"; else CSV << oat;
    CSV << ';';

    // engines
    if (headers) CSV << "EngineLever1"; else CSV << engineLever1;
    CSV << ';';
    if (headers) CSV << "EngineLever2"; else CSV << engineLever2;
    CSV << ';';
    if (headers) CSV << "EngineLever3"; else CSV << engineLever3;
    CSV << ';';
    if (headers) CSV << "EngineLever4"; else CSV << engineLever4;
    CSV << ';';
    if (headers) CSV << "EngineLever5"; else CSV << engineLever5;
    CSV << ';';
    if (headers) CSV << "EngineLever6"; else CSV << engineLever6;
    CSV << ';';
    if (headers) CSV << "EngineLever7"; else CSV << engineLever7;
    CSV << ';';
    if (headers) CSV << "EngineLever8"; else CSV << engineLever8;
    CSV << ';';

    // engine power
    if (headers) CSV << "EnginePower1"; else CSV << enginePower1;
    CSV << ';';
    if (headers) CSV << "EnginePower2"; else CSV << enginePower2;
    CSV << ';';
    if (headers) CSV << "EnginePower3"; else CSV << enginePower3;
    CSV << ';';
    if (headers) CSV << "EnginePower4"; else CSV << enginePower4;
    CSV << ';';
    if (headers) CSV << "EnginePower5"; else CSV << enginePower5;
    CSV << ';';
    if (headers) CSV << "EnginePower6"; else CSV << enginePower6;
    CSV << ';';
    if (headers) CSV << "EnginePower7"; else CSV << enginePower7;
    CSV << ';';
    if (headers) CSV << "EnginePower8"; else CSV << enginePower8;
    CSV << ';';

    // brakes
    if (headers) CSV << "BrakeLeft"; else CSV << brakeLeft;
    CSV << ';';
    if (headers) CSV << "BrakeRight"; else CSV << brakeRight;
    CSV << ';';
    if (headers) CSV << "ParkingBrake"; else CSV << parkingBrake;
    CSV << ';';

    if (headers) CSV << "GearLeverDown"; else CSV << gearLeverDown;
    CSV << ';';

    // flight controls
    if (headers) CSV << "PitchControl"; else CSV << pitchControl;
    CSV << ';';
    if (headers) CSV << "RollControl"; else CSV << rollControl;
    CSV << ';';
    if (headers) CSV << "YawControl"; else CSV << yawControl;
    CSV << ';';

    // secondary flight controls
    if (headers) CSV << "FlapLever"; else CSV << flapLever;
    CSV << ';';
    if (headers) CSV << "SpeedBrakeLever"; else CSV << speedBrakeLever;
    CSV << ';';

    // telemetry
    if (headers) CSV << "AutopilotOn"; else CSV << autopilotOn;
    CSV << ';';

    // weights
    if (headers) CSV << "WeightKg"; else CSV << weightKg;
    CSV << ';';
    if (headers) CSV << "FuelQuantityKg"; else CSV << fuelQuantityKg;
    CSV << ';';

    // cabin
    if (headers) CSV << "CabinAltFt"; else CSV << cabinAltFt;
    CSV << ';';
    if (headers) CSV << "CabinVsFPM"; else CSV << cabinVsFPM;
    CSV << ';';

    // radio nav
    if (headers) CSV << "Nav1CourseDeg"; else CSV << nav1CourseDeg;
    CSV << ';';
    if (headers) CSV << "Nav1SlopeDeg"; else CSV << nav1SlopeDeg;
    CSV << ';';
    if (headers) CSV << "Nav1DME"; else CSV << nav1DME;
    CSV << ';';
    if (headers) CSV << "LocDevDots"; else CSV << locDevDots;
    CSV << ';';
    if (headers) CSV << "GlideDevDots"; else CSV << glideDevDots;
    CSV << ';';
    
    CSV << std::endl;
    
    return CSV.str();
}

string DataPoint::getXACARSFormattedLocation() {
    // result should be like "N48 7.21916 E16 33.4283"
    string result = "";

    float degrees = 0;
    float minutes = 0.0;
    
    // latitude
    result.append(latitudeDeg > 0 ? "N" : "S");
    minutes = modf(latitudeDeg, &degrees);
    result.append(to_string(round(degrees)) + " " + to_string(minutes));

    result.append(" ");

    // longitude
    result.append(longitudeDeg > 0 ? "E" : "W");
    minutes = modf(longitudeDeg, &degrees);
    result.append(to_string(round(degrees)) + " " + to_string(minutes));
    
    return result;
}
