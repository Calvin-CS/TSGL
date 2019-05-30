#include "Circle.h"

namespace tsgl {

Circle::Circle(float x, float y, float radius, const ColorFloat& color, bool filled = true)
: Ellipse(x,y,radius,radius,color,filled) { } //Create an Ellipse with equal x and y radii

}