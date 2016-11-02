#ifndef _UTIL_
#define _UTIL_

#include <cmath>
#include <cstdio>

const double PI = M_PI;
const double RAD = PI / 180;

const int FPS = 60;
const float FRAME = 1.0f/FPS;

typedef long double Decimal;

const int NUM_COLORS = 256, MAX_COLOR = 255;

inline bool clamp(float &f, float min, float max){
  if(f < min){
    f = min;
    return true;
  }
  if(f > max){
    f = max;
    return true;
  }
  return false;
}

inline bool clamp(int &i, int min, int max){
  if(i < min){
    i = min;
    return true;
  }
  if(i > max){
    i = max;
    return true;
  }
  return false;
}

inline bool fileExists(const std::string &name){
  if(FILE *file = fopen(name.c_str(), "r")){
    fclose(file);
    return true;
  }
  else
    return false;
}

#endif 
