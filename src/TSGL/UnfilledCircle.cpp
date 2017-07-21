#include "UnfilledCircle.h"

namespace tsgl {

UnfilledCircle::UnfilledCircle(float x, float y, float radius, ColorFloat color)
: UnfilledRegularPolygon(x,y,radius,(radius > 3) ? radius : 5,color) { }

}
