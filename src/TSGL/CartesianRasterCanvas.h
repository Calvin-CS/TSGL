/*
* CartesianRasterCanvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
*/

#ifndef CARTESIAN_RASTER_CANVAS_H_
#define CARTESIAN_RASTER_CANVAS_H_

#include "CartesianCanvas.h"

namespace tsgl { //TODO: resolve issue that it doesn't close without ^C

  /*!
  * \class CartesianRasterCanvas
  * \brief CartesianCanvas extended for point drawing in Cartesian coordinates.
  * \details CartesianRasterCanvas combines the Cartesian coordinate system with point drawing ability.
  * \note On the CartesianRasterCanvas, drawPoint() and getPixel() are according to the Cartesian coordinates, but all shapes are rendered with the standard coordinates.
  */
  class CartesianRasterCanvas : public CartesianCanvas {
  public:

    /*!
    * \brief Default CartesianRasterCanvas constructor method.
    * \details This is the default constructor for the CartesianRasterCanvas class.
    *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
    *     A value less than or equal to 0 sets it to automatic.
    * \return A new CartesianRasterCanvas in the middle of the screen with no title.
    *   The created Canvas will take up approximately 90% of the monitor's height, and will
    *   have a 4:3 aspect ratio.
    */
    CartesianRasterCanvas(double timerLength = 0.0f)
    : CartesianCanvas(timerLength) { isRaster = true; }

    /*!
     * \brief Explicit CartesianRasterCanvas constructor method.
     * \details This is an explicit constructor for the CartesianRasterCanvas class.
     *   \param x The x position of the CartesianRasterCanvas window.
     *   \param y The y position of the CartesianRasterCanvas window.
     *   \param width The x dimension of the CartesianRasterCanvas window.
     *   \param height The y dimension of the CartesianRasterCanvas window.
     *   \param xMin The Cartesian coordinates of the CartesianRasterCanvas's left bound.
     *   \param yMin The Cartesian coordinates of the CartesianRasterCanvas's bottom bound.
     *   \param xMax The Cartesian coordinates of the CartesianRasterCanvas's right bound.
     *   \param yMax The Cartesian coordinates of the CartesianRasterCanvas's top bound.
     *   \param t The title of the window.
     *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
     *     A value less than or equal to 0 sets it to automatic.
     * \return A new CartesianRasterCanvas with the specified position, dimensions, scaling, title,
     *   and timer length.
     */
    CartesianRasterCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax, std::string t, double timerLength = 0.0)
      : CartesianCanvas(x,y,width,height,xMin,yMin,xMax,yMax,t,timerLength) { isRaster = true; }

      /**
       * \brief Sets the color of the point on the Canvas at the given screen coordinates.
       * \details Draws a point at (x, y).
       *    \param x The x coordinate of the point to draw.
       *    \param y The y coordinate of the point to draw.
       *    \param c The ColorInt for the color of the point.
       *    \param pointSize The size of the point to draw. (defaults to 1.0)
       */
      virtual void drawPoint(float x, float y, ColorInt c, float pointSize = 1.0);

  };
}

#endif /* CARTESIAN_RASTER_CANVAS_H_ */
