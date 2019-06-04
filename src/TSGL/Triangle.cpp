#include "Triangle.h"

namespace tsgl {

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat color, bool filled, bool outlined) : ConvexPolygon(3, filled, outlined) {
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
    addVertex(x3, y3, color);
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat color[], bool filled, bool outlined) : ConvexPolygon(3, filled, outlined) {
    addVertex(x1, y1, color[0]);
    addVertex(x2, y2, color[1]);
    addVertex(x3, y3, color[2]);
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor, const ColorFloat outlineColor, bool filled, bool outlined) : ConvexPolygon(3, filled, outlined) {
    addVertex(x1, y1, fillColor, outlineColor);
    addVertex(x2, y2, fillColor, outlineColor);
    addVertex(x3, y3, fillColor, outlineColor);
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor[], const ColorFloat outlineColor, bool filled, bool outlined) : ConvexPolygon(3, filled, outlined) {
    addVertex(x1, y1, fillColor[0], outlineColor);
    addVertex(x2, y2, fillColor[1], outlineColor);
    addVertex(x3, y3, fillColor[2], outlineColor);
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor, const ColorFloat outlineColor[], bool filled, bool outlined) : ConvexPolygon(3, filled, outlined) {
    addVertex(x1, y1, fillColor, outlineColor[0]);
    addVertex(x2, y2, fillColor, outlineColor[1]);
    addVertex(x3, y3, fillColor, outlineColor[2]);
}

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor[], const ColorFloat outlineColor[], bool filled, bool outlined) : ConvexPolygon(3, filled, outlined) {
    addVertex(x1, y1, fillColor[0], outlineColor[0]);
    addVertex(x2, y2, fillColor[1], outlineColor[1]);
    addVertex(x3, y3, fillColor[2], outlineColor[2]);
}
}
