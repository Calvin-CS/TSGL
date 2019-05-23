#include "Circle.h"

namespace tsgl {

Circle::Circle(float x, float y, float radius, const ColorFloat& color)
: Ellipse(x,y,radius,radius,color) { } //Create an Ellipse with equal x and y radii

}