/*
 * Arrow.h extends Shape and provides a class for drawing a single line to a Canvas.
 */

#ifndef ARROW_H_
#define ARROW_H_

#include "Polyline.h"  // For extending our Shape object

namespace tsgl {

/*! \class Arrow
 *  \brief Draw a simple Arrow.
 *  \details Line is a class for holding vertex data for a simple Arrow.
 */
class Arrow : public Polyline {
 private:
 
 public:

    /*!
     * \brief Explicitly constructs a new Line.
     * \details This is the constructor for the Line class.
     *      \param x1 The x coordinate of the first endpoint.
     *      \param y1 The y coordinate of the first endpoint.
     *      \param x2 The x coordinate of the second endpoint.
     *      \param y2 The y coordinate of the second endpoint.
     *      \param color The reference variable to the color of the Line.
     * \return A new Line with the specified endpoints and color.
     */
    Arrow(int x1, int y1, int x2, int y2, const ColorFloat &color);
};

}

#endif /* LINE_H_ */