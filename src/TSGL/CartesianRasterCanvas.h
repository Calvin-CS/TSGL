/*
* CartesianRasterCanvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
*/

#ifndef CARTESIAN_RASTER_CANVAS_H_
#define CARTESIAN_RASTER_CANVAS_H_

#include "RasterCanvas.h"
#include "CartesianCanvas.h"

namespace tsgl { //TODO: resolve issue that it doesn't close without ^C

  class CartesianRasterCanvas : public CartesianCanvas, public RasterCanvas {
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
    : Canvas(timerLength), CartesianCanvas(timerLength), RasterCanvas(timerLength) { };

    /*!
     * \brief Explicit CartesianCanvas constructor method.
     * \details This is an explicit constructor for the CartesianCanvas class.
     *   \param x The x position of the CartesianCanvas window.
     *   \param y The y position of the CartesianCanvas window.
     *   \param width The x dimension of the CartesianCanvas window.
     *   \param height The y dimension of the CartesianCanvas window.
     *   \param xMin The Cartesian coordinates of the CartesianCanvas's left bound.
     *   \param yMin The Cartesian coordinates of the CartesianCanvas's bottom bound.
     *   \param xMax The Cartesian coordinates of the CartesianCanvas's right bound.
     *   \param yMax The Cartesian coordinates of the CartesianCanvas's top bound.
     *   \param t The title of the window.
     *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
     *     A value less than or equal to 0 sets it to automatic.
     * \return A new CartesianCanvas with the specified position, dimensions, scaling, title,
     *   and timer length.
     */
    CartesianRasterCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax, std::string t, double timerLength = 0.0)
      : Canvas(x,y,width,height,t,timerLength), CartesianCanvas(x,y,width,height,xMin,yMin,xMax,yMax,t,timerLength), RasterCanvas(x,y,width,height,t,timerLength) { };

    /*!
     * \brief Gets the color of the pixel drawn on the current Canvas at the given screen coordinates,
     *   specified in row,column format.
     * \note (0,0) signifies the <b>top-left</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>bottom-left</b> of the screen when working with a CartesianCanvas.
     * \note getPixel() will return only what is currently drawn the screen. Any object waiting to be drawn
     *  will not affect what is returned.
     *      \param row The row (y-position) of the pixel to grab.
     *      \param col The column (x-position) of the pixel to grab.
     * \return A ColorInt containing the color of the pixel at (col,row).
     */
    virtual ColorInt getPixel(int row, int col);

    virtual void drawPoint(Decimal x, Decimal y, ColorInt c, float pointSize = 1.0);

  };
}

#endif /* CARTESIAN_RASTER_CANVAS_H_ */
