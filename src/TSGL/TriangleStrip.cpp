#include <TriangleStrip.h>

namespace tsgl {

TriangleStrip::TriangleStrip(int numVertices) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
    numberOfVertices = numVertices;
    vertices = new float[numberOfVertices * 6];
    geometryType = GL_TRIANGLE_STRIP;
}
}
