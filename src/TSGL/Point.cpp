#include "Point.h"

namespace tsgl {

Point::Point(int x, int y, const ColorFloat& color) : Shape(1, color) {
  addVertex(x, y);
}

}
