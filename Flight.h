//
//  Flight.h
//  openFDR
//
//  Created by Pedro on 28/05/2018.
//

#include <stdio.h>
#include <string>

using namespace std;

class Flight {
    
public:
    
    int simulatorVersion = 0;

    string aircraftType = "";
    string aircraftRegistration = "";
    int  aircraftEmptyWeight = 0;
    int  aircraftMaxWeight = 0;

    string flightNumber = "";
    string originICAO = "";
    string destinationICAO = "";
    string alternateICAO = "";
    int  cruiseAltitude = 0;
    string route = "";
    
    string rules = "IFR"; // IFR/VFR
    string eobt = "";
    string eet = "";
    string comments = "";
    
    string online = "";
    string onlineID = "";
    int pax = 0;
    int cargoKg = 0;
    
    Flight();
    ~Flight();
    
    void toCSV(std::string);
    void reset();
    void planeLoaded();
};
