#include "Rectangle.h"

namespace tsgl {

  Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color) : ConvexPolygon(4, color) {
    addVertex(x, y);
    addVertex(x+width, y);
    addVertex(x+width, y+height);
    addVertex(x, y+height);
}

}
