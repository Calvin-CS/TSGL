#include "UnfilledTriangle.h"

namespace tsgl {

UnfilledTriangle::UnfilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const ColorFloat &color) : UnfilledShape(3, color) {
  addVertex(x1, y1);
  addVertex(x2, y2);
  addVertex(x3, y3);
}

}
