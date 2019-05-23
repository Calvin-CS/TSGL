#include "Line.h"

namespace tsgl {

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat &color) {
    vertices = new float[12];
    numberOfVertices = 2;
    geometryType = GL_LINES;
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
}
}
