//
//  XACARS.h
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include <string>
#include "DataPoint.h"
#include "Flight.h"
#include "Config.h"

using namespace std;

class XACARS {
private:
    string xacars_data1;
    string xacars_data2;
    string xacars_data3;
    string xacars_data4;
    
    string acars_url;
    string pirep_url;
    string flight_info_url;
    string username;
    string password;
    int report_interval_sec;

    string formatRoute(string);
    
    bool SyncRequest(string url, string *response);
    void AsyncRequest(string url);
    
public:
    XACARS(Config *);
    ~XACARS();
    
    bool testConnection();
    bool getFlightInfo(Flight *);
    void sendPIREP(string flightfile);
    void sendRecording(string flightfile, string datafile);
    void acarsReport(DataPoint *);
    void beginFlight(Flight *, DataPoint);
    void pauseFlight();
    void endFlight();
};
