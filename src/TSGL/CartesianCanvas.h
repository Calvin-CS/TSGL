/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting.
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Canvas.h"     // We extend Canvas
#include "Function.h"   // For drawing math functions on the screen
#include "Util.h"

namespace tsgl {

typedef Decimal (*functionPointer)(Decimal x);

/*!
 * \class CartesianCanvas
 * \brief Canvas extended for graphic support.
 * \details CartesianCanvas provides a Canvas with a Cartesian coordinate system for ease of plotting.
 */
class CartesianCanvas : public Canvas {
private:
    Decimal cartWidth,                                                  // maxX-minX
            cartHeight;                                                 // maxY-minY
    Decimal minX, maxX, minY, maxY;                                     // Bounding Cartesian coordinates for the window
    Decimal pixelWidth, pixelHeight;                                    // cartWidth/window.w(), cartHeight/window.h()

    static bool testZoom(CartesianCanvas& can);                         // Unit test for zoom() methods
    static bool testRecomputeDimensions(CartesianCanvas& can);          // Unit test for recomputeDimensions()
    static bool testDraw(CartesianCanvas& can);                         // Unit test for drawing
public:

    /*!
     * \brief Default CartesianCanvas constructor method.
     * \details This is the default constructor for the CartesianCanvas class.
     *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
     *     A value less than or equal to 0 sets it to automatic.
     * \return A new CartesianCanvas, stretching from -400 to +400 on the x axis and
     *   -300 to +300 on the y axis, in the middle of the screen with no title.
     *   The created CartesianCanvas will take up approximately 90% of the monitor's height, and will
     *   have a 4:3 aspect ratio.
     */
    CartesianCanvas(double timerLength = 0.0);

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
     *   \param title The title of the window.
     *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
     *     A value less than or equal to 0 sets it to automatic.
     * \return A new CartesianCanvas with the specified position, dimensions, scaling, title,
     *   and timer length.
     */
    CartesianCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax,
                    std::string t, double timerLength = 0.0);

    /*!
     * \brief Draws axes on the Cartesian Canvas.
     * \details This function draws axes (with tick marks) on the CartesianCanvas, centered at the
     *  given (Cartesian) coordinates
     *    \param originX The horizontal location of the y-axis line.
     *    \param originY The vertical location of the x-axis line.
     *    \param spacingX The distance between marks on the x-axis.
     *    \param spacingY The distance between marks on the y-axis.
     */
    void drawAxes(Decimal originX, Decimal originY, Decimal spacingX, Decimal spacingY);

