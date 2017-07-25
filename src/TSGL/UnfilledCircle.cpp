#include "UnfilledCircle.h"

namespace tsgl {

UnfilledCircle::UnfilledCircle(float x, float y, float radius, ColorFloat color)
: UnfilledEllipse(x,y,radius,radius,color) { }

}
