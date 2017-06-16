#include "Circle.h"

namespace tsgl {

Circle::Circle(int x, int y, int radius, int sides, ColorFloat color, bool filled) {
    // float delta = 2.0f / sides * PI;
    // if (filled) {
    //     s = new ConvexPolygon(sides);
    //     for (int i = 0; i < sides; ++i)
    //         s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),color);
    //     this->add(s);
    // } else {
    //     float oldX = 0, oldY = 0, newX = 0, newY = 0;
    //     s = new Polyline(sides+1);
    //     for (int i = 0; i <= sides; ++i) {
    //         oldX = newX; oldY = newY;
    //         newX = x+radius*cos(i*delta);
    //         newY = y+radius*sin(i*delta);
    //         if (i > 0)
    //             s->addNextVertex(oldX, oldY,color);
    //     }
    //     s->addNextVertex(newX, newY,color);
    // }
    s = new ConvexPolygon(sides);
}

void Circle::draw() {
  s->draw();
}

float* Circle::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

Circle::~Circle() {
  delete s;
}

}
