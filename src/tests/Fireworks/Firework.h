/*
 * Firework.h
 */

#ifndef FIREWORK_H_
#define FIREWORK_H_

#include <tsgl.h>
#include "Dot.h"

using namespace tsgl;

class Firework {
private:
  bool dead;
  int myX, myY;
  Canvas* myCan;
  Dot* myDots[10];
public:
  Firework(Canvas& can, int x, int y);

  ~Firework();

  void step();

  bool isDead();
};

#endif /* FIREWORK_H_ */