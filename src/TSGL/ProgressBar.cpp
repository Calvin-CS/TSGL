#include "ProgressBar.h"

namespace tsgl {

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

ProgressBar::~ProgressBar() {
  delete [] startX; delete [] endX;
}

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

Rectangle* ProgressBar::getRect(int index) {
  return new Rectangle(startX[index], yy, endX[index]-startX[index], myHeight, Colors::highContrastColor(index));
}

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

}
