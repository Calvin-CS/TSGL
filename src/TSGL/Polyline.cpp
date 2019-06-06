#include "Polyline.h"

namespace tsgl {

Polyline::Polyline(int numVertices) : Shape() {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
}

Polyline::Polyline(int numVertices, int x[], int y[], ColorFloat color) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color);
    }
}

Polyline::Polyline(int numVertices, int x[], int y[], ColorFloat color[]) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}

bool Polyline::isProcessed() {
    if (numberOfVertices < 2) {
      TsglDebug("Cannot draw uninitialized Polyline.");
      return false;
    }
    return init;
}

}
