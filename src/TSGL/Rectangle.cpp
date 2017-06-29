#include "Rectangle.h"

namespace tsgl {

  Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color, const ColorFloat &outlineColor) : ConvexPolygon(4, color, outlineColor) {
    addVertex(x, y);
    addVertex(x+width, y);
    addVertex(x+width, y+height);
    addVertex(x, y+height);

  testVerts[0] = x+width;
  testVerts[1] = y+height;
  testVerts[2] = x;
  testVerts[3] = y+height;
  testVerts[4] = x;
  testVerts[5] = y;
  testVerts[6] = x+width;
  testVerts[7] = y;

  testColor = color;
}

}
