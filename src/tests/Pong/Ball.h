/*
 * Ball.h
 *
 *  Created on: Jul 14, 2015
 *      Author: cpd5
 */

#ifndef BALL_H_
#define BALL_H_

#include <tsgl.h>

class Ball {
 public:
  Ball(Canvas& can);
  void draw(Canvas& can);
  float getX() const;
  float getY() const;
  void move();
  void reset();
  virtual ~Ball();
  float randfloat(int divisor = 10000);
private:
  float myX, myY, myXX, myYY, mySpeed, myDir;


};

#endif /* BALL_H_ */
