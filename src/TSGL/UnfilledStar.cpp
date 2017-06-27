#include "UnfilledStar.h"

namespace tsgl {

UnfilledStar::UnfilledStar(int x, int y, int radius, int points, ColorFloat color, bool ninja) : Polyline(points*2+1) {
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta),color);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)),color);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)),color);
  }
  addVertex(x+(radius/2), y, color);
}

}
