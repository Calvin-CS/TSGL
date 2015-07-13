/*
 * Util.h contains commonly used constants and typedefs as well as some utility functions.
 *
 *  Created on: Jun 5, 2015
 *      Author: pretzel
 */

#ifndef SRC_TSGL_UTIL_H_
#define SRC_TSGL_UTIL_H_

#include <cmath>  //To determine M_PI and is also used for math operations

namespace tsgl {
//Constants to be used by TSGL classes and tests
//Tests
#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

//Classes
//Canvas.h
#define FPS 60              // Frames per second
#define FRAME 1.0f/FPS      // Number of seconds between frames

/*!
 * \var typedef long double Decimal
 * \brief A type definition for a long double.
 * \note Used by: CartesianCanvas, Function, and IntegralViewer.
 */
typedef long double Decimal;  // Define the variable type to use for coordinates

//Color.h
const int NUM_COLORS = 256, MAX_COLOR = 255;

//Error.h
#ifdef _WIN32
  #include <string>
  #ifndef _INC_WINDOWS
    #include<windows.h>
  #endif /*_INC_WINDOWS*/
  #define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)
#else
  const std::string TSGL_RED = "\033[1;31m";
  const std::string TSGL_MGN = "\033[1;35m";
  const std::string TSGL_CLR = "\033[0m";
#endif

inline bool clamp(float& f, float min, float max) {
  if (f < min) {
    f = min; return true;
  }
  if (f > max) {
    f = max; return true;
  }
  return false;
}

inline bool clamp(int& i, int min, int max) {
  if (i < min) {
    i = min; return true;
  }
  if (i > max) {
    i = max; return true;
  }
  return false;
}

}

#endif /* SRC_TSGL_UTIL_H_ */
