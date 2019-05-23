#include "Shape.h"

namespace tsgl {

Shape::Shape() : Drawable() {
    isTextured = false;
}

void Shape::draw() {
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(geometryType, 0, numberOfVertices);
}

void Shape::addVertex(float x, float y, const ColorFloat &color = BLACK) {
    if (init) {
        TsglDebug("Cannot add anymore vertices.");
        return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    if (current == numberOfVertices*6) init = true;
}

}