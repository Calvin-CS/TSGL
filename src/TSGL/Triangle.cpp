#include "Triangle.h"

namespace tsgl {

Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, const ColorFloat &color, const ColorFloat& outlineColor) : ConvexPolygon(3, color, outlineColor) {
  addVertex(x1, y1);
  addVertex(x2, y2);
  addVertex(x3, y3);
}

}
