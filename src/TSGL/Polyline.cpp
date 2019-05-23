#include "Polyline.h"

namespace tsgl {

Polyline::Polyline(int numVertices) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices*6];
    init = false;
    geometryType = GL_LINE_STRIP;
}

bool Polyline::isProcessed() {
    if (numberOfVertices < 2) {
      TsglDebug("Cannot draw uninitialized Polyline.");
      return false;
    }
    return init;
}

}
