/*
 * TsglAssert.h is for unit testing purposes.
 */

#ifndef SRC_TSGL_TSGLASSERT_H_
#define SRC_TSGL_TSGLASSERT_H_

#include <iostream>
#include "Error.h"
#ifdef _WIN32
#include <string>
#endif

namespace tsgl {

/*!
 * \brief Assertion used in Unit testing.
 * \details Assertion mechanism used in Unit testing where a function
 * that returns a boolean is passed and an error message is given if the function returns false.
 *    \param b A boolean returned from a Unit test function.
 *    \param message A string representing the error message to print out if b is false.
 * \note Can also pass a boolean expression; it doesn't necessarily have to be a function.
 * \return Nothing if the function returns true, message in red text if it returns false.
 */
static void tsglAssert(bool b, std::string message) {
    if(!b) {
     TsglErr(message);  //Changed it so that now it uses the TsglErr() function
    }
}

#define tsglAssert tsgl::tsglAssert

}

#endif /* SRC_TSGL_TSGLASSERT_H_ */
