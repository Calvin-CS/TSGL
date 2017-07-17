#include "Arrow.h"

namespace tsgl {

  Arrow::Arrow(int x1, int y1, int x2, int y2, const ColorFloat &color, bool doubleArrow) : ConcavePolygon( (doubleArrow)? 10 : 7, color) {
    attribMutex.lock();
    hasOutline = false;
    headX = x2; headY = y2;
    tailX = x1; tailY = y1;
    isDoubleArrow = doubleArrow;
    attribMutex.unlock();
    generateVertices();
  }

  void Arrow::generateVertices() {
    //TODO: figure out locking for this since we are adding vertices, which uses the lock
    makeArrowHead(headX, headY, headX-tailX, headY-tailY);

    if( isDoubleArrow ) {
      makeArrowHead(tailX, tailY, tailX-headX, tailY-headY);
    } else {
      int a, b; //Offsets for vertices
      if( tailY < headY ) a = 1;
      else a = -1;
      if( tailX > headX ) b = 1;
      else b = -1;

      addVertex(tailX-a, tailY-b);
      addVertex(tailX+a, tailY+b);
    }
  }

  void Arrow::moveHead(int x, int y) { //TODO: test
    attribMutex.lock();
    current = 0; init = false;
    headX = x; headY = y;
    attribMutex.unlock();
    generateVertices();
  }

  void Arrow::moveTail(int x, int y) { //TODO: test
    attribMutex.lock();
    current = 0; init = false;
    tailX = x; tailY = y;
    attribMutex.unlock();
    generateVertices();
  }

  /*!
  * \brief private method helping constructor for the arrow heads
  */
  void Arrow::makeArrowHead(float x, float y, float deltaX, float deltaY) {

    int a, b;
    if( deltaY > 0 ) a = 1;
    else a = -1;
    if( deltaX < 0 ) b = 1;
    else b = -1;

    float angle = atan( deltaY / deltaX );
    float s = sin( angle ), c = cos( angle );
    float x1 = -10*c-(-5)*s, x2 = 2*c - 0*s, x3 = -10*c-5*s;
    float y1 = -10*s+(-5)*c, y2 = 2*s + 0*c, y3 = -10*s+5*c;
    // if we have an arrow pointing left, rotate it pi radians ( sin = 0, cos = -1)
    if (deltaX < 0) {
      x1 = -x1, x2 = -x2, x3 = -x3;
      y1 = -y1, y2 = -y2, y3 = -y3;
    }
    // transpose the triangle to the end of the line
    x1 += x, x2 += x, x3 += x;
    y1 += y, y2 += y, y3 += y;

    addVertex( (x1+x3)/2+a, (y1+y3)/2+b );
    addVertex(x1, y1);
    addVertex(x2, y2);
    addVertex(x3, y3);
    addVertex( (x1+x3)/2-a, (y1+y3)/2-b );
  }

}
