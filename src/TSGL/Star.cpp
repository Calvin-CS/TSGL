#include "Star.h"

namespace tsgl {

Star::Star(int x, int y, int radius, int points, ColorFloat color, bool ninja, ColorFloat outlineColor) : ConcavePolygon(points*2) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta),color, outlineColor);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)),color, outlineColor);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)),color, outlineColor);
  }
}

}
