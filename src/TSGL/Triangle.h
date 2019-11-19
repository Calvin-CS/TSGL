/*
 * Triangle.h extends Shape and provides a class for drawing a three-sided polygon to a Canvas.
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "ConvexPolygon.h"  // For extending our Shape object

namespace tsgl {

/*! \class Triangle
 *  \brief Draw a simple Triangle.
 *  \details Triangle is a class for holding vertex data for a simple triangle.
 */
class Triangle : public ConvexPolygon {
 private:
 
 public:
    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat color, bool filled = true);

    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat color[], bool filled = true);

    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor, const ColorFloat outlineColor);

    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor[], const ColorFloat outlineColor);

    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor, const ColorFloat outlineColor[]);

    Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat fillColor[], const ColorFloat outlineColor[]);
};

}

#endif /* TRIANGLE_H_ */
