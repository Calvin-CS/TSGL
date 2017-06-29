#include "TriangleStrip.h"

namespace tsgl {

TriangleStrip::TriangleStrip(int numVertices, const ColorFloat& color, const ColorFloat& outlineColor)
  : ConvexPolygon(numVertices, color, outlineColor) { }

}
