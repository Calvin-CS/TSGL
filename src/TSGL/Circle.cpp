#include "Circle.h"

namespace tsgl {

Circle::Circle(int x, int y, int radius, int sides, const ColorFloat& color, const ColorFloat& outlineColor) : ConvexPolygon(sides,color,outlineColor) {
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i)
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta));
}

}
