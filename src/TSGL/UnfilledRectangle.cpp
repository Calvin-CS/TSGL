#include "UnfilledRectangle.h"

namespace tsgl {

UnfilledRectangle::UnfilledRectangle(int x, int y, int width, int height, const ColorFloat &color) : UnfilledShape(4, color) {
  addVertex(x, y);
  addVertex(x+width, y);
  addVertex(x+width, y+height);
  addVertex(x, y+height);
}

}
