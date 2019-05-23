#include <TriangleStrip.h>

namespace tsgl {

TriangleStrip::TriangleStrip(int numVertices) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
    numberOfVertices = numVertices;
    size = numberOfVertices * 6;
    current = 0;
    vertices = new float[size];
    init = false;
    geometryType = GL_TRIANGLE_STRIP;
}


void TriangleStrip::addVertex(int x, int y, const ColorFloat &color) {
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
    if (current == size) init = true;
}

bool TriangleStrip::isProcessed() {
  return init;
}

}
