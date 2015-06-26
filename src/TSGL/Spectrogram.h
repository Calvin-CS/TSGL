/*
 * Spectrogram.h
 *
 *  Created on: Jun 26, 2015
 *      Author: pretzel
 */

#ifndef SRC_TSGL_SPECTROGRAM_H_
#define SRC_TSGL_SPECTROGRAM_H_

#include "Canvas.h"
#include "Color.h"

namespace tsgl {

enum SpectrogramDrawmode {
  CIRCULAR = 0,
  HORIZONTAL = 1
};

class Spectrogram {
private:
  const int B = 16;  //Border
  const float PI = 3.14159;
  int myHeight, myWidth;
  float maxCount;
  int xx[NUM_COLORS+1], yy[NUM_COLORS+1], maxx[NUM_COLORS+1], maxy[NUM_COLORS+1];
  float count[NUM_COLORS+1];
  ColorFloat col[NUM_COLORS+1], black[NUM_COLORS+1];
  SpectrogramDrawmode myDrawMode;
  Canvas* can;
public:
  Spectrogram(SpectrogramDrawmode s, int w, int h = -1);
  ~Spectrogram();
  void update(int i, float weight = 1.0f, float decay = 0.95f);
  void draw(float ratio);
  void finish();
};

}

#endif /* SRC_TSGL_SPECTROGRAM_H_ */
