/*
* RasterCanvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
*/

#ifndef RASTER_CANVAS_H_
#define RASTER_CANVAS_H_

#include "Canvas.h"

namespace tsgl {

  /*!
   * \class RasterCanvas
   * \brief Canvas extended and renders objects only once, as they are added.
   * \details RasterCanvas provides a Canvas with ability to draw individual points.
   * \note On the RasterCanvas, layers do not work, and drawn objects cannot be removed.
   */
  class RasterCanvas : public Canvas {
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

#endif /* RASTER_CANVAS_H_ */
