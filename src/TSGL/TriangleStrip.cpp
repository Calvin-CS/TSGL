#include "TriangleStrip.h"

namespace tsgl {

TriangleStrip::TriangleStrip(int numVertices) : ConvexPolygon(numVertices) { }

void TriangleStrip::draw() {
    if (!init) {
      TsglDebug("Cannot draw yet.");
      return;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
}

}
