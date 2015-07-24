/*
 * Buddhabrot.h
 */

#ifndef BUDDHABROT_H_
#define BUDDHABROT_H_

#include <stdlib.h>

#include "Mandelbrot.h"

using namespace tsgl;

/*!
 * \class Buddhabrot
 * \brief Draw a Buddhabrot.
 * \details Contains all of the information necessary in order to draw a Buddhabrot.
 * \details Child class of the Mandelbrot class.
 * \see https://en.wikipedia.org/wiki/Buddhabrot for details on what a Buddhabrot is.
 * \see Mandelbrot class
 */
class Buddhabrot : public Mandelbrot {
private:
  int **counter;
  int cww, cwh;
public:

  /*!
   * \brief Explicitly construct a Buddhabrot object.
   * \details Explicit constructor for the Buddhabrot class.
   *    \param threads The number of threads to use when drawing the Buddhabrot object to the CartesianCanvas.
   *    \param depth The number of iterations to go to in order to draw the Buddhabrot object.
   * \return The constructed Buddhabrot object.
   */
  Buddhabrot(unsigned threads, unsigned depth);

  /*!
   * \brief Destroys a Buddhabrot object.
   * \details Destructor for the Buddhabrot class.
   * \return Frees up memory allocated to a Buddhabrot object.
   */
  ~Buddhabrot();

  /*!
   * \brief Draw the Buddhabrot.
   * \details Actually draws the Buddhabrot object to the CartesianCanvas.
   *    \param can Reference to the CartesianCanvas to draw on.
   * \note This method overrides the draw() method from Mandelbrot.
   * \note Cart is a typedef for CartesianCanvas.
   */
  void draw(Cart& can);
};

#endif /* BUDDHABROT_H_ */

