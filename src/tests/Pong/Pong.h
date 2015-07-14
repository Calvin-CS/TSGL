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
#include "Ball.h"
#include "Paddle.h"
#include "Util.h"

using namespace tsgl;

class Pong {
public:
  Pong(Canvas& can);
  void draw(Canvas& can);
private:
  Paddle * leftPaddle;
  Paddle * rightPaddle;
  Ball * pongBall;
};

#endif /* PONG_H_ */
