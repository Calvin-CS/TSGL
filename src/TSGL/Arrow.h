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
    ColorFloat headColor;
    ColorFloat tailColor;
    void makeArrowHead(float x, float y, float deltaX, float deltaY, const ColorFloat color); // Helps constructor by calculating the Arrow Head's coordinates
    void generateVertices();
 public:

    Arrow(int x1, int y1, int x2, int y2, const ColorFloat color, bool doubleArrow = false);

    Arrow(int x1, int y1, int x2, int y2, const ColorFloat color[], bool doubleArrow = false);

    void moveHead(float x, float y);

    void moveTail(float x, float y);
};

}

#endif /* LINE_H_ */