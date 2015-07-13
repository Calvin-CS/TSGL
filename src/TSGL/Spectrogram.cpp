#include "Spectrogram.h"

namespace tsgl {

const int Spectrogram::B = 16;
const float Spectrogram::PI = 3.149f;
Spectrogram::Spectrogram(SpectrogramDrawmode drawMode, int width, int height) {
  for (int i = 0; i < NUM_COLORS; ++i)
    omp_init_lock(&(writelock[i]));
  omp_init_lock(&masterlock);
  myWidth = width;
  myHeight = (height > 0) ? height : width;
  myDrawMode = drawMode;
  if (myDrawMode == HORIZONTAL)
    myHeight = NUM_COLORS;
  else if (myDrawMode == VERTICAL)
    myWidth = NUM_COLORS;

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
  for (int i = 0; i < NUM_COLORS; ++i)
    omp_destroy_lock(&(writelock[i]));
  omp_destroy_lock(&masterlock);
  delete can;
}

void Spectrogram::updateLocked(int index, float weight, float decay) {
  int i = index % NUM_COLORS;
  omp_set_lock(&(writelock[i]));
    count[i] += weight;
    if (count[i] > maxCount) maxCount = count[i];
  omp_unset_lock(&(writelock[i]));
  weight *= decay;
  for (int k = 1; k < NUM_COLORS/2; ++k) {
    i = (index + k) % NUM_COLORS;
    omp_set_lock(&(writelock[i]));
      count[i] += weight;
      if (count[i] > maxCount) maxCount = count[i];
    omp_unset_lock(&(writelock[i]));
    i = (index + NUM_COLORS - k) % NUM_COLORS;
    omp_set_lock(&(writelock[i]));
      count[i] += weight;
      maxCount > count[i] ? true : maxCount = count[i];
      if (count[i] > maxCount) maxCount = count[i];
    omp_unset_lock(&(writelock[i]));
    weight *= decay;
  }
}

void Spectrogram::updateCritical(int index, float weight, float decay) {
  int i = index % NUM_COLORS;
  #pragma omp critical
  {
    count[i] += weight;
    if (count[i] > maxCount)
      maxCount = count[i];
    weight *= decay;
    for (int k = 1; k < NUM_COLORS/2; ++k) {
      i = (index + k) % NUM_COLORS;
      count[i] += weight;
      if (count[i] > maxCount)
        maxCount = count[i];
      i = (index + NUM_COLORS - k) % NUM_COLORS;
      count[i] += weight;
      if (count[i] > maxCount)
        maxCount = count[i];
      weight *= decay;
    }
  }
}

void Spectrogram::draw(float ratio) {
  if (maxCount > 0) {
    const float DELTA = (2*PI)/NUM_COLORS;
    float localmax = maxCount;
    float invcount = 1.0f/localmax;
    float mult = ratio*myHeight*sqrt(invcount);
    switch (myDrawMode) {
    case RADIAL:
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
      can->pauseDrawing();
      can->clear();
      for (int k = 0; k < MAX_COLOR; ++k)
        can->drawLine(0,k,(ratio*myWidth*count[k])/localmax,k,ColorHSV((6.0f*k)/MAX_COLOR,1.0f,1.0f));
      can->resumeDrawing();
      break;
    case VERTICAL:
      can->pauseDrawing();
//      can->clear();
      for (int k = 0; k < MAX_COLOR; ++k)
        can->drawLine(k,myHeight,k,myHeight-(ratio*myHeight*count[k])/localmax,ColorHSV((6.0f*k)/MAX_COLOR,1.0f,1.0f));
      can->resumeDrawing();
      break;
    }
  }
}

void Spectrogram::finish() {
  can->wait();
}

}
