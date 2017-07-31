/*
 * ThreadMandelbrot.h
 */

#ifndef THREADMANDELBROT_H_
#define THREADMANDELBROT_H_

#include "Mandelbrot.h"

using namespace tsgl;

/*!
 * \class ThreadMandelbrot
 * \brief Draws a ThreadMandelbrot set on a CartesianCanvas.
 * \details Similar to a normal Mandelbrot set but with colors based on threads.
 * \details Extends the Mandelbrot class and overrides its draw() function and setRedraw() function.
 * \details Can zoom in and out of the Mandelbrot set by scrolling up and down on the mouse wheel (respectively).
 * \see Mandelbrot class
 */
class ThreadMandelbrot : public Mandelbrot {
public:

  /*!
   * \brief Explicitly constructs a ThreadMandelbrot set.
   * \details Explicit constructor for the GradienMandelbrot class.
   *    \param threads The number of threads to use in the drawing of the ThreadMandelbrot object.
   *    \param depth The number of iterations to go to in order to draw the ThreadMandelbrot object.
   * \return The ThreadMandelbrot object ready to be drawn onto the CartesianCanvas.
   */
  ThreadMandelbrot(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the ThreadMandelbrot set.
   * \details Actually draws the ThreadMandelbrot set onto the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw to.
   * \note This method overrides the Mandelbrot class' draw() method.
   * \note CartesianRasterCanvas is a typedef for a CartesianCanvas object.
   */
  void draw(CartesianRasterCanvas& can);
};

#endif /* THREADMANDELBROT_H_ */
