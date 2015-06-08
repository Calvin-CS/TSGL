/*
 * TsglAssert.h is for unit testing purposes.
 *
 *  Created on: Jun 4, 2015
 *      Author: Chris Dilley.
 */

#ifndef SRC_TSGL_TSGLASSERT_H_
#define SRC_TSGL_TSGLASSERT_H_

#include <iostream>
#ifdef _WIN32
#include <string>
#endif

namespace tsgl {

/*!
 *
 */
static void tsglAssert(bool b, std::string message) {
    if(!b) {
     std::cerr << message << std::endl;
    }
}

#define tsglAssert tsgl::tsglAssert

}

#endif /* SRC_TSGL_TSGLASSERT_H_ */
