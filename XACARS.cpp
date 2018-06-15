//
//  XACARS.cpp
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include "XACARS.h"
#include <fstream>
#include <string>

using namespace std;

XACARS::XACARS() {
    xacars_data1 = "XACARS|1.1";
}

bool XACARS::SyncRequest(std::string url, std::string *response) {
    char buffer[4096] = "";
    //http url request  -> buffer
    
    string rawresponse(buffer);
    
    int result = stoi(rawresponse.substr(0, rawresponse.find('|')));
    *response = rawresponse.substr(rawresponse.find('|')+1, rawresponse.length());
    
    return (result == 1);
}

void XACARS::AsyncRequest(std::string url) {
    char buffer[4096] = "";
    //http url async request  -> buffer
}

bool XACARS::testConnection() {
    xacars_data2 = "TEST";
    xacars_data3 = username;
    xacars_data4 = "";

    return SyncRequest(acars_url, NULL);
}

bool XACARS::getFlightInfo(Flight *flight) {
    xacars_data2 = flight->flightNumber;
    xacars_data3 = "";
    xacars_data4 = "";
    
    string response;
    if (!SyncRequest(flight_info_url, &response)) {
        return false;
    }

    string buffer;
    istringstream rawresponse(response);
    
    getline(rawresponse, flight->origionICAO);
    getline(rawresponse, flight->destinationICAO);
    getline(rawresponse, flight->alternateICAO);
    getline(rawresponse, flight->route);
    getline(rawresponse, buffer); // max pax
    getline(rawresponse, buffer); // max cargo
    getline(rawresponse, buffer); // IFR/VFR
    getline(rawresponse, flight->aircraftRegistration);
    getline(rawresponse, buffer); // cruise altitude
    flight->cruiseAltitude = stoi(buffer);

    return true;
}
