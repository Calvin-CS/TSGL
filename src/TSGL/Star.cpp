#include "Star.h"

namespace tsgl {

Star::Star(int x, int y, int radius, int points, ColorFloat color, bool ninja) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myX = x, myY = y;
  myRadius = radius;
  myPoints = points;
  myColor = color;
  float delta = 2.0f / points * PI;
  s = new ConcavePolygon(points*2);
  for(int i = 0; i < points; ++i) {
    s->addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta),color);
    if( ninja )
      s->addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)),color);
    else
      s->addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)),color);
  }
}

void Star::draw() {
  s->draw();
}

float* Star::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

}
