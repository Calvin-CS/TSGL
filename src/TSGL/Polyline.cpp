#include "Polyline.h"

namespace tsgl {

Polyline::Polyline(int numVertices) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    size = numberOfVertices * 6;
    current = 0;
    vertices = new float[size];
    init = false;
    geometryType = GL_LINE_STRIP;
}


void Polyline::addNextVertex(int x, int y, const ColorFloat &color) {
    if (init)  {
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

bool Polyline::isProcessed() {
    if (!init || numberOfVertices < 2) {
      TsglDebug("Cannot draw uninitialized Polyline.");
      return false;
    }
    return true;
}

}
