/*
 * Dot.h
 */

#ifndef DOT_H_
#define DOT_H_

#include <tsgl.h>

using namespace tsgl;

class Dot {
private:
  bool dead;
  Canvas* myCan;
  Background * myBackground;
  float myX, myY, mySpeed, myDir, myFric;
public:
  Dot(Canvas& can, float x, float y, float s, float d, float f);

  void step();

  bool isDead();
};

#endif /* DOT_H_ */