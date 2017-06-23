#include "HollowCircle.h"

namespace tsgl {

HollowCircle::HollowCircle(int x, int y, int radius, int sides, ColorFloat color) : Polyline(sides+1) {
  myX = x, myY = y;
  myRadius = radius;
  mySides = sides;
  float delta = 2.0f / sides * PI;
  float oldX = 0, oldY = 0, newX = 0, newY = 0;
  for (int i = 0; i <= sides; ++i) {
      oldX = newX; oldY = newY;
      newX = x+radius*cos(i*delta);
      newY = y+radius*sin(i*delta);
      if (i > 0)
          addNextVertex(oldX, oldY,color);
  }
  addNextVertex(newX, newY,color);
}

}