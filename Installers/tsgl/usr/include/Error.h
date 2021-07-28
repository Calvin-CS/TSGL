/*
 * Error.h contains public methods for printing error / debug messages to the console
 */

#ifndef SRC_TSGL_ERROR_H_
#define SRC_TSGL_ERROR_H_

//#define TSGL_DEBUG

#include <string>
#include <sstream>

#include <iostream>

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

namespace tsgl {

template < typename T > std::string to_string( const T& n ) {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

void TsglErr(std::string message);

void TsglDebug(std::string message);

}

#endif /* SRC_TSGL_ERROR_H_ */
