#include "Ellipse.h"

namespace tsgl {

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat color, bool filled, bool outlined) : ConvexPolygon((xRadius + yRadius) / 2, filled, outlined) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), color);
  }
}

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat color[], bool filled, bool outlined) : ConvexPolygon((xRadius + yRadius) / 2, filled, outlined) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), color[i]);
  }
}

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor, const ColorFloat outlineColor, bool filled, bool outlined) : ConvexPolygon((xRadius + yRadius) / 2, filled, outlined) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor, outlineColor);
  }
}

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor[], const ColorFloat outlineColor, bool filled, bool outlined) : ConvexPolygon((xRadius + yRadius) / 2, filled, outlined) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor[i], outlineColor);
  }
}

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor, const ColorFloat outlineColor[], bool filled, bool outlined) : ConvexPolygon((xRadius + yRadius) / 2, filled, outlined) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor, outlineColor[i]);
  }
}

Ellipse::Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor[], const ColorFloat outlineColor[], bool filled, bool outlined) : ConvexPolygon((xRadius + yRadius) / 2, filled, outlined) {
  //TODO: do we need any locking here? All the values we use below are from the constructor
  float delta = 2.0f / numberOfVertices * PI;
  for (int i = 0; i < numberOfVertices; ++i) {
      addVertex(x+xRadius*cos(i*delta), y+yRadius*sin(i*delta), fillColor[i], outlineColor[i]);
  }
}


}