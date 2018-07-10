//
//  utitilies.cpp
//  openFDR
//
//  Created by Pedro on 16/04/2018.
//

#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "utitilies.h"

int readDataI(const char *dataref){
    int value = 0;
    value = XPLMGetDatai(XPLMFindDataRef(dataref));
    return value;
}

float readDataF(const char *dataref) {
    float value = 0;
    value = XPLMGetDataf(XPLMFindDataRef(dataref));
    return value;
}

int readDataB(const char *dataref, void *buf, int size) {
    int read = 0;
    read = XPLMGetDatab(XPLMFindDataRef(dataref), buf, 0, size - 1);
    return read;
}

int readDataVF(const char *dataref, float *vector, int size) {
    int i = 0;
    for (i=0; i<size; i++) {
        vector[i] = 0;
    }
    
    int readValues = 0;
    readValues = XPLMGetDatavf(XPLMFindDataRef(dataref), vector, 0, size-1);
    return readValues;
}

int readDataVI(const char *dataref, int *vector, int size) {
    int i = 0;
    for (i=0; i<size; i++) {
        vector[i] = 0;
    }
    
    int readValues = 0;
    readValues = XPLMGetDatavi(XPLMFindDataRef(dataref), vector, 0, size-1);
    return readValues;
}

float getSimUnixTimestamp() {
    int localdays = readDataI("sim/time/local_date_days");
    float zulu_time_secs = readDataF("sim/time/zulu_time_sec");
    
    time_t t;
    time(&t);
    
    struct tm *timeinfo = gmtime(&t);
    timeinfo->tm_mon = 1;
    timeinfo->tm_mday += localdays;
    t = mktime(timeinfo);
    
    return (t + zulu_time_secs);
}

void devConsole(string m) {
    string message = "openFDR: " + m + "\n";
    XPLMDebugString(message.c_str());
}
