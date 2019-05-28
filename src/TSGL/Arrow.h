/*
 * Arrow.h extends Shape and provides a class for drawing a single line to a Canvas.
 */

#ifndef ARROW_H_
#define ARROW_H_

#include "ConcavePolygon.h"  // For extending our Shape object

namespace tsgl {

/*! \class Arrow
 *  \brief Draw a simple Arrow.
 *  \details Line is a class for holding vertex data for a simple Arrow.
 */
class Arrow : public ConcavePolygon {
 private:
    int headX, headY, tailX, tailY;
    bool isDoubleArrow;
    void makeArrowHead(float x, float y, float deltaX, float deltaY, const ColorFloat& color); // Helps constructor by calculating the Arrow Head's coordinates
    void generateVertices(const ColorFloat& color);
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
    Arrow(int x1, int y1, int x2, int y2, const ColorFloat &color, bool doubleArrow = false);

    // /**
    //  * \brief Moves the head of the Arrow.
    //  * \details Changes the coordinates of the first point in the Arrow to a new x and y.
    //  *    \param x The new x coordinate of the head.
    //  *    \param y The new y coordinate of the head.
    //  */
    // void moveHead(float x, float y);

    // /**
    //  * \brief Moves the tail of the Arrow.
    //  * \details Changes the coordinates of the second point in the Arrow to a new x and y.
    //  *    \param x The new x coordinate of the tail.
    //  *    \param y The new y coordinate of the tail.
    //  */
    // void moveTail(float x, float y); //TODO: comment
};

}

#endif /* LINE_H_ */