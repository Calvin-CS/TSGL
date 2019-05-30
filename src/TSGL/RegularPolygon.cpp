#include "RegularPolygon.h"

namespace tsgl {

RegularPolygon::RegularPolygon(float x, float y, float radius, int sides, const ColorFloat& color, bool filled = true) : ConvexPolygon(sides, filled) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i) {
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta), color);
  }
}

} 