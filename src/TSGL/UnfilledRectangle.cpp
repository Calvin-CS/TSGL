#include "UnfilledRectangle.h"

namespace tsgl {

UnfilledRectangle::UnfilledRectangle(int x, int y, int width, int height, const ColorFloat &color) : Polyline(5) {
  addVertex(x, y, color);
  addVertex(x+width, y, color);
  addVertex(x+width, y+height, color);
  addVertex(x, y+height, color);
  addVertex(x, y, color);
}

}
