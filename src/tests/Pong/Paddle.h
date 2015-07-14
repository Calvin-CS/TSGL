/*
 * Paddle.h
 *
 *  Created on: Jul 14, 2015
 *      Author: cpd5
 */

#ifndef PADDLE_H_
#define PADDLE_H_

#include <tsgl.h>
#include "Ball.h"

class Paddle {
public:
  Paddle(Canvas& can);
  virtual ~Paddle();
  void bindings(Canvas& can, int side);
  void checkBoundColl(Ball& b, Canvas& can, int side);
  void checkPadColl(Ball& b, Canvas& can, int side);
  void draw(Canvas& can, int side);
  void increment();
  void move();
  int getPoints() const;
  void setDir(int direction);
private:
  int myDir;  //-1 = up, 1 = down, 0 = stationary
  int myPoints;
  float myY;
};

#endif /* PADDLE_H_ */
