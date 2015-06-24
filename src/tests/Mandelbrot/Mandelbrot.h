/*
 * Mandelbrot.cpp
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
 */

#ifndef MANDELBROT_H_
#define MANDELBROT_H_

//Imports, constants....
#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \var Cart
 * \brief Typedef for CartesianCanvas.
 */
typedef CartesianCanvas Cart;

/*!
 * \var complex
 * \brief Typedef for std::complex<long double>.
 */
typedef std::complex<long double> complex;

/*!
 * \class Mandelbrot
 * \brief Draw a Mandelbrot set.
 * \details Contains the information necessary in order to draw a Mandelbrot set onto a CartesianCanvas.
 * \details Can zoom in and out of the screen by scrolling up and down on the mouse wheel (respectively).
 * \details Same is true if you click on the left mouse button (zoom in) and right mouse button (zoom out).
 */
class Mandelbrot {
private:
  Decimal myFirstX, myFirstY, mySecondX, mySecondY;
protected:
  int myThreads;
  unsigned int myDepth;
  bool myRedraw;
public:

  /*!
   * \brief Explicitly construct a Mandelbrot object.
   * \details Explicit constructor for the Mandelbrot class.
   *    \param threads The number of threads to use in drawing the Mandelbrot object onto the CartesianCanvas.
   *    \param depth The number of iterations to go to in order to draw the Mandelbrot object.
   * \return The constructed Mandelbrot object.
   */
  Mandelbrot(unsigned threads, unsigned depth);

  /*!
   * \brief Destroys a Mandelbrot object and any children of the Mandelbrot class.
   * \details Destructor for the Mandelbrot class and its children.
   * \note Does not actually do anything.
   */
  virtual ~Mandelbrot() {};

  /*!
   * \brief Binds buttons and/or mouse clicks.
   * \details Binds buttons and/or mouse clicks needed for I/O capabilities.
   * \details In this case: the mouse wheel, left and right mouse buttons.
   *    \param can Reference to the CartesianCanvas to have the buttons bound to.
   * \note Cart is a typedef for CartesianCanvas.
   */
  void bindings(Cart& can);

  /*!
   * \brief Draw the Mandelbrot object.
   * \details Actually draws the Mandelbrot object onto the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw on.
   * \note Can be inherited by children classes who extend the Mandelbrot class.
   * \note Cart is a typedef for CartesianCanvas.
   */
  virtual void draw(Cart& can);

  /*!
   * \brief Redraw the Mandelbrot object.
   * \details Sets a boolean flag that determines if we should redraw the Mandelbrot object or not.
   *    \param newValue A boolean indicating whether or not we should redraw.
   * \note Can be inherited by children classes who extend the Mandelbrot class.
   */
  virtual void setRedraw(bool newValue);
};

#endif /* MANDELBROT_H_ */
