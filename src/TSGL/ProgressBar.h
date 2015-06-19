/*
 * ProgressBar.h extends Shape and provides a class for drawing a progress bar to a Canvas.
 *
 * Authors: Patrick Crain
 */

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "Polyline.h"
#include "Rectangle.h"

namespace tsgl {

/*! \class ProgressBar
 *  \brief Draw and update a progress bar.
 *  \details ProgressBar is a class for holding vertex data for a multiple rectangles forming a progress bar.
 */
class ProgressBar {
 private:
   float *startX, *endX;
   float min, max;
   int xx, yy, width, height, segs;
 public:
    ProgressBar(int x, int y, int w, int h, float minV, float maxV, unsigned segments);
    ~ProgressBar();
    void update( unsigned segment, float newV);

    int getSegs() { return segs; }
    Rectangle* getRect(int i);
    Polyline* getBorder(int i);
};

}

#endif /* PROGRESSBAR_H */
