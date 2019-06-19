/*
 * Line.h extends Shape and provides a class for drawing a single line to a Canvas.
 */

#ifndef LINE_H_
#define LINE_H_

#include "Polyline.h"  // For extending our Shape object

namespace tsgl {

/*! \class Line
 *  \brief Draw a simple line.
 *  \details Line is a class for holding vertex data for a simple line.
 */
class Line : public Polyline {
 private:
 
 public:
    Line(int x1, int y1, int x2, int y2, const ColorFloat color);

    Line(int x1, int y1, int x2, int y2, const ColorFloat color[]);

    void setFirstEnd(float x, float y);

    void setSecondEnd(float x, float y);

    float getLength();
};

}

#endif /* LINE_H_ */
