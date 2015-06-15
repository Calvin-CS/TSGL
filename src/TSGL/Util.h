/*
 * Util.h
 *
 *  Created on: Jun 5, 2015
 *      Author: pretzel
 */

#ifndef SRC_TSGL_UTIL_H_
#define SRC_TSGL_UTIL_H_

namespace tsgl {

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
