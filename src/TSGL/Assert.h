/*
 * Assert.h is for unit testing purposes.
 *
 *  Created on: Jun 4, 2015
 *      Author: Chris Dilley.
 */

#ifndef SRC_TSGL_ASSERT_H_
#define SRC_TSGL_ASSERT_H_

#include <iostream>

/*!
 *
 */
static void assert(bool b, std::string message) {
    if(!b) {
     std::cerr << message << std::endl;
    }
}

#endif /* SRC_TSGL_ASSERT_H_ */
