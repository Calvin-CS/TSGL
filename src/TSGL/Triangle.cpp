#include "Triangle.h"

namespace tsgl {

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color, ColorFloat outlineColor)
  : ConvexPolygon(3, color, outlineColor) {
  addVertex(x1, y1);
  addVertex(x2, y2);
  addVertex(x3, y3);
}

}
