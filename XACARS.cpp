//
//  XACARS.cpp
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include "XACARS.h"
#include <fstream>
#include <string>
#include <cmath>
#include "XPLMUtilities.h"

using namespace std;

string XACARS::curl_received_data;

XACARS::XACARS(Config *config) {
    xacars_data1 = "XACARS|1.1";
    
    acars_url = config->xacars_acars_url;
    flight_info_url = config->xacars_flight_info_url;
    pirep_url = config->xacars_pirep_url;
    username = config->xacars_username;
    password = config->xacars_password;
    
    curl = curl_easy_init();
}

XACARS::~XACARS() {
    curl_easy_cleanup(curl);
}

size_t XACARS::curlDataHandler(void *ptr, size_t size, size_t nmemb, void *ourpointer) {
    string chunk((char *) ptr, size * nmemb);
    XACARS::curl_received_data.append(chunk);
    return chunk.length();
}


bool XACARS::SyncRequest(std::string url, std::string *response) {
    
    string query_url(url);
    query_url.append(string("?data1=") + xacars_data1);
    query_url.append(string("&data2=") + xacars_data2);
    query_url.append(string("&data3=") + xacars_data3);
    query_url.append(string("&data4=") + xacars_data4);
    
    string message = "openFDR: SyncRequest request: " + query_url;
    XPLMDebugString(message.c_str());
    
    curl_received_data.erase();
    curl_easy_reset(curl);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, XACARS::curlDataHandler);
    if (curl_easy_perform(curl) != CURLE_OK) {
        return false;
    }

    message = "openFDR: SyncRequest received: " + curl_received_data;
    XPLMDebugString(message.c_str());
    
    int result = stoi(curl_received_data.substr(0, curl_received_data.find('|')));
    *response = curl_received_data.substr(curl_received_data.find('|')+1, curl_received_data.length());
    
    return (result == 1);
}

