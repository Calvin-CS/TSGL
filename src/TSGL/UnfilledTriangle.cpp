#include "UnfilledTriangle.h"

namespace tsgl {

UnfilledTriangle::UnfilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color) : Polyline(3, color) {
  addVertex(x1, y1);
  addVertex(x2, y2);
  addVertex(x3, y3);
}

}
