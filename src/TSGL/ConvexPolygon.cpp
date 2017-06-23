#include "ConvexPolygon.h"

namespace tsgl {

ConvexPolygon::ConvexPolygon(int numVertices) : Polygon(numVertices) { }

void ConvexPolygon::draw() {
    if (!init) {
      TsglDebug("Cannot draw yet.");
      return;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, length);
}


}