    /*!
     * \brief Draws a circle.
     * \details This function draws a circle with the given center, radius, resolution
     *   (number of sides), color, and fill status.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param sides The number of sides to use in the circle.
     *   \param color The color of the circle
     *     (set to BLACK by default).
     *   \param filled Whether the circle should be filled
     *     (set to true by default).
     * \note Identical to Canvas::drawCircle().
     */
    void drawCircle(Decimal x, Decimal y, Decimal radius, int sides, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draws an arbitrary polygon with colored vertices.
     * \details This function draws a ColoredPolygon with the given vertex data, specified as
     *   a triangle strip.
     *   \param size The number of vertices in the polygon.
     *   \param xverts An array of x positions of the vertices.
     *   \param yverts An array of y positions of the vertices.
     *   \param color An array of colors for the vertices.
     *   \param filled Whether the colored polygon should be filled (true) or not (false)
     *     (set to true by default).
     * \note Identical to Canvas::drawColoredPolygon().
     */
    void drawColoredPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws a Concave polygon with colored vertices.
     * \details This function draws a ConcavePolygon with the given vertex data, specified as the
     *   outer perimeter of the polygon.
     *   \param size The number of vertices in the polygon.
     *   \param xverts An array of x positions of said vertices.
     *   \param yverts An array of y positions of said vertices.
     *   \param color An array of colors for the said vertices.
     *   \param filled Whether the Concave polygon should be filled in or not
     *     (set to true by default).
     * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
     *   that you draw Convex polygons with this function.
     * \note Identical to Canvas::drawConcavePolygon().
     * \see drawConvexPolygon().
     */
    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws a convex polygon with colored vertices.
     * \details This function draws a ConvexPolygon with the given vertex data, specified as the
     *   outer perimeter of the polygon.
     *   \param size The number of vertices in the polygon.
     *   \param xverts An array of the x positions of said vertices.
     *   \param yverts An array of the y positions of said vertices.
     *   \param color An array of colors for the said vertices.
     *   \param filled Whether the ConvexPolygon should be filled in or not
     *     (set to true by default).
     * \note The difference between a convex polygon and a concave polygon
     *   is that a convex polygon has all interior angles less than
     *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
     * \note Identical to Canvas::drawConvexPolygon().
     */
    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Plots a function on the screen.
     * \details This function receives a TSGL Function instance as a parameter and plots the function on the CartesianCanvas.
     *   \param function Reference to the Function to plot.
     *   \param sleepTime Time to sleep between plotting points
     *   \param color The color of the vertices of the plotted function (set to BLACK by default).
     */
    void drawFunction(const Function &function, float sleepTime = 0.0f, ColorFloat color = BLACK);

    /*!
     * \brief Plots a function on the screen.
     * \details This function receives a pointer to a function method as a parameter and plots the function on
     *   the CartesianCanvas.
     *   \param function Pointer to the function-drawing method to plot.
     *   \param sleepTime Time to sleep between plotting points
     *   \param color The color of the vertices of the plotted function (set to BLACK by default).
     * \note <code>function</code> must receive exactly one Decimal x parameter, and return a Decimal y parameter.
     */
    void drawFunction(functionPointer &function, float sleepTime = 0.0f, ColorFloat color = BLACK);

    /*!
     * \brief Draws an image.
     * \details This function draws an Image with the given coordinates and dimensions.
     *   \param function The name of the file to load the image from.
     *   \param x The x coordinate of the Image's left edge.
     *   \param y The y coordinate of the Image's top edge.
     *   \param w The width of the Image.
     *   \param h The height of the Image.
     *   \param a The alpha with which to draw the Image
     *     (set to 1.0f by default).
     * \note Identical to Canvas::drawImage().
     */
    void drawImage(std::string filename, Decimal x, Decimal y, Decimal w, Decimal h, float a = 1.0f);

    /*!
     * \brief Draws a line.
     * \details This function draws a Line at the given coordinates with the given color.
     *   \param x1 The x position of the start of the line.
     *   \param y1 The y position of the start of the line.
     *   \param x2 The x position of the end of the line.
     *   \param y2 The y position of the end of the line.
     *   \param color The color of the line
     *     (set to BLACK by default).
     *  \note Identical to Canvas::drawLine().
     */
    void drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color = BLACK);

    /*!
     * \brief Plots part of a function on the screen.
     * \details This function receives a pointer to a function method as a parameter and plots the function on
     *   the CartesianCanvas between the specified minimum and maximum coordinates.
     *   \param function Pointer to the function-drawing method to plot.
     *   \param min Minimum x value to evaluate and plot
     *   \param max Maximum x value to evaluate and plot
     *   \param sleepTime Time to sleep between plotting points
     *   \param color The color of the vertices of the plotted function (set to BLACK by default).
     * \note <code>function</code> must receive exactly one Decimal x parameter, and return a Decimal y parameter.
     */
    void drawPartialFunction(functionPointer &function, Decimal min, Decimal max,
                             float sleepTime = 0.0f, ColorFloat color = BLACK);

    /*!
     * \brief Draws a single pixel, specified in row,column format.
     * \details This function draws a pixel at the given screen coordinates with the given color.
     * \note (0,0) signifies the <b>top-left</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>bottom-left</b> of the screen when working with a CartesianCanvas object.
     *   \param row The row (y-position) of the pixel.
     *   \param col The column (x-position) of the pixel.
     *   \param color The color of the point (set to BLACK by default).
     * \see drawPixel()
     * \note Identical to Canvas::drawPixel().
     */
    void drawPixel(Decimal row, Decimal col, ColorFloat color = BLACK);

    /*!
     * \brief Draws a single pixel, specified in x,y format.
     * \details This function draws a pixel at the given Cartesian coordinates with the given color.
     * \note (0,0) signifies the <b>left-top</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>left-bottom</b> of the screen when working with a CartesianCanvas object.
     *   \param x The x position of the point.
     *   \param y The y position of the point.
     *   \param color The color of the point (set to BLACK by default).
     * \see drawPoint()
     * \note Identical to Canvas::drawPoint().
     */
    void drawPoint(Decimal x, Decimal y, ColorFloat color = BLACK);

