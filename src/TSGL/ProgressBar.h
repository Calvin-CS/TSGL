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
   int xx, yy, myWidth, myHeight, segs;
 public:
    /*!
     * \brief Explicit ProgressBar constructor method.
     * \details This is the explicit constructor for the ProgressBar class.
     *   \param x The x position of the left edge of the ProgressBar.
     *   \param y The y position of the top edge of the ProgressBar.
     *   \param width The maximum width in pixels of the ProgressBar.
     *   \param height The maximum height in pixels of the ProgressBar.
     *   \param minValue The minimum value represented by the ProgressBar.
     *   \param maxValue The maximum value represented by the ProgressBar.
     *   \param numSegments The number of segments in the progress bar
     * \return A new ProgressBar with the specified coordinates, maximum dimensions, value range, and segments.
     */
    ProgressBar(int x, int y, int width, int height, float minValue, float maxValue, unsigned numSegments);
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
     *   \param newValue The value to set the segment to.
     *   \param segnum The segment whose value to update. A value of -1 indicates the current thread number.
     * \note The minimum value for a segment is calculated as <code>minV + (maxV-minV)*seg/segs</code>
     * \note The maximum value for a segment is calculated as <code>minV + (maxV-minV)*(seg+1)/segs</code>
     */
    void update(float newValue, int segnum = -1);

    /*!
     * \brief Accessor for the ProgressBar's representative Polyline array.
     *   \param index Index of the segment to access.
     * \return A pointer to the Polyline array representing segment border <code>i</code>  of the ProgressBar.
     */
    Polyline* getBorder(int index);

    /*!
     * \brief Accessor for the ProgressBar's representative Rectangle array.
     *   \param index Index of the segment to access.
     * \return A pointer to the Rectangle array representing segment <code>i</code> of the ProgressBar.
     */
    Rectangle* getRect(int index);

    /*!
     * \brief Accessor for the ProgressBar's number of segments
     * \return The number of segments in the ProgressBar.
     */
    int getSegs() { return segs; }

    /*!
     * \brief Accessor for a segment's x position
     *   \param i Index of the segment
     * \return The x-coordinate of the left edge of segment <code>i</code> in the ProgressBar.
     */
    int getSegX(int i) { return startX[i]; }

    /*!
     * \brief Accessor for a segment's y position
     * \return The y-coordinate of the top edge of the ProgressBar.
     */
    int getSegY() { return yy; }
};

}

#endif /* PROGRESSBAR_H */
