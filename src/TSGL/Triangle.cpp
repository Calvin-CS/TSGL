#include "Triangle.h"

namespace tsgl {

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color) : ConvexPolygon(3) {
    vertices = new float[18];
    numberOfVertices = 3;
    geometryType = GL_TRIANGLE_STRIP;
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
    addVertex(x3, y3, color);
}
}