    /*!
     * \brief Draws a rectangle.
     * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
     *   \param x1 The x coordinate of the Rectangle's left edge.
     *   \param y1 The y coordinate of the Rectangle's top edge.
     *   \param x2 The x coordinate of the Rectangle's right edge.
     *   \param y2 The y coordinate of the Rectangle's bottom edge.
     *   \param color The color of the rectangle
     *     (set to BLACK by default).
     *   \param filled Whether the rectangle should be filled
     *     (set to true by default).
     * \note Identical to Canvas::drawRectangle().
     */
    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color = BLACK, bool filled = true);


    /*!
     * \brief Draw a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *   \paramtexts The string to draw.
     *   \param x The x coordinate of the text's left bound.
     *   \param y The y coordinate of the text's left bound.
     *   \param size The size of the text in pixels.
     *   \param color The color of the Text (set to BLACK by default).
     * \note Identical to Canvas::drawText(std::string,..).
     */
    void drawText(std::string text, Decimal x, Decimal y, unsigned size, ColorFloat color = BLACK);

    /*!
     * \brief Draw a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *   \param text The UTF8-encoded string to draw.
     *   \param x The x coordinate of the text's left bound.
     *   \param y The y coordinate of the text's left bound.
     *   \param size The size of the text in pixels.
     *   \param color The color of the Text (set to BLACK by default).
     * \note Identical to Canvas::drawText(std::wstring,..).
     */
    void drawText(std::wstring text, Decimal x, Decimal y, unsigned size, ColorFloat color = BLACK);

    /*!
     * \brief Draw a triangle.
     * \details This function draws a Triangle with the given vertices.
     *      \param x1 The x coordinate of the first vertex of the Triangle.
     *      \param y1 The y coordinate of the first vertex of the Triangle.
     *      \param x2 The x coordinate of the second vertex of the Triangle.
     *      \param y2 The y coordinate of the second vertex of the Triangle.
     *      \param x3 The x coordinate of the third vertex of the Triangle.
     *      \param y3 The y coordinate of the third vertex of the Triangle.
     *      \param color The color of the Triangle (set to BLACK by default).
     *      \param filled Whether the Triangle should be filled (set to true by default).
     * \note Identical to Canvas::drawTriangle().
     */
    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color = BLACK,
                      bool filled = true);

    /*!
     * \brief Translates Cartesian coordinates into window coordinates.
     * \details getCartesianCoordinates() takes a pair of on-screen coordinates and translates them to Cartesian
     *  coordinates.
     *      \param screenX The window's x coordinate.
     *      \param screenY The window's y coordinate.
     *      \param cartX A reference variable to be filled with screenX's Cartesian position.
     *      \param cartY A reference variable to be filled with screenY's Cartesian position.
     */
    void getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY);

    /*!
     * \brief Accessor for the CartesianCanvas's Cartesian height.
     * \return The Cartesian height of the CartesianCanvas.
     */
    Decimal getCartHeight();

    /*!
     * \brief Accessor for the CartesianCanvas's Cartesian width.
     * \return The Cartesian width of the CartesianCanvas.
     */
    Decimal getCartWidth();

    /*!
     * \brief Accessor for the CartesianCanvas's right bound.
     * \return The real number corresponding the right of the CartesianCanvas.
     */
    Decimal getMaxX();

    /*!
     * \brief Accessor for the CartesianCanvas's top bound.
     * \return The real number corresponding the top of the CartesianCanvas.
     */
    Decimal getMaxY();

    /*!
     * \brief Accessor for the CartesianCanvas's left bound.
     * \return The real number corresponding the left of the CartesianCanvas.
     */
    Decimal getMinX();

    /*!
     * \brief Accessor for the CartesianCanvas's bottom bound.
     * \return The real number corresponding the bottom of the CartesianCanvas.
     */
    Decimal getMinY();

    /*!
     * \brief Accessor for the CartesianCanvas's effective pixel width.
     * \return The width corresponding to a single pixel in the current CartesianCanvas.
     */
    Decimal getPixelWidth();

    /*!
     * \brief Accessor for the CartesianCanvas's effective pixel height.
     * \return The height corresponding to a single pixel in the current CartesianCanvas.
     */
    Decimal getPixelHeight();

    /*!
     * \brief Translates window coordinates into Cartesian coordinates.
     * \details getScreenCoordinates() takes a pair of Cartesian coordinates and translates them to on-screen
     *   coordinates.
     *   \param cartX The Cartesian x coordinate.
     *   \param cartY The Cartesian y coordinate.
     *   \param screenX A reference variable to be filled with cartX's window position.
     *   \param screenY A reference variable to be filled with cartY's window position.
     */
    void getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY);

    /*!
     * \brief Recomputes the CartesianCanvas's bounds.
     * \details This function recomputes the size variables of CartesianCanvas according to new bounds.
     *   \param xMin A real number corresponding to the new left edge of the CartesianCanvas.
     *   \param YMin A real number corresponding to the new bottom edge of the CartesianCanvas.
     *   \param xMax A real number corresponding to the new right edge of the CartesianCanvas.
     *   \param xMax A real number corresponding to the new top edge of the CartesianCanvas.
     */
    void recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax);
    /*!
      * \brief Resets the internal drawing timer of a CartesianCanvas object.
      * \note Can be thought of as a wrapper function for a Timer function call.
      * \note Identical to Canvas::reset().
      */
    void reset();

    /*!
     * \brief Start the CartesianCanvas, run a function on it, and wait for the user to close it
     * \details This function binds another function to the current CartesianCanvas, waits until that function is
     *   complete, and waits for the user to close the CartesianCanvas.  This function effectively calls start(),
     *   <code>myFunction</code>(), and wait() in sequence.
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     */
    void run(void (*myFunction)(CartesianCanvas&));

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param i An integer argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, int), int i);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param u An unsigned integer argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, unsigned), unsigned u);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param i1 An integer argument to myFunction
     * \param i2 An integer argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, int, int), int i1, int i2);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param u1 An unsigned integer argument to myFunction
     * \param u2 An unsigned integer argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, unsigned, unsigned), unsigned u1, unsigned u2);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param s A string argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, std::string),std::string s);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param i An integer argument to myFunction
     * \param s A string argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, int, std::string), int i, std::string s);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
     *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
     * \param s A string argument to myFunction
     * \param i An integer argument to myFunction
     */
    void run(void (*myFunction)(CartesianCanvas&, std::string, int), std::string s, int i);

    /*!
     * \brief Sleeps the internal drawing timer of a CartesianCanvas object.
     * \details A timer is put to sleep until a subsequent event is ready to occur.
     * \details The drawing timer is put to sleep until the next drawing frame is ready to occur.
     * \note Identical to Canvas::sleep().
     */
    void sleep();

    /*!
     * \brief Zoom the CartesianCanvas with a given center.
     * \details This function will re-center the CartesianCanvas at the given coordinates, then zoom with
     *   respect to the given scale.
     *   \param x The coordinate to re-center the screen on.
     *   \param y The coordinate to re-center the screen on.
     *   \param scale The zoom scale compared to the original. Less than 1 zooms in, greater than 1 zooms out.
     * \note This function will automatically maintain the current aspect ratio.
     */
    void zoom(Decimal x, Decimal y, Decimal scale);

    /*!
     * \brief Zoom the CartesianCanvas with the given bounding (Cartesian) coordinates.
     * \details This function will zoom the CartesianCanvas with respect to the given bounding coordinates.
     *   \param x1 The left Cartesian bound.
     *   \param y1 The bottom Cartesian bound.
     *   \param x2 The right Cartesian bound.
     *   \param y2 The top Cartesian bound.
     * \note Setting the right bound lower than the left bound or the top lower than the bottom will just
     *   swap the variables.
     * \warning This function will *NOT* automatically maintain the previous aspect ratio.
     * \warning Change the aspect ratio on-the-fly only with caution.
     */
    void zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2);

    /*!
     * \brief Runs the Unit tests for CartesianCanvas.
     */
    static void runTests();
};

}

#endif /* CARTESIANCANVAS_H_- */
