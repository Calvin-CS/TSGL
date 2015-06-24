#ifndef JULIA_H_
#define JULIA_H_

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
 * \class Julia
 * \brief Draw a Julia set.
 * \details Contains all of the information necessary in order to draw a Julia set onto a CartesianCanvas.
 * \details Can zoom in and out of the Julia set by scrolling the mouse wheel up and down (respectively).
 * \details Same is true when clicking the left mouse button (zooming in) and the right mouse button (zooming out).
 * \details Can also clear the screen by pressing the spacebar.
 */
class Julia {
private:
  Decimal myFirstX, myFirstY, mySecondX, mySecondY;
protected:
  int myThreads;
  unsigned int myDepth;
  bool myRedraw;
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
   * \brief Destroys a Julia object.
   * \details Destructor for the Julia class.
   * \note Does absolutely nothing.
   */
  virtual ~Julia();

  /*!
   * \brief Bind buttons such as mouse clicks and keys.
   * \details Binds keys and/or mouse clicks to a specific CartesianCanvas for I/O capabilities.
   * \details In this case: the mouse wheel, left and right mouse buttons, and the spacebar.
   *    \param can Reference to the CartesianCanvas to bind the buttons to.
   * \note Cart is a typedef for a CartesianCanvas object.
   */
  void bindings(Cart& can);

  /*!
   * \brief Draw the Julia object.
   * \details Actually draws the Julia object to a CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw on.
   * \note Cart is a typedef for a CartesianCanvas object.
   */
  virtual void draw(Cart& can);

  /*!
   * \brief Redraw the Julia object.
   * \details Sets a boolean flag that determines whether or not the Julia object should be redrawn to the CartesianCanvas.
   * \param newValue A boolean indicating whether or not we should redraw the Julia object.
   */
  virtual void setRedraw(bool newValue);
};

#endif /* JULIA_H_ */
