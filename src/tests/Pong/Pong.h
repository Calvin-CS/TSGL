/*
 * Pong.h
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#ifndef PONG_H_
#define PONG_H_

#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>
#include "Util.h"

using namespace tsgl;

class Pong {
public:
  Pong(Canvas& can);
  void bindings(Canvas& can);
  void draw(Canvas& can);
};

#endif /* PONG_H_ */
