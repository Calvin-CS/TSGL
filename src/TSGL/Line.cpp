#include "Line.h"

namespace tsgl {

Line::Line(float x1, float y1, float x2, float y2, const ColorFloat &color) : Polyline(2, color) {
  addVertex(x1, y1);
  addVertex(x2, y2);
}

void Line::setFirstEnd(float x, float y) {
  attribMutex.lock();
  vertices[0] = x;
  vertices[1] = y;
  attribMutex.unlock();
}

void Line::setSecondEnd(float x, float y) {
  attribMutex.lock();
  vertices[2] = x;
  vertices[3] = y;
  attribMutex.unlock();
}

float Line::getLength() {
  attribMutex.lock();
  float length = sqrt((vertices[0]-vertices[2])*(vertices[0]-vertices[2])+(vertices[1]-vertices[3])*(vertices[1]-vertices[3]));
  attribMutex.unlock();
  return length;
}

}
