#include "Circle.h"

namespace tsgl {

Circle::Circle(float x, float y, float radius, const ColorFloat& color, const ColorFloat& outlineColor)
: Ellipse(x,y,radius,radius,color,outlineColor) { } //Create an Ellipse with equal x and y radii

}
