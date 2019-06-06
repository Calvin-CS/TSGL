#include "Line.h"

namespace tsgl {

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat color) : Polyline(2) {
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
}

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat color[]) : Polyline(2) {
    addVertex(x1, y1, color[0]);
    addVertex(x2, y2, color[1]);
}
}
