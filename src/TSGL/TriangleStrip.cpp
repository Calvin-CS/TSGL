#include <TriangleStrip.h>

namespace tsgl {

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat color, bool filled) : ConvexPolygon(numVertices, filled, !filled) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color);
    }
}

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat color[], bool filled) : ConvexPolygon(numVertices, filled, !filled) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor);
    }
}

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor);
    }
}

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor[i]);
    }
}

TriangleStrip::TriangleStrip(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) : ConvexPolygon(numVertices, true, true) { 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor[i]);
    }
}
}
