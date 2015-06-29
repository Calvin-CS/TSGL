#include "Spectrogram.h"

namespace tsgl {

Spectrogram::Spectrogram(SpectrogramDrawmode s, int w, int h) {
  myWidth = w;
  myHeight = (h > 0) ? h : w;
  myDrawMode = s;

  can = new Canvas(-1, 0, myWidth, myHeight ,"");
  maxCount = 0;
  count[0] = 0;
  xx[0] = (myWidth)/2;
  yy[0] = (myHeight)/2;
  col[0] = WHITE;
  black[0] = BLACK;
  for (unsigned i = 1; i < NUM_COLORS; ++i) {
    count[i] = 0;
    xx[i] = (myWidth + myWidth*cos((2*PI*i)/NUM_COLORS))/2;
    yy[i] = (myHeight + myHeight*sin((2*PI*i)/NUM_COLORS))/2;
    col[i] = ColorHSV(6.0f*i/255.0f,1.0f,1.0f);
    black[i] = col[i] * 0.25f;
  }
  count[NUM_COLORS] = 0;
  xx[NUM_COLORS] = xx[1];
  yy[NUM_COLORS] = yy[1];
  col[NUM_COLORS] = col[1];
  black[NUM_COLORS] = black[1];
  for (int k = 0; k < NUM_COLORS+1; ++k) {
    maxx[k] = xx[k];
    maxy[k] = yy[k];
  }
  can->start();
}

Spectrogram::~Spectrogram() {
  delete can;
}

void Spectrogram::update(int i, float weight, float decay) {
  int index = i % NUM_COLORS;
  count[index] += weight;
  if (count[index] > maxCount)
    maxCount = count[index];
  weight *= decay;
  for (int k = 1; k < NUM_COLORS/2; ++k) {
    index = (i + k) % NUM_COLORS;
    count[index] += weight;
    if (count[index] > maxCount)
      maxCount = count[index];
    index = (i + NUM_COLORS - k) % NUM_COLORS;
    count[index] += weight;
    if (count[index] > maxCount)
      maxCount = count[index];
    weight *= decay;
  }
}

void Spectrogram::draw(float ratio) {
  if (maxCount > 0) {
    switch (myDrawMode) {
    case CIRCULAR:
      const float DELTA = (2*PI)/NUM_COLORS;
      float invcount = 1.0f/maxCount;
      float mult = ratio*myHeight*sqrt(invcount);
      for (int k = 0; k < MAX_COLOR; ++k) {
        float kroot = mult*sqrt(count[k]);
        xx[k+1] = (myWidth + kroot*cos(k*DELTA))/2;
        yy[k+1] = (myHeight + kroot*sin(k*DELTA))/2;
        col[k+1] = ColorHSV(6.0f*k/255.0f,invcount*count[k],1.0f);
      }
      xx[NUM_COLORS] = xx[1];
      yy[NUM_COLORS] = yy[1];
      col[NUM_COLORS] = col[1];
      can->clear();
      can->drawConvexPolygon(NUM_COLORS+1,maxx,maxy,black,true);
      can->drawConvexPolygon(NUM_COLORS+1,xx,yy,col,true);
      break;
    case HORIZONTAL:
      for (int k = 0; k < MAX_COLOR; ++k)
        can->drawLine(0,k,(ratio*myWidth*count[k])/maxCount,k,ColorHSV((6.0f*k)/MAX_COLOR,1.0f,1.0f));
      break;
    }
  }
}

void Spectrogram::finish() {
  can->wait();
}

}
