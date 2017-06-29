#include "Line.h"

namespace tsgl {

Line::Line(int x1, int y1, int x2, int y2, const ColorFloat &color) : Polyline(2, color) {
  addVertex(x1, y1);
  addVertex(x2, y2);
}

}
