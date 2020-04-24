// /*
//  * ProgressBar.h extends Shape and provides a class for drawing a progress bar to a Canvas.
//  */

// #ifndef PROGRESSBAR_H
// #define PROGRESSBAR_H

// #include <omp.h>

// #include "Polyline.h"
// #include "Rectangle.h"

// namespace tsgl {

// /*! \class ProgressBar
//  *  \brief Draws and updates a progress bar.
//  *  \details ProgressBar is a class for holding vertex data for multiple rectangles forming a progress bar.
//  *    ProgressBar is formed of multiple segments, each of which is thread-safe and updated individually
//  *    with the update() method.  A ProgressBar can be drawn to the screen using Canvas::drawProgress().
//  */
// class ProgressBar {
//  private:
//    float *startX, *endX;
//    float min, max;
//    int xx, yy, myWidth, myHeight, segs;
//  public:

//     ProgressBar(int x, int y, int width, int height, float minValue, float maxValue, unsigned numSegments);

//     ~ProgressBar();

//     void update(float newValue, int segnum = -1);

//     Polyline* getBorder(int index);

//     Rectangle* getRect(int index);

//     /*!
//      * \brief Accessor for the ProgressBar's number of segments
//      * \return The number of segments in the ProgressBar.
//      */
//     int getSegs() { return segs; }

//     /*!
//      * \brief Accessor for a segment's x position
//      *   \param i Index of the segment
//      * \return The x-coordinate of the left edge of segment <code>i</code> in the ProgressBar.
//      */
//     int getSegX(int i) { return startX[i]; }

//     /*!
//      * \brief Accessor for a segment's y position
//      * \return The y-coordinate of the top edge of the ProgressBar.
//      */
//     int getSegY() { return yy; }
// };

// }

// #endif /* PROGRESSBAR_H */
