#include "Rectangle.h"

namespace tsgl {

Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color, const ColorFloat &outlineColor) : ConvexPolygon(4) {
  addVertex(x, y, color, outlineColor);
  addVertex(x+width, y, color, outlineColor);
  addVertex(x+width, y+height, color, outlineColor);
  addVertex(x, y+height, color, outlineColor);
}

}
