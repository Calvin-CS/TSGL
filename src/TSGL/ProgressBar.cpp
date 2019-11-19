#include "ProgressBar.h"

namespace tsgl {

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
ProgressBar::ProgressBar(int x, int y, int width, int height, float minValue, float maxValue, unsigned numSegments) {
    startX = new float[numSegments];
    endX = new float[numSegments];
    min = minValue; max = maxValue;
    xx = x; yy = y;
    myWidth = width; myHeight = height;
    segs = numSegments;
    startX[0] = x; endX[0] = x + myWidth/segs;
    for (int i = 1; i < segs; ++i) {
      startX[i] = endX[i-1];
      endX[i] = startX[i] + myWidth/segs;
    }
}

 /*!
  * \brief ProgressBar destructor method.
  * \details This is the destructor for the ProgressBar class.
  * \details Frees up memory that was allocated to a ProgressBar instance.
  */
ProgressBar::~ProgressBar() {
  delete [] startX; delete [] endX;
}

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
void ProgressBar::update(float newValue, int segnum) {
  if (segnum == -1)
    segnum = omp_get_thread_num();
  float d = max-min;
  float start = min + (d * segnum)/segs;
  float end = start + d/segs;
  clamp(newValue,start,end);
  float percent = (newValue-start) / (end-start);
  endX[segnum] = startX[segnum]+percent*(myWidth/segs);
}

 /*!
  * \brief Accessor for the ProgressBar's representative Polyline array.
  *   \param index Index of the segment to access.
  * \return A pointer to the Polyline array representing segment border <code>i</code>  of the ProgressBar.
  */
Polyline* ProgressBar::getBorder(int index) {
  int y2 = yy+myHeight;
  Polyline* p = new Polyline(5);
    p->addVertex(startX[index],yy,BLACK);
    p->addVertex(startX[index]+myWidth/segs,yy,BLACK);
    p->addVertex(startX[index]+myWidth/segs,y2,BLACK);
    p->addVertex(startX[index],y2,BLACK);
    p->addVertex(startX[index],yy,BLACK);
  return p;
}

 /*!
  * \brief Accessor for the ProgressBar's representative Rectangle array.
  *   \param index Index of the segment to access.
  * \return A pointer to the Rectangle array representing segment <code>i</code> of the ProgressBar.
  */
Rectangle* ProgressBar::getRect(int index) {
  return new Rectangle(startX[index], yy, endX[index]-startX[index], myHeight, Colors::highContrastColor(index));
}

}
