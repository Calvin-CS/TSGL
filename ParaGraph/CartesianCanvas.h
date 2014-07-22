/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/20/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Canvas.h"     // We extend Canvas
#include "Function.h"   // For drawing math functions on the screen

/*! \var typedef long double Decimal
    \brief A type definition for a long double.
*/
typedef long double Decimal;  // Define the variable type to use for coordinates

/*! \class CartesianCanvas
    \brief Canvas extended for graphic support.
    \details CartesianCanvas provides a Canvas with a Cartesian coordinate system for ease of plotting.
*/
class CartesianCanvas : public Canvas {
private:
    Decimal cartWidth,                                                  // maxX-minX
            cartHeight;                                                 // maxY-minY
    Decimal minX, maxX, minY, maxY;                                     // Bounding Cartesian coordinates for the window
    Decimal pixelWidth, pixelHeight;                                    // cartWidth/window.w(), cartHeight/window.h()
public:

    /*!
     * \brief Constructs a new CartesianCanvas.
     * \details This is the default constructor for the CartesianCanvas class
     *      \param b The size of the Canvas's internal vertex buffer.
     *      \return A new 800x600 CartesianCanvas, unscaled (stretching from -400 to +400 on the x axis and -300 to +300 on the y axis) in the middle of the screen with no title.
     */
    CartesianCanvas(unsigned int b);

    /*!
     * \brief Explicitly constructs a new CartesianCanvas.
     * \details This is an explicit constructor for the CartesianCanvas class.
     *      \param xx The x position of the CartesianCanvas window.
     *      \param yy The y position of the CartesianCanvas window.
     *      \param w The x dimension of the CartesianCanvas window.
     *      \param h The y dimension of the CartesianCanvas window.
     *      \param xMin The Cartestian coordinates of the CartesianCanvas's left bound.
     *      \param yMin The Cartestian coordinates of the CartesianCanvas's bottom bound.
     *      \param xMax The Cartestian coordinates of the CartesianCanvas's right bound.
     *      \param yMax The Cartestian coordinates of the CartesianCanvas's top bound.
     *      \param b The size of the Canvas's internal vertex buffer.
     *      \param title The title of the window.
     *      \return a new CartesianCanvas with the specified positional/scaling data, buffer size, and title
     */
    CartesianCanvas(int xx, int yy, int w, int h, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax, unsigned int b, std::string = "");

    /*!
     * \brief Draw axes.
     * \details This function draws axes (with tick marks) on the CartesianCanvas, centered at the given (Cartesian) coordinates
     *      \param x The horizontal location of the y-axis line.
     *      \param y The vertical location of the x-axis line.
     *      \param dx The distance between marks on the x-axis.
     *      \param dy The distance between marks on the y-axis.
     */
    void    drawAxes(Decimal x, Decimal y, Decimal dx, Decimal dy);

    /*!
     * \brief Draw a circle.
     * \details This function draws a circle with the given origin coordinates, radius, resolution (number of sides), and color.
     *      \param x The x coordinate of the circle's origin.
     *      \param y The y coordinate of the circle's origin.
     *      \param radius The radius of the circle in pixels.
     *      \param res The number of sides to use in the circle.
     *      \param color The color of the circle.
     *      \param filled Whether the circle should be filled.
     *  \note Identical to Canvas::drawCircle()
     */
    void    drawCircle(Decimal x, Decimal y, Decimal radius, int res, RGBfloatType color = BLACK, bool filled = true);

    /*!
     * \brief Plots a function on the screen
     * \details This function receives a Function as a parameter and plots the function on the CartesianCanvas.
     *      \param f The function to plot.
     * \note The passed function must receive exactly one Decimal parameter, and return a Decimal.
     */
    void    drawFunction(const Function* f);                            // Draws the Function on the screen

    /*!
      * \brief Draw an image.
      * \details This function draws an Image with the given coordinates and dimensions.
      *      \param fname The name of the file to load the image from.
      *      \param x The x coordinate of the Image's left edge.
      *      \param y The y coordinate of the Image's top edge.
      *      \param w The width of the Image.
      *      \param h The height of the Image.
      *      \param a The alpha with which to draw the Image.
      *  \note Identical to Canvas::drawImage()
      */
    void    drawImage(std::string fname, Decimal x, Decimal y, Decimal w, Decimal h, float a = 1.0f);

