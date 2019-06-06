#include "Square.h"

namespace tsgl {

Square::Square(float x, float y, float sideLength, const ColorFloat color, bool filled)
: Rectangle(x,y,sideLength,sideLength,color,filled) { } //Create an Rectangle with equal width and height

Square::Square(float x, float y, float sideLength, const ColorFloat color[], bool filled)
: Rectangle(x,y,sideLength,sideLength,color,filled) { } //Create an Rectangle with equal width and height

Square::Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor)
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

Square::Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor)
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

Square::Square(float x, float y, float sideLength, const ColorFloat fillColor, const ColorFloat outlineColor[])
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

Square::Square(float x, float y, float sideLength, const ColorFloat fillColor[], const ColorFloat outlineColor[])
: Rectangle(x,y,sideLength,sideLength,fillColor,outlineColor) { } //Create an Rectangle with equal width and height

}