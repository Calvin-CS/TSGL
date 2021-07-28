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
    Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color);

    Triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color[]);
};

}

#endif /* TRIANGLE_H_ */
