/*
* RasterCanvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
*/

#ifndef RASTER_CANVAS_H_
#define RASTER_CANVAS_H_

#include "Canvas.h"

namespace tsgl {

  class RasterCanvas : virtual public Canvas {
  // private:
    // std::vector<int> v;
  public:

    /*!
    * \brief Default Canvas constructor method.
    * \details This is the default constructor for the Canvas class.
    *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
    *     A value less than or equal to 0 sets it to automatic.
    * \return A new Canvas in the middle of the screen with no title.
    *   The created Canvas will take up approximately 90% of the monitor's height, and will
    *   have a 4:3 aspect ratio.
    */
    RasterCanvas(double timerLength = 0.0f) : Canvas(timerLength) {
      isRaster = true;
    };

    /*!
    * \brief Explicit Canvas constructor method.
    * \details This is the explicit constructor for the Canvas class.
    *   \param x The x position of the Canvas window.
    *   \param y The y position of the Canvas window.
    *   \param width The x dimension of the Canvas window.
    *   \param height The y dimension of the Canvas window.
    *   \param title The title of the window.
    *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
    *     A value less than or equal to 0 sets it to automatic.
    * \return A new Canvas with the specified position, dimensions, title, and draw cycle length.
    */
    RasterCanvas(int x, int y, int width, int height, std::string title, double timerLength = 0.0f) : Canvas(x,y,width,height,title,timerLength) {
      isRaster = true;
    };

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

    virtual void drawPoint(float x, float y, ColorInt c, float pointSize = 1.0);

  };
}

#endif /* RASTER_CANVAS_H_ */
