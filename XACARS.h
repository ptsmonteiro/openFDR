//
//  XACARS.h
//  openFDR
//
//  Created by Pedro on 12/06/2018.
//

#include <string>
#include "DataPoint.h"
#include "Flight.h"

class XACARS {

    std::string xacars_data1;
    std::string xacars_data2;
    std::string xacars_data3;
    std::string xacars_data4;
    
    std::string acars_url;
    std::string pirep_url;
    std::string flight_info_url;
    std::string username;
    std::string password;
    int report_interval_sec;
    
    bool SyncRequest(std::string url, std::string *response);
    void AsyncRequest(std::string url);
    
public:
    XACARS();
    ~XACARS();
    
    bool testConnection();
    bool getFlightInfo(Flight *);
    void sendPIREP(std::string flightfile);
    void sendRecording(std::string flightfile, std::string datafile);
    void acarsReport(DataPoint *);
    void startFlight();
    void pauseFlight();
    void endFlight();
};
