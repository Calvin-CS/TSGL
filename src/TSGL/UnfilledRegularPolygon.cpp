#include "UnfilledRegularPolygon.h"

namespace tsgl {

UnfilledRegularPolygon::UnfilledRegularPolygon(float x, float y, float radius, int sides, ColorFloat color) : UnfilledShape(sides, color) {
  attribMutex.lock();
  myX = x, myY = y;
  myRadius = radius;
  mySides = sides;
  attribMutex.unlock();
  float delta = 2.0f / sides * PI;
  float oldX = 0, oldY = 0, newX = 0, newY = 0;
  for (int i = 0; i <= sides; ++i) {
      oldX = newX; oldY = newY;
      newX = x+radius*cos(i*delta);
      newY = y+radius*sin(i*delta);
      if (i > 0)
          addVertex(oldX, oldY);
  }
}

}
