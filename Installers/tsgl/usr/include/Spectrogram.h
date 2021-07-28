/*
 * Spectrogram.h
 */

#ifndef SRC_TSGL_SPECTROGRAM_H_
#define SRC_TSGL_SPECTROGRAM_H_

#include <omp.h>

#include "Canvas.h"
#include "Color.h"

namespace tsgl {

enum SpectrogramDrawmode {
  RADIAL = 0,
  HORIZONTAL = 1,
  VERTICAL = 2
};

/*! \class Spectrogram
 *  \brief Displays a spectrogram of a given data set.
 *  \details Spectrogram is a class for visualizing data as a color spectrum.
 *    This data will typically be hues, but Spectrogram will accept any data
 *    mapping to an integer value between 0 and 255.
 */
class Spectrogram {
private:
  static const int B;  //Border
  static const float PI;

  omp_lock_t writelock[NUM_COLORS], masterlock;
  int myHeight, myWidth;
  float centerX, centerY;
  float maxCount;
  float xx[NUM_COLORS+1], yy[NUM_COLORS+1], maxx[NUM_COLORS+1], maxy[NUM_COLORS+1];
  float count[NUM_COLORS+1];
  ColorFloat col[NUM_COLORS+1], black[NUM_COLORS+1];
  SpectrogramDrawmode myDrawMode;
  Canvas* can;
public:
  Spectrogram(SpectrogramDrawmode drawMode, int width, int height = -1);

  ~Spectrogram();

  void updateLocked(int index, float weight = 1.0f, float decay = 0.8f);

  void updateCritical(int index, float weight = 1.0f, float decay = 0.8f);

  void draw(float ratio);

  void finish();
};

}

#endif /* SRC_TSGL_SPECTROGRAM_H_ */
