#include "ProgressBar.h"

namespace tsgl {

ProgressBar::ProgressBar(int x, int y, int w, int h, float minV, float maxV, unsigned segments) {
    startX = new float[segments];
    endX = new float[segments];
    min = minV; max = maxV;
    xx = x; yy = y;
    width = w; height = h;
    segs = segments;
    for (int i = 0; i < segs; ++i)
      startX[i] = endX[i] = x + i*(w/segs);
}

ProgressBar::~ProgressBar() {
  delete [] startX; delete [] endX;
}

void ProgressBar::update(unsigned seg, float newV) {
  float d = max-min;
  float start = min+seg*d/segs;
  float end = start + d/segs;
  clamp(newV,start,end);
  endX[seg] = xx+width*(newV-min)/d;
}

Rectangle* ProgressBar::getRect(int i) {
  return new Rectangle(startX[i], yy, endX[i]-startX[i], height, Colors::highContrastColor(i));
}

Polyline* ProgressBar::getBorder(int i) {
  int x2 = xx+(width*(i+1))/segs, y2 = yy+height;
  Polyline* p = new Polyline(5);
    p->addNextVertex(startX[i],yy,BLACK);
    p->addNextVertex(x2,yy,BLACK);
    p->addNextVertex(x2,y2,BLACK);
    p->addNextVertex(startX[i],y2,BLACK);
    p->addNextVertex(startX[i],yy,BLACK);
  return p;
}

}
