/*!
* Arrow.h extends ConcavePolygon and provides a class for drawing an arrow to a Canvas.
*/

#ifndef ARROW_H_
#define ARROW_H_

#include "ConcavePolygon.h"

namespace tsgl {
  /*!
  * \class Arrow
  * \brief draw an arrow.
  * \details Arrow extends ConcavePolygon and draws an arrow from one point to another.
  */
  class Arrow : public ConcavePolygon {
  private:
    void makeArrowHead(float x, float y, float deltaX, float deltaY); // Helps constructor by calculating the Arrow Head's coordinates

  public:
    /*!
    * \brief Explicitly constructs a new Arrow.
    * \details This is the constructor for the Arrow class.
    *      \param x1 The x coordinate of the first endpoint.
    *      \param y1 The y coordinate of the first endpoint.
    *      \param x2 The x coordinate of the second endpoint.
    *      \param y2 The y coordinate of the second endpoint.
    *      \param color The reference variable to the color of the Arrow. (Defaults to BLACK.)
    *      \param doubleArrow Determines whether Arrow has two heads. (Defaults to false.)
    * \return A new Arrow with the specified endpoints and color.
    */
    Arrow(int x1, int y1, int x2, int y2, const ColorFloat &color = BLACK, bool doubleArrow = false);

  };

}

#endif /* ARROW_H_ */
