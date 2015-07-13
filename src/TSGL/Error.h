/*
 * Error.h contains public methods for printing error / debug messages to the console
 */

#ifndef SRC_TSGL_ERROR_H_
#define SRC_TSGL_ERROR_H_

#define TSGL_DEBUG

#include <string>
#include <sstream>

#include <iostream>
#include "Util.h"

namespace tsgl {

template < typename T > std::string to_string( const T& n ) {
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

/*!
 * \brief Prints an error message to the console
 * \details This method prints the string <code>message</code> to the
 *   console in boldface red text.
 *   \param message The message to print to the console
 */
void TsglErr(std::string message);
/*!
 * \brief Prints a debug message to the console
 * \details This method prints the string <code>message</code> to the
 *   console in boldface magenta text.
 * \note This method prints only if DEBUG is defined, and does nothing otherwise.
 *   \param message The message to print to the console
 */
void TsglDebug(std::string message);

}

#endif /* SRC_TSGL_ERROR_H_ */
