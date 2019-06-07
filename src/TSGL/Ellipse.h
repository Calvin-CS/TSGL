/*
* Ellipse.h extends ConvexPolygon and provides a class for drawing a ellipse to a Canvas.
*/

#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "ConvexPolygon.h" // For extending our Shape object

namespace tsgl {

  /*! \class Ellipse
  *  \brief Draw a simple ellipse.
  *  \details Ellipse is a class for holding vertex data for an ellipse.
  */
class Ellipse : public ConvexPolygon {
 private:

 public:
    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat color, bool filled = true);

    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat color[], bool filled = true);

    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor, const ColorFloat outlineColor);

    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor[], const ColorFloat outlineColor);

    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    Ellipse(float x, float y, float xRadius, float yRadius, const ColorFloat fillColor[], const ColorFloat outlineColor[]);
    

};

}

#endif /* ELLIPSE_H_ */