    /*!
     * \brief Draw a line.
     * \details This function draws a Line at the given coordinates with the given color.
     *      \param x1 The x position of the start of the line.
     *      \param y1 The y position of the start of the line.
     *      \param x2 The x position of the end of the line.
     *      \param y2 The y position of the end of the line.
     *      \param color The color of the line.
     *  \note Identical to Canvas::drawLine()
     */
    void    drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, RGBfloatType color = BLACK);

    /*!
     * \brief Draw a single pixel.
     * \details This function draws a Point at the given coordinates with the given color.
     *      \param x The x position of the point.
     *      \param y The y position of the point.
     *      \param color The color of the point.
     *  \note Identical to Canvas::drawPoint()
     */
    void    drawPoint(Decimal x, Decimal y, RGBfloatType color = BLACK);

    /*!
     * \brief Draw a rectangle.
     * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
     *      \param x The x coordinate of the Rectangle's left edge.
     *      \param y The y coordinate of the Rectangle's top edge.
     *      \param w The width of the Rectangle.
     *      \param h The height of the Rectangle.
     *      \param color The color of the rectangle.
     *      \param filled Whether the rectangle should be filled.
     *  \note Identical to Canvas::drawRectangle()
     */
    void    drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, RGBfloatType color = BLACK, bool filled = true);

    /*!
     * \brief Draw an arbitrary polygon with colored vertices.
     * \details This function draws a ShinyPolygon with the given vertex data
     *      \param size the number of vertices in the polygon
     *      \param x an array of x positions of the vertices
     *      \param y an array of y positions of the vertices
     *      \param color an array of colors for the vertices
     *      \param filled whether the shiny polygon should be filled (true) or not (false)
     * \note Identical to Canvas::drawShinyPolygon()
     */
    void    drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[], bool filled = true);

    /*!
     * \brief Draw a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *      \param s The string to draw.
     *      \param x The x coordinate of the text's left bound.
     *      \param y The y coordinate of the text's left bound.
     *      \param color The color of the Text.
     * \note Identical to Canvas::drawText().
     */
    void    drawText(std::string s, Decimal x, Decimal y, RGBfloatType color = BLACK);

    /*!
     * \brief Draw a triangle.
     * \details This function draws a Triangle with the given vertices.
     *      \param x1 the x coordinate of the first vertex of the Triangle.
     *      \param y1 the y coordinate of the first vertex of the Triangle.
     *      \param x2 the x coordinate of the second vertex of the Triangle.
     *      \param y2 the y coordinate of the second vertex of the Triangle.
     *      \param x3 the x coordinate of the third vertex of the Triangle.
     *      \param y3 the y coordinate of the third vertex of the Triangle.
     *      \param color the color of the Triangle.
     *      \param filled Whether the Triangle should be filled.
     * \note Identical to Canvas::drawTriangle()
     */
    void    drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color = BLACK, bool filled = true);

    /*!
     * \brief Translates Cartesian coordinates into window coordinates.
     * \details getCartesianCoordinates takes a pair of on-screen coordinates and translates them to Cartesian coordinates.
     *      \param screenX The window's x coordinate.
     *      \param screenY The window's y coordinate.
     *      \param cartX A reference variable to be filled with screenX's Cartesian position.
     *      \param cartY A reference variable to be filled with screenY's Cartesian position.
     */
    void    getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY);

    /*!
     * \brief Accessor for the CartesianCanvas's Cartesian height.
     *      \return The Cartesian height of the CartesianCanvas.
     */
    Decimal getCartHeight()       { return cartHeight; }

    /*!
     * \brief Accessor for the CartesianCanvas's Cartesian width.
     *      \return The Cartesian width of the CartesianCanvas.
     */
    Decimal getCartWidth()        { return cartWidth; }

    /*!
     * \brief Accessor for the CartesianCanvas's effective pixel width.
     *      \return The width corresponding to a single pixel in the current CartesianCanvas.
     */
    Decimal getPixelWidth()       { return pixelWidth; }

    /*!
     * \brief Accessor for the CartesianCanvas's effective pixel height.
     *      \return The height corresponding to a single pixel in the current CartesianCanvas.
     */
    Decimal getPixelHeight()      { return pixelHeight; }

    /*!
      * \brief Accessor for the CartesianCanvas's right bound.
      *      \return The real number corresponding the right of the CartesianCanvas.
      */
    Decimal getMaxX()             { return maxX; }

    /*!
     * \brief Accessor for the CartesianCanvas's top bound.
     *      \return The real number corresponding the top of the CartesianCanvas.
     */
    Decimal getMaxY()             { return maxY; }


    /*!
      * \brief Accessor for the CartesianCanvas's left bound.
      *      \return The real number corresponding the left of the CartesianCanvas.
      */
    Decimal getMinX()             { return minX; }

    /*!
      * \brief Accessor for the CartesianCanvas's bottom bound.
      *      \return The real number corresponding the bottom of the CartesianCanvas.
      */
    Decimal getMinY()             { return minY; }

    /*!
     * \brief Translates window coordinates into Cartesian coordinates.
     * \details getScreenCoordinates takes a pair of Cartesian coordinates and translates them to on-screen coordinates.
     *      \param cartX The Cartesian x coordinate.
     *      \param cartY The Cartesian y coordinate.
     *      \param screenX A reference variable to be filled with cartX's window position.
     *      \param screenY A reference variable to be filled with cartY's window position.
     */
    void    getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY);

    /*!
     * \brief Recomputes the CartesianCanvas's bounds.
     * \details This function recomputes the size variables of CartesianCanvas according to new bounds.
     *      \param xMin A real number corresponding to the new left edge of the CartesianCanvas.
     *      \param YMin A real number corresponding to the new bottom edge of the CartesianCanvas.
     *      \param xMax A real number corresponding to the new right edge of the CartesianCanvas.
     *      \param xMax A real number corresponding to the new top edge of the CartesianCanvas.
     */
    void    recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax);

    /*!
     * \brief Zoom the CartesianCanvas with a given center
     * \details This function will re-center the CartesianCanvas at the given coordinates, then zoom with respect to the given scale.
     *      \param x The coordinate to re-center the screen on.
     *      \param y The coordinate to re-center the screen on.
     *      \param scale The part to zoom in by. Less than 1 zooms in, greater than 1 zooms out.
     * \note This function will automatically maintain the current aspect ratio.
     */
    void    zoom(Decimal x, Decimal y, Decimal scale);

    /*!
     * \brief Zoom the CartesianCanvas with the given bounding (Cartesian) coordinates.
     * \details This function will zoom the CartesianCanvas with respect to the given bounding coordinates.
     *      \param x1 The left Cartesian bound.
     *      \param y1 The bottom Cartesian bound.
     *      \param x2 The right Cartesian bound.
     *      \param y2 The top Cartesian bound.
     * \note Setting the right bound lower than the left bound or the top lower than the bottom will just swap the variables.
     * \warning This function will *NOT* automatically maintain the previous aspect ratio.
     * \warning Change the aspect ratio on-the-fly only with caution.
     */
    void    zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2);
};

#endif /* CARTESIANCANVAS_H_- */
