#include "Circle.h"

namespace tsgl {

Circle::Circle(int x, int y, int radius, int sides, ColorFloat color) : ConvexPolygon(sides) {
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i)
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),color);
}

}
