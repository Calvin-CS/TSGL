#include "Line.h"

namespace tsgl {

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat &color) : Polyline(2, color) {
  addVertex(x1, y1);
  addVertex(x2, y2);
}

void Line::setFirstEnd(int x, int y) {
  attribMutex.lock();
  vertices[0] = x;
  vertices[1] = y;
  attribMutex.unlock();
}

void Line::setSecondEnd(int x, int y) {
  attribMutex.lock();
  vertices[2] = x;
  vertices[3] = y;
  attribMutex.unlock();
}

}
