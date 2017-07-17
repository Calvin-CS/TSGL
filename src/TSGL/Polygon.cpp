#include "Polygon.h"

namespace tsgl {

Polygon::Polygon(int numVertices, const ColorFloat& c, const ColorFloat& outlineC) : Shape(numVertices, c) {
  attribMutex.lock();
  if (numVertices < 3) {
    TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
  }
  hasOutline = true;
  outlineColor = outlineC;
  attribMutex.unlock();
}

GLfloat* Polygon::getPointerToOutlineVerticesArray() {
  attribMutex.lock();
  GLfloat* result = vertices;
  attribMutex.unlock();
  return result;
}

int Polygon::getOutlineNumberOfVertices() {
  attribMutex.lock();
  int l = length;
  attribMutex.unlock();
  return l;
}

ColorFloat* Polygon::getOutlineColor() {
  attribMutex.lock();
  ColorFloat* c = &outlineColor;
  attribMutex.unlock();
  return c;
}

void Polygon::setOutlineColor(const ColorFloat& newColor) {
  attribMutex.lock();
  outlineColor = newColor;
  attribMutex.unlock();
}

void Polygon::setHasOutline(bool outline) {
  attribMutex.lock();
  hasOutline = outline;
  attribMutex.unlock();
}

}
