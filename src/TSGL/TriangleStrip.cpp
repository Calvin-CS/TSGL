#include <TriangleStrip.h>

namespace tsgl {

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat color[], bool filled, bool outlined) : ConvexPolygon(numVertices, filled, outlined) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}
}
