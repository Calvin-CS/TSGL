/*
 * FastMandelbrot.h
 */

#ifndef FAST_MANDELBROT_H_
#define FAST_MANDELBROT_H_

#include "Mandelbrot.h"
using namespace tsgl;

/*!
 * \class FastMandelbrot
 * \brief Draw a Mandelbrot set.
 * \details The FastMandelbrot looks identical to regular Mandelbrot, but it divides the task based on threads completing lines rather than into equal chunks. Compare the time in the outputs.
 * \details Extends the Mandelbrot class and overrides its draw() method.
 * \see Mandelbrot class.
 */
class FastMandelbrot : public Mandelbrot {
public:

  /*!
   * \brief Explicitly construct a FastMandelbrot object.
   * \details Explicit constructor for the FastMandelbrot class.
   *    \param threads The number of threads to use in drawing the FastMandelbrot object onto the CartesianCanvas.
   *    \param depth The number of iterations to go to in order to draw the FastMandelbrot object.
   * \return The constructed FastMandelbrot object.
   */
  FastMandelbrot(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the FastMandelbrot object.
   * \details Actually draws the FastMandelbrot object onto the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw on.
   * \note Can be inherited by children classes who extend the FastMandelbrot class.
   */
  virtual void draw(CartesianRasterCanvas& can);
};

#endif /* FAST_MANDELBROT_H_ */
