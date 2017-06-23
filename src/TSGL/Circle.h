/*
 * Circle.h extends ConvexPolygon and provides a class for drawing a circle to a Canvas.
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Shape.h"  // For extending our ConvexPolygon object
#include "ConvexPolygon.h"
#include "Polyline.h"

namespace tsgl {

/*! \class Circle
 *  \brief Draw a circle.
 *  \details Circle is a class for holding ConvexPolygon data for a circle.
 */
class Circle : public ConvexPolygon {
 private:
    int myX, myY, myRadius, mySides;
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
    Circle(int x, int y, int radius, int sides, ColorFloat color = BLACK);

    //TODO: change or comment or something
    int getX() { return myX; }
    int getY() { return myY; }

    /**
     * \brief Moves Circle to different coordinates
     * \details Sets the Circle's center to different x and y
     *  \param x The x coordinate of the circle's center.
     *  \param y The y coordinate of the circle's center.
     */
    void setLocation(int x, int y);

    // /**
    //  * \brief Changes the Circle's color
    //  * \details Sets the Circle's color to a new color.
    //  *  \param color The new ColorFloat of the circle.
    //  */
    // void setColor(ColorFloat color);
};

}

#endif /* CIRCLE_H_ */
