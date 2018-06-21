//
//  XACARS.h
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include <string>
#include <ctime>
#include <curl/curl.h>
#include "DataPoint.h"
#include "Flight.h"
#include "Config.h"

using namespace std;

class XACARS {
private:
    
    CURL *curl;
    
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
    string formatDateTime(time_t);
    string formatDurationFromSeconds(int);
    string formatDegreesMinutes(float, bool);
    string getFormattedLocation(float, float);
    
    bool SyncRequest(string url, string *response);
    void AsyncRequest(string url);
    
public:
    XACARS(Config *);
    ~XACARS();

    static string curl_received_data;
    static size_t curlDataHandler(void *, size_t, size_t, void *);

    bool testConnection();
    bool getFlightInfo(Flight *);
    void sendPIREP(Flight *);
    void sendRecording(string flightfile, string datafile);
    void acarsReport(DataPoint *);
    void beginFlight(Flight *, DataPoint);
    void pauseFlight();
    void endFlight();
};
