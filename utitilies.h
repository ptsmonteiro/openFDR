//
//  utitilies.hpp
//  openFDR
//
//  Created by Pedro on 16/04/2018.
//

#include <chrono>
#include <ctime>
#include <string>
#include <stdio.h>

using namespace std;

int readDataI(const char *dataref);
float readDataF(const char *dataref);
int readDataB(const char *dataref, void *buf, int size);
int readDataVF(const char *dataref, float *vector, int size);
int readDataVI(const char *dataref, int *vector, int size);

float getSimUnixTimestamp();
