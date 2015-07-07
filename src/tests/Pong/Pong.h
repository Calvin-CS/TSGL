/*
 * Pong.h
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#ifndef PONG_H_
#define PONG_H_

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

using namespace tsgl;

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

class Pong {
private:
  float myLeftY, myRightY, myBallX, myBallY;
  float myXX, myYY, mySpeed, myDir;
  int myLeftDir, myRightDir, myLeftPoints, myRightPoints;
  float randfloat(int divisor = 10000);
public:
  Pong(Canvas& can);
  void bindings(Canvas& can);
  void draw(Canvas& can);
};

#endif /* PONG_H_ */
