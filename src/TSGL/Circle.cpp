#include "Circle.h"

namespace tsgl {

Circle::Circle(float x, float y, float radius, const ColorFloat& color, bool filled, bool outlined)
: Ellipse(x,y,radius,radius,color,filled,outlined) { } //Create an Ellipse with equal x and y radii

}