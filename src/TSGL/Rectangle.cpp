#include "Rectangle.h"

namespace tsgl {

  Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat &color, const ColorFloat& outlineColor) : ConvexPolygon(4, color, outlineColor) {
    addVertex(x, y);
    addVertex(x+width, y);
    addVertex(x+width, y+height);
    addVertex(x, y+height);
}

}
