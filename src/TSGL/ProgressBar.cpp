#include "ProgressBar.h"

namespace tsgl {

ProgressBar::ProgressBar(int x, int y, int w, int h, float minV, float maxV, unsigned segments) {
    startX = new float[segments];
    endX = new float[segments];
    min = minV; max = maxV;
    xx = x; yy = y;
    width = w; height = h;
    segs = segments;
    startX[0] = x; endX[0] = x + width/segs;
    for (int i = 1; i < segs; ++i) {
      startX[i] = endX[i-1];
      endX[i] = startX[i] + width/segs;
    }
}

ProgressBar::~ProgressBar() {
  delete [] startX; delete [] endX;
}

void ProgressBar::update(float newV, int seg) {
  if (seg == -1)
    seg = omp_get_thread_num();
  float d = max-min;
  float start = min + (d * seg)/segs;
  float end = start + d/segs;
  clamp(newV,start,end);
  float percent = (newV-start) / (end-start);
  endX[seg] = startX[seg]+percent*(width/segs);
}

Rectangle* ProgressBar::getRect(int i) {
  return new Rectangle(startX[i], yy, endX[i]-startX[i], height, Colors::highContrastColor(i));
}

Polyline* ProgressBar::getBorder(int i) {
  int y2 = yy+height;
  Polyline* p = new Polyline(5);
    p->addNextVertex(startX[i],yy,BLACK);
    p->addNextVertex(startX[i]+width/segs,yy,BLACK);
    p->addNextVertex(startX[i]+width/segs,y2,BLACK);
    p->addNextVertex(startX[i],y2,BLACK);
    p->addNextVertex(startX[i],yy,BLACK);
  return p;
}

}
