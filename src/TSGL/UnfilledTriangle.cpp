#include "UnfilledTriangle.h"

namespace tsgl {

UnfilledTriangle::UnfilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color) : Polyline(3) {
  addNextVertex(x1, y1, color);
  addNextVertex(x2, y2, color);
  addNextVertex(x3, y3, color);
}

}
