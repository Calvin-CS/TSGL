#include "Polygon.h"

namespace tsgl {

Polygon::Polygon(int numVertices, const ColorFloat& c) : Shape(numVertices, c) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
}

}
