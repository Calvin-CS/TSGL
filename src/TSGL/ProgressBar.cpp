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
ProgressBar::ProgressBar(float x, float y, float z, float width, float height, float minValue, float maxValue, unsigned numSegments, float yaw, float pitch, float roll)
: Drawable(x, y, z, yaw, pitch, roll) {
    shaderType = SHAPE_SHADER_TYPE;
    segRecs = new Rectangle*[numSegments];
    segBorders = new Polyline*[numSegments];
    startX = new float[numSegments];
    endX = new float[numSegments];
    min = minValue; max = maxValue;
    myWidth = width; myHeight = height;
    segs = numSegments;
    startX[0] = -myWidth / 2; endX[0] = startX[0] + myWidth/segs;
    for (int i = 1; i < segs; ++i) {
      startX[i] = endX[i-1];
      endX[i] = startX[i] + myWidth/segs;
    }

    // all Polylines can have the same vertices; different locations will be handled with draw()
    vertices = new float[15];
    vertices[0] = -(myWidth/segs)/2; vertices[1] = myHeight/2; vertices[2] = 0;
    vertices[3] = -(myWidth/segs)/2; vertices[4] = -myHeight/2; vertices[5] = 0;
    vertices[6] = (myWidth/segs)/2; vertices[7] = -myHeight/2; vertices[8] = 0;
    vertices[9] = (myWidth/segs)/2; vertices[10] = myHeight/2; vertices[11] = 0;
    vertices[12] = -(myWidth/segs)/2; vertices[13] = myHeight/2; vertices[14] = 0;
    for (int i = 0; i < segs; i++) {
        segBorders[i] = new Polyline(0,0,0,5,vertices,yaw,pitch,roll,BLACK);
        segRecs[i] = new Rectangle(0,0,0,myWidth/segs,myHeight-2,yaw,pitch,roll,Colors::highContrastColor(i));
        segRecs[i]->setIsOutlined(false);
    }

    init = true;
}

 /*!
  * \brief ProgressBar destructor method.
  * \details This is the destructor for the ProgressBar class.
  * \details Frees up memory that was allocated to a ProgressBar instance.
  */
ProgressBar::~ProgressBar() {
  delete [] startX; delete [] endX;
  for (int i = 0; i < segs; i++) {
      delete segBorders[i];
      delete segRecs[i];
  }
  delete [] segBorders; delete [] segRecs;
}

void ProgressBar::draw(Shader * shader) {
    for (int i = 0; i < segs; i++) {
        segBorders[i]->setCenter(myCenterX + (myWidth/segs) * ( (float)i - (float)(segs-1)/2), myCenterY, myCenterZ);
        segRecs[i]->setCenter(myCenterX + (myWidth/segs) * ( (float)i - (float)(segs)/2) + segRecs[i]->getWidth()/2, myCenterY, myCenterZ);
        segBorders[i]->setRotationPoint(myCenterX, myCenterY, myCenterZ);
        segRecs[i]->setRotationPoint(myCenterX, myCenterY, myCenterZ);
        segBorders[i]->setYawPitchRoll(myCurrentYaw, myCurrentPitch, myCurrentRoll);
        segRecs[i]->setYawPitchRoll(myCurrentYaw, myCurrentPitch, myCurrentRoll);
        segRecs[i]->draw(shader);
        segBorders[i]->draw(shader);
    }
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
  segRecs[segnum]->setWidth(percent*(myWidth/segs));
}


}
