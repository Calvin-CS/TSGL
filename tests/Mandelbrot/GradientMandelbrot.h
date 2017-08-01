/*
 * GradientMandelbrot.h
 */

#ifndef GRADIENTMANDELBROT_H_
#define GRADIENTMANDELBROT_H_

#include "Mandelbrot.h"

using namespace tsgl;

/*!
 * \class GradientMandelbrot
 * \brief Draws a GradientMandelbrot set on a CartesianCanvas.
 * \details Similar to a normal Mandelbrot set but with a smoother shading.
 * \details Extends the Mandelbrot class and overrides its draw() function and setRedraw() function.
 * \details Can zoom in and out of the Mandelbrot set by scrolling up and down on the mouse wheel (respectively).
 * \see Mandelbrot class
 */
class GradientMandelbrot : public Mandelbrot {
public:

  /*!
   * \brief Explicitly constructs a GradientMandelbrot set.
   * \details Explicit constructor for the GradienMandelbrot class.
   *    \param threads The number of threads to use in the drawing of the GradientMandelbrot object.
   *    \param depth The number of iterations to go to in order to draw the GradientMandelbrot object.
   * \return The GradientMandelbrot object ready to be drawn onto the CartesianCanvas.
   */
  GradientMandelbrot(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the GradientMandelbrot set.
   * \details Actually draws the GradientMandelbrot set onto the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw to.
   * \note This method overrides the Mandelbrot class' draw() method.
   */
  void draw(CartesianRasterCanvas& can);
};

#endif /* GRADIENTMANDELBROT_H_ */
