/*
 * Circle.h extends Shape and provides a class for drawing a circle to a Canvas.
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Shape.h"  // For extending our Shape object
#include "ConvexPolygon.h"
#include "Polyline.h"

namespace tsgl {

/*! \class Circle
 *  \brief Draw a circle.
 *  \details Circle is a class for holding Shape data for a circle.
 */
class Circle : public Shape {
 private:
    Shape * s;
 public:

    /*!
     * \brief Explicitly constructs a new Circle.
     * \details This function draws a circle with the given center, radius, resolution
     *   (number of sides), color, and fill status.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param sides The number of sides to use in the circle.
     *   \param color The color of the circle
     *     (set to BLACK by default).
     *   \param filled Whether the circle should be filled
     *     (set to true by default).
     */
    Circle(int x, int y, int radius, int sides, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draw the Line.
     * \details This function actually draws the Line to the Canvas.
     */
    void draw();

    //TODO: comment this
    float* getVerticesPointerForRenderer();

    ~Circle();
};

}

#endif /* CIRCLE_H_ */
