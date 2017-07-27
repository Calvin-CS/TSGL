/*
 * Nova.h
 */

#ifndef NOVA_H_
#define NOVA_H_

#include "Mandelbrot.h"

using namespace tsgl;

/*!
 * \class Nova
 * \brief Draws a Nova (Newton Fractal) set on a CartesianCanvas.
 * \details Extends the Mandelbrot class and overrides its draw() function and setRedraw() function.
 * \details Can zoom in and out of the Nova set by scrolling up and down on the mouse wheel (respectively).
 * \see Mandelbrot class
 */
class Nova : public Mandelbrot {
public:

  /*!
   * \brief Explicitly constructs a Nova set.
   * \details Explicit constructor for the Nova class.
   *    \param threads The number of threads to use in the drawing of the Nova object.
   *    \param depth The number of iterations to go to in order to draw the Nova object.
   * \return The Nova object ready to be drawn onto the CartesianCanvas.
   */
  Nova(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the Nova set.
   * \details Actually draws the Nova set onto the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw to.
   * \note This method overrides the Mandelbrot class' draw() method.
   * \note CartesianRasterCanvas is a typedef for a CartesianCanvas object.
   */
  void draw(CartesianRasterCanvas& can);
};

#endif /* NOVA_H_ */
