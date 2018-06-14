//
//  XACARS.cpp
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include "XACARS.h"

XACARS::XACARS() {
    xacars_data1 = "XACARS|1.1";
}

bool XACARS::SyncRequest(std::string url, std::string *response) {
    return true;
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
    std::string response;
    if (!SyncRequest(flight_info_url, &response)) {
        return false;
    }
    
    return true;
}
