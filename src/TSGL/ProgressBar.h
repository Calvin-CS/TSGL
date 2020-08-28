/*
 * ProgressBar.h extends Shape and provides a class for drawing a progress bar to a Canvas.
 */

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <omp.h>

#include "Polyline.h"
#include "Rectangle.h"
#include "Drawable.h"

namespace tsgl {

/*! \class ProgressBar
 *  \brief Draws and updates a progress bar.
 *  \details ProgressBar is a class for holding vertex data for multiple rectangles forming a progress bar.
 *    ProgressBar is formed of multiple segments, each of which is thread-safe and updated individually
 *    with the update() method.  A ProgressBar can be drawn to the screen using Canvas::drawProgress().
 */
class ProgressBar : public Drawable {
 private:
    Rectangle ** segRecs;
    Polyline ** segBorders;
    float *startX;
    float *endX;
    float min, max;
    float myWidth, myHeight;
    int segs;
 public:

    ProgressBar(float x, float y, float z, float width, float height, float minValue, float maxValue, unsigned numSegments, float yaw, float pitch, float roll);

    ~ProgressBar();

    void draw(Shader * shader);

    void update(float newValue, int segnum = -1);

    /*!
     * \brief Accessor for the ProgressBar's number of segments
     * \return The number of segments in the ProgressBar.
     */
    int getSegs() { return segs; }

    /*!
     * \brief Accessor for the ProgressBar's width in pixels
     * \return The pixel width of the ProgressBar.
     */
    float getWidth() { return myWidth; }

    /*!
     * \brief Accessor for the ProgressBar's height in pixels
     * \return The pixel height of the ProgressBar.
     */
    float getHeight() { return myHeight; }
};

}

#endif /* PROGRESSBAR_H */
