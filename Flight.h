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
    string origionICAO = "";
    string destinationICAO = "";
    string alternateICAO = "";
    int  cruiseAltitude = 0;
    string route = "";
    
    Flight();
    ~Flight();
    
    void toCSV(std::string);
    void reset();
};