void XACARS::AsyncRequest(std::string url) {
    //http url async request

    string message = "openFDR: AsyncRequest: ";
    message.append(string("data1: ") + xacars_data1 + ", ");
    message.append(string("data2: ") + xacars_data2 + ", ");
    message.append(string("data3: ") + xacars_data3 + ", ");
    message.append(string("data4: ") + xacars_data4 + "\n");
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
    
    getline(rawresponse, flight->originICAO);
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
    payload.append(getFormattedLocation(datapoint.latitudeDeg, datapoint.longitudeDeg) + "|");
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

void XACARS::sendPIREP(Flight *flight) {
    xacars_data2 = "";
    xacars_data3 = "";
    xacars_data4 = "";
    /*
     DATA2 includes all the data necessary for the PIREP as fields separated by "~". In the following listing each field is a new line, but see the data example below how it really looks like:
     
     Username
     Password
     Flightnumber
     Aircraft REG (also used for ICAO)
     Altitude/Flightlevel
     Flightrules (IFR or VFR)
     Departure ICAO
     Destination ICAO
     Alternate ICAO
     Departuretime (dd.mm.yyyy hh:mm)
     Blocktime (hh:mm)
     Flighttime (hh:mm)
     Blockfuel
     Flightfuel
     Pax
     Cargo
     Online (VATSIM|ICAO|[other])
     OUT (UNIX timestamp) - time of engine start
     OFF (UNIX timestamp) - time of takeoff
     ON (UNIX timestamp) - time of landing
     IN (UNIX timestamp) - time of engine stop
     ZFW - zero fuel weight in lbs
     TOW - take off weight in lbs
     LW - landing weight in lbs
     OUT latitude (N/Sxx xx.xxxx)
     OUT longitude (E/Wxx xx.xxxx)
     OUT altitude in ft
     IN latitude (N/Sxx xx.xxxx)
     IN longitude (E/Wxx xx.xxxx)
     IN altitude in ft
     max CLIMB in ft/min
     max DESCEND in ft/min
     max IAS in kt
     max GS in kt
     A full parameterlist can look like this (spaces inserted for a better readability):
     
     pirep.php?DATA1=XACARS|1.1&DATA2=xactesting~ xactestingpass~ xac1001~ F100~ 24000~ IFR~ LOWW~ LOWI~ EDDM~ 01.07.2009 18:32~02:04~01:27~ 1980~ 1456~ 72~ 2100~VATSIM~ 123456719~ 123456729~ 123456739~ 123456749~ 22000~ 25000~ 23000~ N43 12.2810~ E18 12.3802~ 630~ N43 12.2810~ E18 12.3802~ 320~ 2347~ 3202~ 290~ 450
     */

    xacars_data2 += username + "~";
    xacars_data2 += password + "~";
    xacars_data2 += flight->flightNumber + "~";
    xacars_data2 += flight->aircraftType + "~";
    xacars_data2 += to_string(flight->cruiseAltitude) + "~";
    xacars_data2 += flight->rules + "~"
    ;
    xacars_data2 += flight->originICAO + "~";
    xacars_data2 += flight->destinationICAO + "~";
    xacars_data2 += flight->alternateICAO + "~";
    
    xacars_data2 += formatDateTime(flight->timeOut) + "~";
    xacars_data2 += formatDurationFromSeconds((int) flight->timeIn - (int) flight->timeOut) + "~";
    xacars_data2 += formatDurationFromSeconds((int) flight->timeOn - (int) flight->timeOff) + "~";
    xacars_data2 += to_string(flight->pax) + "~";
    xacars_data2 += to_string(flight->cargoKg) + "~"; // TODO this needs kg -> lbs conversion
    xacars_data2 += flight->online + "~";
    xacars_data2 += to_string(flight->timeOut) + "~";
    xacars_data2 += to_string(flight->timeOff) + "~";
    xacars_data2 += to_string(flight->timeOn) + "~";
    xacars_data2 += to_string(flight->timeIn) + "~";
    xacars_data2 += to_string(flight->zfw) + "~"; // TODO this needs kg -> lbs conversion
    xacars_data2 += to_string(flight->tow) + "~"; // TODO this needs kg -> lbs conversion
    xacars_data2 += to_string(flight->lw) + "~"; // TODO this needs kg -> lbs conversion
    xacars_data2 += formatDegreesMinutes(flight->outLat, true) + "~";
    xacars_data2 += formatDegreesMinutes(flight->outLong, false) + "~";
    xacars_data2 += to_string(flight->outAlt) + "~";
    xacars_data2 += formatDegreesMinutes(flight->inLat, true) + "~";
    xacars_data2 += formatDegreesMinutes(flight->inLong, false) + "~";
    xacars_data2 += to_string(flight->inAlt) + "~";
    xacars_data2 += to_string(flight->maxClimb) + "~";
    xacars_data2 += to_string(flight->maxDescent) + "~";
    xacars_data2 += to_string(flight->maxIAS) + "~";
    xacars_data2 += to_string(flight->maxGS) + "~";

    AsyncRequest(pirep_url);
}

string XACARS::formatRoute(string route) {
    string formatted(route);
    replace(formatted.begin(), formatted.end(), ' ', '~');
    return formatted;
}

string XACARS::formatDateTime(time_t t) {
    char buffer[1024];
    struct tm *timeinfo = gmtime(&t);
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %m.%s", timeinfo);
    return string(buffer);
}

string XACARS::formatDurationFromSeconds(int s) {
    int hours = floor(s / 3600);
    int minutes = floor((s % 3600) / 60);
    char buffer[8];
    sprintf(buffer, "%2d:%2d", hours, minutes);
    return string(buffer);
}

string XACARS::formatDegreesMinutes(float c, bool isLatitude) {
    // result should be like "N48 7.21916" or "E16 33.4283"
    string result = "";
    
    float degrees = 0;
    float minutes = 0.0;

    if (isLatitude) {
        result.append(c > 0 ? "N" : "S");
    }
    else {
        result.append(c > 0 ? "E" : "W");
    }
    minutes = modf(c, &degrees) * 60;
    result.append(to_string((int) round(degrees)) + " " + to_string(minutes));
    return result;
}

string XACARS::getFormattedLocation(float lon, float lat) {
    // result should be like "N48 7.21916 E16 33.4283"
    string result = formatDegreesMinutes(lat, true) + " " + formatDegreesMinutes(lon, false);
    return result;
}
