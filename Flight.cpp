//
//  Flight.cpp
//  openFDR
//
//  Created by Pedro on 28/05/2018.
//

#include "Flight.h"
#include "utitilies.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

Flight::Flight() {
    reset();
}

Flight::~Flight() {
    
}

void Flight::reset() {
    char buffer[1024];
    simulatorVersion = readDataI("sim/version/xplane_internal_version");
    
    readDataB("sim/aircraft/view/acf_ICAO", buffer, sizeof(buffer));
    aircraftType = string(buffer);
    
    readDataB("sim/aircraft/view/acf_tailnum", buffer, sizeof(buffer));
    aircraftRegistration = string(buffer);
    
    aircraftEmptyWeight = round(readDataF("sim/aircraft/weight/acf_m_empty"));
    aircraftMaxWeight = round(readDataF("sim/aircraft/weight/acf_m_max"));
}

void Flight::toCSV(std::string name) {
    using namespace std;
    
    char filename[1024];
    sprintf(filename, "%s.openfdr.flight.csv", name.c_str());
    
    ofstream outfile;
    outfile.open(filename);

    outfile << to_string(simulatorVersion);
    outfile << ";";
    outfile << aircraftType;
    outfile << ";";
    outfile << aircraftRegistration;
    outfile << ";";
    outfile << to_string(aircraftEmptyWeight);
    outfile << ";";
    outfile << to_string(aircraftMaxWeight);
    outfile << ";";
    outfile << flightNumber;
    outfile << ";";
    outfile << origionICAO;
    outfile << ";";
    outfile << destinationICAO;
    outfile << ";";
    outfile << alternateICAO;
    outfile << ";";
    outfile << to_string(cruiseAltitude);
    outfile << ";";
    outfile << route;
    outfile << endl;
    
    outfile.close();
}

