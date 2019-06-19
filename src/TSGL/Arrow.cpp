#include "Arrow.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new monocolored Line.
  * \details This is the constructor for the Line class.
  *      \param x1 The x coordinate of the first endpoint.
  *      \param y1 The y coordinate of the first endpoint.
  *      \param x2 The x coordinate of the second endpoint.
  *      \param y2 The y coordinate of the second endpoint.
  *      \param color The reference variable to the color of the Line.
  * \return A new Line with the specified endpoints and color.
  */
  Arrow::Arrow(int x1, int y1, int x2, int y2, const ColorFloat color, bool doubleArrow) : ConcavePolygon((doubleArrow)? 10 : 7) {
    headX = x2; headY = y2;
    tailX = x1; tailY = y1;
    isDoubleArrow = doubleArrow;
    headColor = color;
    tailColor = color;
    generateVertices();
  }

 /*!
  * \brief Explicitly constructs a new multicolored Line.
  * \details This is the constructor for the Line class.
  *      \param x1 The x coordinate of the first endpoint.
  *      \param y1 The y coordinate of the first endpoint.
  *      \param x2 The x coordinate of the second endpoint.
  *      \param y2 The y coordinate of the second endpoint.
  *      \param color An array of colors for the line endpoints.
  * \return A new Line with the specified endpoints and endpoint colors.
  */
  Arrow::Arrow(int x1, int y1, int x2, int y2, const ColorFloat color[], bool doubleArrow) : ConcavePolygon((doubleArrow)? 10 : 7) {
    headX = x2; headY = y2;
    tailX = x1; tailY = y1;
    isDoubleArrow = doubleArrow;
    headColor = color[0];
    tailColor = color[1];
    generateVertices();
  }

 /*!
  * \brief private method helping constructor initialize vertices array for monocolored arrow
  */
  void Arrow::generateVertices() {
    //TODO: figure out locking for this since we are adding vertices, which uses the lock
    makeArrowHead(headX, headY, headX-tailX, headY-tailY, headColor);

    if( isDoubleArrow ) {
      makeArrowHead(tailX, tailY, tailX-headX, tailY-headY, tailColor);
    } else {
      int a, b; //Offsets for vertices
      if( tailY < headY ) a = 1;
      else a = -1;
      if( tailX > headX ) b = 1;
      else b = -1;

      addVertex(tailX-a, tailY-b, tailColor);
      addVertex(tailX+a, tailY+b, tailColor);
    }
  }

  /**
   * \brief Moves the head of the Arrow.
   * \details Changes the coordinates of the first point in the Arrow to a new x and y.
   *    \param x The new x coordinate of the head.
   *    \param y The new y coordinate of the head.
   */
  void Arrow::moveHead(float x, float y) {
    // attribMutex.lock();
    current = 0; init = false;
    headX = x; headY = y;
    // attribMutex.unlock();
    generateVertices();
  }

  /**
   * \brief Moves the tail of the Arrow.
   * \details Changes the coordinates of the second point in the Arrow to a new x and y.
   *    \param x The new x coordinate of the tail.
   *    \param y The new y coordinate of the tail.
   */
  void Arrow::moveTail(float x, float y) {
    // attribMutex.lock();
    current = 0; init = false;
    tailX = x; tailY = y;
    // attribMutex.unlock();
    generateVertices();
  }

 /*!
  * \brief private method helping constructor for the arrow heads
  */
  void Arrow::makeArrowHead(float x, float y, float deltaX, float deltaY, const ColorFloat color) {

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

    addVertex( (x1+x3)/2+a, (y1+y3)/2+b, color );
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
    addVertex(x3, y3, color);
    addVertex( (x1+x3)/2-a, (y1+y3)/2-b, color );
  }
}