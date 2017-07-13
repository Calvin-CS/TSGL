#include "UnfilledStar.h"

namespace tsgl {

UnfilledStar::UnfilledStar(int x, int y, int radius, int points, ColorFloat color, bool ninja) : UnfilledShape(points*2, color) {
  attribMutex.lock();
  myRadius = radius;
  myPoints = points;
  attribMutex.unlock();
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta));
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)));
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)));
  }
}

}
