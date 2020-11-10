//
//  Config.cpp
//  openFDR
//
//  Created by Pedro on 28/05/2018.
//

#include "Config.h"
#include "XPLMPlugin.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <regex>
#include <string.h>
using namespace std;

Config::Config() {
}

Config::~Config() {
}

void Config::Load() {
    string plugin_dir_path;
    string line;

    char plugin_file_path[256];
    XPLMGetPluginInfo(XPLMGetMyID(), NULL, plugin_file_path, NULL, NULL);
    
    ifstream configfile("Resources/plugins/openFDR/64/config.ini");
    if (configfile.is_open()) {
        while (getline(configfile, line)) {
            ProcessLine(line);
        }
    }
    
    configfile.close();
}

bool Config::ScanLineParameter(string line, string parameter, void *value, bool isNumber = false) {
    smatch match;
    if (regex_match(line, match, regex(parameter + "\\s*\\=\\s*(.+)"))) {
        if (isNumber) {
            *(int *)value = stoi(match[1].str());
        }
        else {
            *(string *)value = match[1].str();
        }
        return true;
    }
    return false;
}

void Config::ProcessLine(std::string line) {
    if (ScanLineParameter(line, "xacars_acars_url",         &xacars_acars_url)) return;
    if (ScanLineParameter(line, "xacars_pirep_url",         &xacars_pirep_url)) return;
    if (ScanLineParameter(line, "xacars_flight_info_url",   &xacars_flight_info_url)) return;
    if (ScanLineParameter(line, "xacars_username",          &xacars_username)) return;
    if (ScanLineParameter(line, "xacars_password",          &xacars_password)) return;
    if (ScanLineParameter(line, "xacars_pos_report_min",    &xacars_pos_report_min, true)) return;
    if (ScanLineParameter(line, "openfdr_url",              &openfdr_url)) return;
}
