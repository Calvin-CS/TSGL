#include "Rectangle.h"

namespace tsgl {

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat &color, bool filled = true) : ConvexPolygon(4, filled) {
    addVertex(x, y, color);
    addVertex(x + width, y, color);
    addVertex(x + width, y + height, color);
    addVertex(x, y + height, color);
}
}
