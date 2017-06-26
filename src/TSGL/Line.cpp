#include "Line.h"

namespace tsgl {

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat &color) : Polyline(2) {
  addNextVertex(x1, y1, color);
  addNextVertex(x2, y2, color);
}

}
