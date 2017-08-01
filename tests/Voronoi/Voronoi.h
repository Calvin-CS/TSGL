/*
 * Voronoi.h
 */

#ifndef VORONOI_H_
#define VORONOI_H_

#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>
#include "Util.h"

using namespace tsgl;

/*!
 * \class Voronoi
 * \brief A Voronoi diagram.
 * \details Creates a Voronoi diagram to be drawn onto a RasterCanvas.
 * \see http://en.wikipedia.org/wiki/Voronoi_diagram.
 */
class Voronoi {
protected:
  static const int MY_POINTS = 100 * 4;
  int * myX;
  int * myY;
  int * myKValue;
  ColorFloat myColor[MY_POINTS];                 // And for an array of colors
  ColorFloat myTC, myRC, myLC, myBC, myXC, myYC; // Color for the top, right, left, bottom, x-average, and y-average
public:

  /*!
   * \brief Explicitly construct a Voronoi object.
   * \details Explicit constructor for a Voronoi object.
   * \param can Reference to the RasterCanvas to draw to.
   * \return The constructed Voronoi object.
   */
  Voronoi(RasterCanvas& can);

  /*!
   * \brief Draw the Voronoi object.
   * \details Actually draws the Voronoi object onto the RasterCanvas.
   * \param can Reference to the RasterCanvas to draw to.
   */
  void draw(RasterCanvas& can);

  /*!
   * \brief Destroy a Voronoi object.
   * \details Destructor for a Voronoi object.
   * \return The memory allocated to a Voronoi object.
   */
  virtual ~Voronoi();
};

#endif /* VORONOI_H_ */
