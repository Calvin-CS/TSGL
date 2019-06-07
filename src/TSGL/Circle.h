/*
* Circle.h extends Ellipse and provides a class for drawing a circle to a Canvas.
*/

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Ellipse.h" // For extending our Ellipse object

namespace tsgl {

  /*! \class Circle
  *  \brief Draw a circle.
  *  \details Circle is a class for holding Shape data for a circle.
  */
  class Circle : public Ellipse {
  public:
    Circle(float x, float y, float radius, const ColorFloat color, bool filled = true);

    Circle(float x, float y, float radius, const ColorFloat color[], bool filled = true);

    Circle(float x, float y, float radius, const ColorFloat fillColor, const ColorFloat outlineColor);

    Circle(float x, float y, float radius, const ColorFloat fillColor[], const ColorFloat outlineColor);

    Circle(float x, float y, float radius, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    Circle(float x, float y, float radius, const ColorFloat fillColor[], const ColorFloat outlineColor[]);

  };

}

#endif /* CIRCLE_H_ */