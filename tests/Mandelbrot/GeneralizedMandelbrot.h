/*
 * GeneralizedMandelbrot.h
 */

#ifndef GENERALIZED_MANDELBROT_H_
#define GENERALIZED_MANDELBROT_H_

#include "Mandelbrot.h"

using namespace tsgl;

/*!
 * \class GeneralizedMandelbrot
 * \brief Draw a GeneralizedMandelbrot set.
 * \details Contains all of the information necessary in order to draw a GeneralizedMandelbrot set onto a CartesianCanvas.
 * \details Can zoom in and out of the GeneralizedMandelbrot set by scrolling the mouse wheel up and down (respectively).
 * \details Same is true when clicking the left mouse button (zooming in) and the right mouse button (zooming out).
 * \details Can also clear the screen by pressing the spacebar.
 */
class GeneralizedMandelbrot : public Mandelbrot {
public:

  /*!
   * \brief Explicitly constructs a GeneralizedMandelbrot object.
   * \details Explicit constructor for the GeneralizedMandelbrot class.
   *    \param threads The number of threads to use in drawing the GeneralizedMandelbrot object onto the CartesianCanvas.
   *    \param depth The number of iterations to go to in order to draw the GeneralizedMandelbrot object.
   *\return The constructed GeneralizedMandelbrot object.
   */
  GeneralizedMandelbrot(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the GeneralizedMandelbrot object.
   * \details Actually draws the GeneralizedMandelbrot object to a CartesianCanvas.
   * \note Change the value of n in f(z) = z^n + c at the beginning of the method.
   *    \param can Reference to the CartesianCanvas to draw on.
   */
  void draw(CartesianRasterCanvas& can);
};

#endif /* GENERALIZED_MANDELBROT_H_ */
