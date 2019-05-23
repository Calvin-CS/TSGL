#include "Ellipse.h"

namespace tsgl {

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat& color = BLACK, const ColorFloat& outlineColor = BLACK)
: ConvexPolygon((0+xRadius+yRadius)/2,color,outlineColor) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / getNumberOfVertices() * PI;
  for (int i = 0; i < getNumberOfVertices(); ++i)
    addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta));
}

}