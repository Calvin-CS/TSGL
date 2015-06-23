/*
 * GradientMandelbrot.h
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
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
   *    \param depth The number of iterations until we are finished drawing the GradientMandelbrot object.
   * \return The GradientMandelbrot object ready to be drawn onto the CartesianCanvas.
   */
  GradientMandelbrot(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the GradientMandelbrot set.
   * \details Actually draws the GradientMandelbrot set onto the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw to.
   * \note This method overrides the Mandelbrot class' draw() method.
   * \note Cart is a typedef for a CartesianCanvas object.
   */
  void draw(Cart& can);

  /*!
   * \brief Redraw the GradientMandelbrot.
   * \details Sets a boolean flag to true or false if we have to redraw the
   * GradientMandelbrot onto the CartesianCanvas.
   *    \param newValue A boolean indicating whether or not we should redraw the GradientMandelbrot.
   * \note This method overrides the Mandelbrot class' setRedraw() method.
   */
  void setRedraw(bool newValue);
};

#endif /* GRADIENTMANDELBROT_H_ */

