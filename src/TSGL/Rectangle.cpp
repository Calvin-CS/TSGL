#include "Rectangle.h"

namespace tsgl {

Rectangle::Rectangle(float x, float y, float width, float height, const ColorFloat &color) {
    numberOfVertices = 4;
    geometryType = GL_TRIANGLE_STRIP;
    vertices = new float[24];
    addVertex(x, y, color);
    addVertex(x + width, y, color);
    addVertex(x, y + height, color);
    addVertex(x + width, y + height, color);
}
}
