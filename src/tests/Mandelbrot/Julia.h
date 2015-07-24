/*
 * Julia.h
 */

#ifndef JULIA_H_
#define JULIA_H_

#include "Mandelbrot.h"

using namespace tsgl;

/*!
 * \class Julia
 * \brief Draw a Julia set.
 * \details Contains all of the information necessary in order to draw a Julia set onto a CartesianCanvas.
 * \details Can zoom in and out of the Julia set by scrolling the mouse wheel up and down (respectively).
 * \details Same is true when clicking the left mouse button (zooming in) and the right mouse button (zooming out).
 * \details Can also clear the screen by pressing the spacebar.
 */
class Julia : public Mandelbrot {
public:

  /*!
   * \brief Explicitly constructs a Julia object.
   * \details Explicit constructor for the Julia class.
   *    \param threads The number of threads to use in drawing the Julia object onto the CartesianCanvas.
   *    \param depth The number of iterations to go to in order to draw the Julia object.
   *\return The constructed Julia object.
   */
  Julia(unsigned threads, unsigned depth);

  /*!
   * \brief Draw the Julia object.
   * \details Actually draws the Julia object to a CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw on.
   * \note Cart is a typedef for a CartesianCanvas object.
   */
  void draw(Cart& can);
};

#endif /* JULIA_H_ */
