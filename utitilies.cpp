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
