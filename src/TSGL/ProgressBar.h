/*
 * ProgressBar.h extends Shape and provides a class for drawing a progress bar to a Canvas.
 *
 * Authors: Patrick Crain
 */

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <omp.h>

#include "Polyline.h"
#include "Rectangle.h"

namespace tsgl {

/*! \class ProgressBar
 *  \brief Draws and updates a progress bar.
 *  \details ProgressBar is a class for holding vertex data for multiple rectangles forming a progress bar.
 *    ProgressBar is formed of multiple segments, each of which is thread-safe and updated individually
 *    with the update() method.  A ProgressBar can be drawn to the screen using Canvas::drawProgress().
 */
class ProgressBar {
 private:
   float *startX, *endX;
   float min, max;
   int xx, yy, width, height, segs;
 public:
    /*!
     * \brief Explicit ProgressBar constructor method.
     * \details This is the explicit constructor for the ProgressBar class.
     *   \param x The x position of the left edge of the ProgressBar.
     *   \param y The y position of the top edge of the ProgressBar.
     *   \param w The maximum width in pixels of the ProgressBar.
     *   \param h The maximum height in pixels of the ProgressBar.
     *   \param minV The minimum value represented by the ProgressBar.
     *   \param maxV The maximum value represented by the ProgressBar.
     *   \param segments The number of segments in the progress bar
     * \return A new ProgressBar with the specified coordinates, maximum dimensions, value range, and segments.
     */
    ProgressBar(int x, int y, int w, int h, float minV, float maxV, unsigned segments);
    /*!
     * \brief ProgressBar destructor method.
     * \details This is the destructor for the ProgressBar class.
     * \details Frees up memory that was allocated to a ProgressBar instance.
     */
    ~ProgressBar();

    /*!
     * \brief Updates a ProgressBar segment with a new value.
     * \details This function updates the segment <code>seg</code> of the ProgressBar to represent
     *   the new value <code>newV</code>. If newV is less than the segment's minimum value, the segment
     *   is set to its minimum value. If newV is more than the segment's maximum value, the segment
     *   is set to its maximum value.
     *   \param newV The value to set the segment to.
     *   \param seg The segment whose value to update. A value of -1 indicates the current thread number.
     * \note The minimum value for a segment is calculated as <code>minV + (maxV-minV)*seg/segs</code>
     * \note The maximum value for a segment is calculated as <code>minV + (maxV-minV)*(seg+1)/segs</code>
     */
    void update(float newV, int seg = -1);

    /*!
     * \brief Accessor for the ProgressBar's number of segments
     * \return The number of segments in the ProgressBar.
     */
    int getSegs() { return segs; }

    int getSegX(int i) { return startX[i]; }
    int getSegY() { return yy; }

    /*!
     * \brief Accessor for the ProgressBar's representative Rectangle array.
     * \return A pointer to the Rectangle array representing the segments of the ProgressBar.
     */
    Rectangle* getRect(int i);

    /*!
     * \brief Accessor for the ProgressBar's representative Polyline array.
     * \return A pointer to the Polyline array representing the segment borders of the ProgressBar.
     */
    Polyline* getBorder(int i);
};

}

#endif /* PROGRESSBAR_H */
