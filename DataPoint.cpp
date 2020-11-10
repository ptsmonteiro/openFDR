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
#include "utilities.h"

using namespace std;

DataPoint::DataPoint(float elapsed) {
    
	char bytes[1024];
	
	// properties
	simulatorVersion = readDataI("sim/version/xplane_internal_version");

	readDataB("sim/aircraft/view/acf_ICAO", bytes, sizeof(bytes));
	aircraftType = string(bytes);

	aircraftNumberOfEngines = readDataI("sim/aircraft/engine/acf_num_engines");

	readDataB("sim/aircraft/view/acf_tailnum", bytes, sizeof(bytes));
	aircraftRegistration = string(bytes);

	aircraftEmptyWeightKg = round(readDataF("sim/aircraft/weight/acf_m_empty"));
	aircraftMaxWeightKg = round(readDataF("sim/aircraft/weight/acf_m_max"));

	// time     
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
	readDataVI("sim/flightmodel/engine/ENGN_running", engineRunning, 8);
    
    float tr_engines[8];
    readDataVF("sim/flightmodel/engine/ENGN_thro", tr_engines, 8);
	for (int i = 0; i < aircraftNumberOfEngines; i++) {
		engineLever[i] = round(tr_engines[i] * 100);
	}

    // engine power
    float pwr_engines[8];
    readDataVF("sim/cockpit2/engine/indicators/power_watts", pwr_engines, 8);
	for (int i = 0; i < aircraftNumberOfEngines; i++) {
		enginePowerWatt[i] = round(pwr_engines[i]);
	}

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
    readDataB("sim/cockpit2/radios/indicators/nav1_nav_id", bytes, sizeof(bytes));
	nav1Id = string(bytes);

    nav1CourseDeg = round(readDataF("sim/cockpit/radios/nav1_course_degm"));
    nav1SlopeDeg = readDataF("sim/cockpit/radios/nav1_slope_degt");
    nav1DME = readDataF("sim/cockpit/radios/nav1_dme_dist_m");
    locDevDots = readDataF("sim/cockpit/radios/nav1_hdef_dot");
    glideDevDots = readDataF("sim/cockpit/radios/nav1_vdef_dot");

}

DataPoint::~DataPoint() {
    
}

