#ifndef FUZZY_H
#define FUZZY_H
#include <Arduino.h>

float kering(float b, float c, float d, float x);
float lembab(float a, float b, float c, float x);
float basah(float a, float b, float c, float x);
String* fuzzyfikasi(int sens[]);
String* inferensi(String* sens);
int* defuzzyfikasi(String* sens);
int* fuzzy(int* sens);

#endif
