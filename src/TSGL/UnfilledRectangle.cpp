#include "UnfilledRectangle.h"

namespace tsgl {

UnfilledRectangle::UnfilledRectangle(int x, int y, int width, int height, const ColorFloat &color) : Polyline(5) {
  addNextVertex(x, y, color);
  addNextVertex(x+width, y, color);
  addNextVertex(x+width, y+height, color);
  addNextVertex(x, y+height, color);
  addNextVertex(x, y, color);
}

}
