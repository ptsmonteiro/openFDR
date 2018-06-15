//
//  XACARS.cpp
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include "XACARS.h"
#include <fstream>
#include <string>
#include "XPLMUtilities.h"

using namespace std;

XACARS::XACARS(Config *config) {
    xacars_data1 = "XACARS|1.1";
}

bool XACARS::SyncRequest(std::string url, std::string *response) {
    char buffer[4096] = "";
    //http url request  -> buffer

    string message = "openFDR: SyncRequest: ";
    message.append(string("data1: ") + xacars_data1 + ", ");
    message.append(string("data2: ") + xacars_data2 + ", ");
    message.append(string("data3: ") + xacars_data3 + ", ");
    message.append(string("data4: ") + xacars_data4);
    XPLMDebugString(message.c_str());
    
    return true;

    
    string rawresponse(buffer);
    
    int result = stoi(rawresponse.substr(0, rawresponse.find('|')));
    *response = rawresponse.substr(rawresponse.find('|')+1, rawresponse.length());
    
    return (result == 1);
}

void XACARS::AsyncRequest(std::string url) {
    //http url async request

    string message = "openFDR: AsyncRequest: ";
    message.append(string("data1: ") + xacars_data1 + ", ");
    message.append(string("data2: ") + xacars_data2 + ", ");
    message.append(string("data3: ") + xacars_data3 + ", ");
    message.append(string("data4: ") + xacars_data4);
    XPLMDebugString(message.c_str());
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

void XACARS::beginFlight(Flight *flight, DataPoint datapoint) {
    
    xacars_data2 = "BEGINFLIGHT";
    xacars_data3 = "";
    xacars_data4 = "";

    /*
     The flightdata in the BEGINFLIGHT message is sent as fields separated by a pipe "|":
     
     Username
     [empty]
     Flightnumber
     Aircraft ICAO
     [empty]
     Flightplan (spaces are sent as "~")
     Position (e.g. "N48 7.21916 E16 33.4283")
     Altitude
     [empty]
     [empty]
     [empty]
     Fuel on board
     Heading
     Wind (e.g. "31012" for 310°, 12kts)
     OAT
     Flightrules (IFR or VFR)
     Distance flown (0 at this time)
     Password for the user
     */

    string payload = "";
    payload.append(username + "|");
    payload.append("|");
    payload.append(flight->flightNumber + "|");
    payload.append(flight->aircraftType + "|");
    payload.append("|");
    payload.append(formatRoute(flight->route) + "|");
    payload.append(datapoint.getXACARSFormattedLocation() + "|");
    payload.append(to_string(flight->cruiseAltitude) + "|");
    payload.append("|");
    payload.append("|");
    payload.append("|");
    payload.append(to_string(datapoint.fuelQuantityKg) + "|");
    payload.append(to_string(datapoint.headingDeg) + "|");
    payload.append(string("00000") + "|");
    payload.append(to_string(datapoint.oat) + "|");
    payload.append(flight->rules + "|");
    payload.append(string("0") + "|");
    payload.append(password);
    
    xacars_data3 = payload;
    
    AsyncRequest(acars_url);
}

void XACARS::endFlight() {
    xacars_data2 = "ENDFLIGHT";
    xacars_data3 = "";
    xacars_data4 = "";
    AsyncRequest(acars_url);
}

string XACARS::formatRoute(string route) {
    string formatted(route);
    replace(formatted.begin(), formatted.end(), ' ', '~');
    return formatted;
}
