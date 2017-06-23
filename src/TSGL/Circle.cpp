#include "Circle.h"

namespace tsgl {

Circle::Circle(int x, int y, int radius, int sides, ColorFloat color) : ConvexPolygon(sides) {
  myX = x, myY = y;
  myRadius = radius;
  mySides = sides;
  float delta = 2.0f / sides * PI;
  for (int i = 0; i < sides; ++i)
    addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),color);
}

void Circle::setLocation(int x, int y) {
  myX = x, myY = y;
  float delta = 2.0f / mySides * PI;
  for (int i = 0; i < mySides; ++i) {
    vertices[i*6] = myX+myRadius*cos(i*delta);
    vertices[(i*6)+1] = myY+myRadius*sin(i*delta);
  }
}

}
