/*
 * Firework.h
 */

#ifndef ARC_H_
#define ARC_H_

#include <tsgl.h>
#include "Firework.h"

using namespace tsgl;

class Arc {
private:
  Canvas* myCan;
  Background * myBackground;
  int myLife;
  float myX, myY, myRad;
  float myAngle, myStepSize;
  ColorHSV myColor;
  Firework* f;
public:
  Arc(Canvas& can);

  Arc(Canvas* can, int x, int y, int rad, float angle);

  ~Arc();

  bool outOfBounds();

  bool onBlackPixel();

  void computeStepSize();

  void relocate();

  void step();
};

#endif /* ARC_H_ */

/* #ifdef _WIN32
  #define Arc tsgl::Arc
#endif  */