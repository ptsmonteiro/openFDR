//
//  Config.h
//  openFDR
//
//  Created by Pedro on 28/05/2018.
//
#include <stdio.h>
#include <string>
using namespace std;

#define CONFIG_FILE "config.ini"

class Config {
protected:
    void ProcessLine(string);
    bool ScanLineParameter(string, string, void *, bool);
    
public:
    string xacars_acars_url;
    string xacars_pirep_url;
    string xacars_flight_info_url;
    string xacars_username;
    string xacars_password;
    int xacars_pos_report_min;
    string openfdr_url;
    
    Config();
    ~Config();
    
    void Load();
};
