#include "Ellipse.h"

namespace tsgl {

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat& color = BLACK) : ConvexPolygon((xRadius + yRadius) / 2) {
  numberOfVertices = (xRadius + yRadius) / 2;
  geometryType = GL_TRIANGLE_FAN;
  vertices = new float[numberOfVertices * 6];
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), color);
  }
}

}