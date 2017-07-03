#include "Arrow.h"

namespace tsgl {

  Arrow::Arrow(int x1, int y1, int x2, int y2, const ColorFloat &color, bool doubleArrow)
  : ConcavePolygon( (doubleArrow)? 11 : 8, color) {
    isDoubleArrow = doubleArrow;
    addVertex(x1, y1-1);
    makeArrowHead(x2, y2, x2-x1, y2-y1);

    if( isDoubleArrow ) {
      makeArrowHead(x1, y1, x1-x2, y1-y2);
    }
    addVertex(x1, y1+1);

  }

  /*!
  * \brief private method helping constructor for the arrow heads
  */
  void Arrow::makeArrowHead(float x, float y, float deltaX, float deltaY) {
    double angle = atan( deltaY / deltaX );
    double s = sin( angle ), c = cos( angle );
    int x1 = -10*c-(-5)*s, x2 = 2*c - 0*s, x3 = -10*c-5*s;
    int y1 = -10*s+(-5)*c, y2 = 2*s + 0*c, y3 = -10*s+5*c;
    // if we have an arrow pointing left, rotate it pi radians ( sin = 0, cos = -1)
    if (deltaX < 0) {
      x1 = -x1, x2 = -x2, x3 = -x3;
      y1 = -y1, y2 = -y2, y3 = -y3;
    }
    // transpose the triangle to the end of the line
    x1 += x, x2 += x, x3 += x;
    y1 += y, y2 += y, y3 += y;

    addVertex( (x1+x3)/2, (y1+y3)/2-1 );
    addVertex(x1, y1);
    addVertex(x2, y2);
    addVertex(x3, y3);
    addVertex( (x1+x3)/2, (y1+y3)/2+1 );
  }

}
