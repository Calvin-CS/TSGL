/*
 * Spectrogram.h
 *
 *  Created on: Jun 26, 2015
 *      Author: pretzel
 */

#ifndef SRC_TSGL_SPECTROGRAM_H_
#define SRC_TSGL_SPECTROGRAM_H_

#include <omp.h>

#include "Canvas.h"
#include "Color.h"

namespace tsgl {

enum SpectrogramDrawmode {
  CIRCULAR = 0,
  HORIZONTAL = 1
};

/*! \class Spectrogram
 *  \brief Displays a spectrogram of a given data set.
 *  \details Spectrogram is a class for visualizing data as a color spectrum.
 *    This data will typically be hues, but Spectrogram will accept any data
 *    mapping to an integer value between 0 and 255.
 */
class Spectrogram {
private:
  const int B = 16;  //Border
  const float PI = 3.14159;

  omp_lock_t writelock[NUM_COLORS];
  int myHeight, myWidth;
  float maxCount;
  int xx[NUM_COLORS+1], yy[NUM_COLORS+1], maxx[NUM_COLORS+1], maxy[NUM_COLORS+1];
  float count[NUM_COLORS+1];
  ColorFloat col[NUM_COLORS+1], black[NUM_COLORS+1];
  SpectrogramDrawmode myDrawMode;
  Canvas* can;
public:
  /*!
   * \brief Explicit Spectrogram constructor method.
   * \details This is the explicit constructor for the Spectrogram class.
   *   \param drawMode Method used for displaying spectral data. Can be one of CIRCULAR
   *     or HORIZONTAL.
   *   \param width The width of the Spectrogram canvas.
   *   \param height The height of the Spectrogram canvas. This value is ignored for
   *     HORIZONTAL Spectrograms. Setting this to -1 sets the width automatically.
   * \return A new Spectrogram with the specified drawing mode and size.
   */
  Spectrogram(SpectrogramDrawmode drawMode, int width, int height = -1);

  /*!
   * \brief Spectrogram destructor method.
   * \details This is the destructor for the Spectrogram class.
   * \details Frees up memory that was allocated to a Spectrogram instance.
   */
  ~Spectrogram();

  /*!
   * \brief Updates a spectrogram with new data.
   * \details This function adds the value of <code>weight</code> to the hue specified
   *   by <code>index</code>, and adds the value (<code>decay</code>^<code>n</code>)*
   *   <code>weight</code> to all hues <code>n</code> steps away from <code>index</code>.
   *   \param index Index of the hue to update. Value is taken mod 256.
   *   \param weight The value to add to <code>index</code>.
   *   \param decay Falloff for <code>weight</code> upon adjacent values.
   */
  void update(int index, float weight = 1.0f, float decay = 0.8f);

  /*!
   * \brief Updates the image on the spectrogram.
   * \details This function updates the Spectrogram's Canvas with the data since the last
   *   call to update() and redraws it.
   *   \param ratio The scaling of the visualizer. Accepts values between 0.0f and 1.0f.
   */
  void draw(float ratio);

  /*!
   * \brief Finishes the spectrogram.
   * \details This function tells the Spectrogram to free all of its memory and close
   *   down its Canvas.
   */
  void finish();
};

}

#endif /* SRC_TSGL_SPECTROGRAM_H_ */
