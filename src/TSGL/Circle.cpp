#include "Circle.h"

namespace tsgl {

Circle::Circle(float x, float y, float radius, const ColorFloat color, bool filled)
: Ellipse(x,y,radius,radius,color,filled) { } //Create an Ellipse with equal x and y radii

Circle::Circle(float x, float y, float radius, const ColorFloat color[], bool filled)
: Ellipse(x,y,radius,radius,color,filled) { } //Create an Ellipse with equal x and y radii

Circle::Circle(float x, float y, float radius, const ColorFloat fillColor, const ColorFloat outlineColor)
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

Circle::Circle(float x, float y, float radius, const ColorFloat fillColor[], const ColorFloat outlineColor)
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

Circle::Circle(float x, float y, float radius, const ColorFloat fillColor, const ColorFloat outlineColor[])
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

Circle::Circle(float x, float y, float radius, const ColorFloat fillColor[], const ColorFloat outlineColor[])
: Ellipse(x,y,radius,radius,fillColor,outlineColor) { } //Create an Ellipse with equal x and y radii

}