#include "Rectangle.h"

namespace tsgl {

Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color) : ConvexPolygon(4) {
  addVertex(x, y, color);
  addVertex(x+width, y, color);
  addVertex(x+width, y+height, color);
  addVertex(x, y+height, color);
}

}
