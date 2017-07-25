/*
 * Line.h extends Polyline and provides a class for drawing a single line to a Canvas.
 */

#ifndef LINE_H_
#define LINE_H_

#include "Polyline.h"  // For extending our Polyline object

namespace tsgl {

/*! \class Line
 *  \brief Draw a simple line.
 *  \details Line is a class for holding vertex data for a simple line.
 */
class Line : public Polyline {
 public:

    /*!
     * \brief Explicitly constructs a new Line.
     * \details This is the constructor for the Line class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param color The reference variable to the color of the Line. (Defaults to BLACK.)
     * \return A new Line with the specified endpoints and color.
     */
    Line(float x1, float y1, float x2, float y2, const ColorFloat &color = BLACK);

    /**
      * \brief Moves one end of the Line.
      * \details Moves the end of the line originally specified by (x1, y1).
      *   \param x The new x coordinate.
      *   \param y The new y coordinate.
      */
    void setFirstEnd(float x, float y);

    /**
      * \brief Moves one end of the Line.
      * \details Moves the end of the line originally specified by (x2, y2).
      *   \param x The new x coordinate.
      *   \param y The new y coordinate.
      */
    void setSecondEnd(float x, float y);

    /**
     * \brief Calculates the length of the line.
     * \details Finds the distance between the two ends of the line.
     * \return Length of the Line.
     */
    float getLength();

};

}

#endif /* LINE_H_ */
