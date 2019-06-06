#include "Rectangle.h"

namespace tsgl {

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat color, bool filled) : ConvexPolygon(4, filled, !filled) {
    addVertex(x, y, color);
    addVertex(x + width, y, color);
    addVertex(x + width, y + height, color);
    addVertex(x, y + height, color);
}

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat color[], bool filled) : ConvexPolygon(4, filled, !filled) {
    addVertex(x, y, color[0]);
    addVertex(x + width, y, color[1]);
    addVertex(x + width, y + height, color[2]);
    addVertex(x, y + height, color[3]);
}

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor, const ColorFloat outlineColor) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor, outlineColor);
    addVertex(x + width, y, fillColor, outlineColor);
    addVertex(x + width, y + height, fillColor, outlineColor);
    addVertex(x, y + height, fillColor, outlineColor);
}

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor[], const ColorFloat outlineColor) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor[0], outlineColor);
    addVertex(x + width, y, fillColor[1], outlineColor);
    addVertex(x + width, y + height, fillColor[2], outlineColor);
    addVertex(x, y + height, fillColor[3], outlineColor);
}

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor, const ColorFloat outlineColor[]) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor, outlineColor[0]);
    addVertex(x + width, y, fillColor, outlineColor[1]);
    addVertex(x + width, y + height, fillColor, outlineColor[2]);
    addVertex(x, y + height, fillColor, outlineColor[3]);
}

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat fillColor[], const ColorFloat outlineColor[]) : ConvexPolygon(4, true, true) {
    addVertex(x, y, fillColor[0], outlineColor[0]);
    addVertex(x + width, y, fillColor[1], outlineColor[1]);
    addVertex(x + width, y + height, fillColor[2], outlineColor[2]);
    addVertex(x, y + height, fillColor[3], outlineColor[3]);
}

}
