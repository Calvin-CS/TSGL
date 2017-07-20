#include "Point.h"

namespace tsgl {

Point::Point(float x, float y, const ColorFloat& color) : Shape(1, color) {
  addVertex(x, y);
}

}