string DataPoint::toJSON() {

	stringstream buffer;
	int i = 0;

	// properties
	buffer << "\"simulatorVersion\": " << simulatorVersion << "," << std::endl;
	buffer << "\"aircraftType\": \"" << aircraftType << "\"," << std::endl;
	buffer << "\"aircraftNumberOfEngines\": " << aircraftNumberOfEngines << "," << std::endl;
	buffer << "\"aircraftRegistration\": \"" << aircraftRegistration << "\"," << std::endl;
	buffer << "\"aircraftEmptyWeightKg\": " << aircraftEmptyWeightKg << "," << std::endl;
	buffer << "\"aircraftMaxWeightKg\": " << aircraftMaxWeightKg << "," << std::endl;

	// time
	buffer << "\"elapsedFlightTime\": " << elapsedFlightTime << "," << std::endl;
	buffer << "\"flightTimeStamp\": " << flightTimeStamp << "," << std::endl;

	// flight and navigation
	buffer << "\"headingDeg\": " << headingDeg << "," << std::endl;
	buffer << "\"heightFt\": " << heightFt << "," << std::endl;
	buffer << "\"altitudeFt\": " << altitudeFt << "," << std::endl;
	buffer << "\"verticalSpeedFPM\": " << verticalSpeedFPM << "," << std::endl;
	buffer << "\"speedIAS\": " << speedIAS << "," << std::endl;
	buffer << "\"speedMach\": " << speedMach << "," << std::endl;
	buffer << "\"pitchDeg\": " << pitchDeg << "," << std::endl;
	buffer << "\"bankDeg\": " << bankDeg << "," << std::endl;
	buffer << "\"alpha\": " << alpha << "," << std::endl;
	buffer << "\"loadFactorG\": " << loadFactorG << "," << std::endl;
	buffer << "\"trackDeg\": " << trackDeg << "," << std::endl;
	buffer << "\"speedGS\": " << speedGS << "," << std::endl;
	buffer << "\"latitudeDeg\": " << latitudeDeg << "," << std::endl;
	buffer << "\"longitudeDeg\": " << longitudeDeg << "," << std::endl;

	// weather
	buffer << "\"windDeg\": " << windDeg << "," << std::endl;
	buffer << "\"windKt\": " << windKt << "," << std::endl;
	buffer << "\"oat\": " << oat << "," << std::endl;

	// engines

	// running
	buffer << "\"engineRunning\": [";
	for (i = 0; i < aircraftNumberOfEngines-1; i++) {
		buffer << engineRunning[i] << ", ";
	}
	buffer << engineRunning[i] << "]" << std::endl;

	// thrust lever
	buffer << "\"engineLever\": [";
	for (i = 0; i < aircraftNumberOfEngines - 1; i++) {
		buffer << engineLever[i] << ", ";
	}
	buffer << engineLever[i] << "]" << std::endl;

	// power
	buffer << "\"enginePowerWatt\": [";
	for (i = 0; i < aircraftNumberOfEngines - 1; i++) {
		buffer << enginePowerWatt[i] << ", ";
	}
	buffer << enginePowerWatt[i] << "]" << std::endl;

	// brakes
	buffer << "\"brakeLeft\": " << brakeLeft << "," << std::endl;
	buffer << "\"brakeRight\": " << brakeRight << "," << std::endl;
	buffer << "\"parkingBrake\": " << parkingBrake << "," << std::endl;
	buffer << "\"gearLeverDown\": " << gearLeverDown << "," << std::endl;

	// flight controls
	buffer << "\"pitchControl\": " << pitchControl << "," << std::endl;
	buffer << "\"rollControl\": " << rollControl << "," << std::endl;
	buffer << "\"yawControl\": " << yawControl << "," << std::endl;

	// secondary flight controls
	buffer << "\"flapLever\": " << flapLever << "," << std::endl;
	buffer << "\"speedBrakeLever\": " << speedBrakeLever << "," << std::endl;

	// telemetry
	buffer << "\"autopilotOn\": " << autopilotOn << "," << std::endl;

	// weights
	buffer << "\"weightKg\": " << weightKg << "," << std::endl;
	buffer << "\"fuelQuantityKg\": " << fuelQuantityKg << "," << std::endl;

	// cabin
	buffer << "\"cabinAltFt\": " << cabinAltFt << "," << std::endl;
	buffer << "\"cabinVsFPM\": " << cabinVsFPM << "," << std::endl;

	// radio nav
	buffer << "\"nav1Id\": \"" << nav1Id << "\"," << std::endl;
	buffer << "\"nav1CourseDeg\": " << nav1CourseDeg << "," << std::endl;
	buffer << "\"nav1SlopeDeg\": " << nav1SlopeDeg << "," << std::endl;
	buffer << "\"nav1DME\": " << nav1DME << "," << std::endl;
	buffer << "\"locDevDots\": " << locDevDots << "," << std::endl;
	buffer << "\"glideDevDots\": " << glideDevDots << std::endl;

	string dataJSON = "{\n" + buffer.str() + "}\n";
	buffer.clear();
	return dataJSON;
}

string DataPoint::getXACARSFormattedLocation() {
    // result should be like "N48 7.21916 E16 33.4283"
    string result = "";

    float degrees = 0;
    float minutes = 0.0;
    
    // latitude
    result.append(latitudeDeg > 0 ? "N" : "S");
    minutes = modf(latitudeDeg, &degrees) * 60;
    result.append(to_string((int) round(degrees)) + " " + to_string(minutes));

    result.append(" ");

    // longitude
    result.append(longitudeDeg > 0 ? "E" : "W");
    minutes = modf(longitudeDeg, &degrees) * 60;
    result.append(to_string((int) round(degrees)) + " " + to_string(minutes));
    
    return result;
}
