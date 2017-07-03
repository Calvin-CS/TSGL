#include "Polygon.h"

namespace tsgl {

Polygon::Polygon(int numVertices, const ColorFloat& c, const ColorFloat& outlineColor) : Shape(numVertices, c) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
    outline = new Polyline(numVertices+1, outlineColor);
    outlineOn = true;
}

void Polygon::setCenter(int x, int y) {
  Shape::setCenter(x, y);
  outline->setCenter(x, y);
}

void Polygon::addVertex(int x, int y) {
  Shape::addVertex(x, y);
  outline->addVertex(x, y);
}

void Polygon::drawOutline() {
  if( outlineOn )
    outline->draw();
}

void Polygon::rotate(float angle) {
  Shape::rotate(angle);
  outline->rotate(angle);
}

void Polygon::rotateAround(float angle, float x, float y) {
  Shape::rotateAround(angle, x, y);
  outline->rotateAround(angle, x, y);
}

}
