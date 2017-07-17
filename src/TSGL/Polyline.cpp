#include "Polyline.h"

namespace tsgl {

Polyline::Polyline(int numVertices, const ColorFloat& color) : Shape(numVertices, color) {
  if (numVertices < 2) {
    TsglDebug("Cannot have a line with fewer than 2 vertices."); }
}

}
