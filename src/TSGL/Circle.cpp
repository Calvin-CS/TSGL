#include "Circle.h"

namespace tsgl {

Circle::Circle(float x, float y, float radius, const ColorFloat& color, const ColorFloat& outlineColor)
: RegularPolygon(x,y,radius,(radius > 3) ? radius : 5,color,outlineColor) { } //Create a RegularPolygon with minimum of 4 sides

}
