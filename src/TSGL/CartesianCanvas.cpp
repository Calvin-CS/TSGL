#include "CartesianCanvas.h"

namespace tsgl {

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
CartesianCanvas::CartesianCanvas(double timerLength)
    : Canvas(timerLength) {
    recomputeDimensions(-400, -300, 400, 300);
}

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
CartesianCanvas::CartesianCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax,
                                 Decimal yMax, std::string t, double timerLength)
    : Canvas(x, y, width, height, t, timerLength) {
    recomputeDimensions(xMin, yMin, xMax, yMax);
}

 /*!
  * \brief Draws axes on the Cartesian Canvas.
  * \details This function draws axes (with tick marks) on the CartesianCanvas, centered at the
  *  given (Cartesian) coordinates
  *    \param originX The horizontal location of the y-axis line.
  *    \param originY The vertical location of the x-axis line.
  *    \param spacingX The distance between marks on the x-axis.
  *    \param spacingY The distance between marks on the y-axis.
  */
void CartesianCanvas::drawAxes(Decimal originX, Decimal originY, Decimal spacingX, Decimal spacingY) {
    drawLine(maxX, originY, minX, originY);  // Make the two axes
    drawLine(originX, maxY, originX, minY);

    if (spacingX != 0.0) {
        if (spacingX < 0.0) spacingX = -spacingX;

        for (Decimal x_ = originX + spacingX; x_ < maxX; x_ += spacingX) {
            drawLine(x_, originY + 8 * pixelHeight, x_, originY - 8 * pixelHeight);
        }
        for (Decimal x_ = originX - spacingX; x_ > minX; x_ -= spacingX) {
            drawLine(x_, originY + 8 * pixelHeight, x_, originY - 8 * pixelHeight);
        }
    }
    if (spacingY != 0.0) {
        if (spacingY < 0.0) spacingY = -spacingY;

        for (Decimal y_ = originY + spacingY; y_ < maxY; y_ += spacingY) {
            drawLine(originX + 8 * pixelWidth, y_, originX - 8 * pixelWidth, y_);
        }
        for (Decimal y_ = originY - spacingY; y_ > minY; y_ -= spacingY) {
            drawLine(originX + 8 * pixelWidth, y_, originX - 8 * pixelWidth, y_);
        }
    }
}

 /*!
  * \brief Draws a circle with monocolored fill or outline.
  * \details This function draws a circle with the given center, radius, color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param color The color of the circle
  *   \param filled Whether the circle should be filled
  *     (set to true by default).
  */
void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat color, bool filled) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, color, filled);
}

 /*!
  * \brief Draws a circle with multicolored fill or outline.
  * \details This function draws a circle with the given center, radius, color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param color An array of colors for the circle
  *   \param filled Whether the circle should be filled
  *     (set to true by default).
  */
void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat color[], bool filled) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, color, filled);
}

 /*!
  * \brief Draws a circle with different monocolored fill and outline.
  * \details This function draws a circle with the given center, radius, coloring.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor A color for the circle's fill.
  *   \param outlineColor A color for the circle's outline.
  */
void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor, ColorFloat outlineColor) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, fillColor, outlineColor);
}

 /*!
  * \brief Draws a circle with multicolored fill and monocolored outline.
  * \details This function draws a circle with the given center, radius, coloring.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor An array of colors for the circle's fill.
  *   \param outlineColor A color for the circle's outline.
  */
void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor[], ColorFloat outlineColor) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, fillColor, outlineColor);
}

 /*!
  * \brief Draws a circle with monocolored fill and multicolored outline.
  * \details This function draws a circle with the given center, radius, coloring.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor A color for the circle's fill.
  *   \param outlineColor An array of colors for the circle's outline.
  */
void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor, ColorFloat outlineColor[]) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, fillColor, outlineColor);
}

 /*!
  * \brief Draws a circle with different multicolored fill and outline.
  * \details This function draws a circle with the given center, radius, coloring.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor An array of color for the circle's fill.
  *   \param outlineColor An array of color for the circle's outline.
  */
void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, fillColor, outlineColor);
}

 /*!
  * \brief Draws a Concave polygon with monocolored fill or outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of said vertices.
  *   \param yverts An array of y positions of said vertices.
  *   \param color A color for the said vertices.
  *   \param filled Whether the Concave polygon should be filled in or not
  *     (set to true by default).
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw Convex polygons with this function.
  */
void CartesianCanvas::drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color, bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a Concave polygon with multicolored fill or outline.
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
  */
void CartesianCanvas::drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a Concave polygon with different monocolored fill and outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of said vertices.
  *   \param yverts An array of y positions of said vertices.
  *   \param fillColor A color for the concave polygon's fill.
  *   \param outlineColor A color for the concave polygon's outline.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw Convex polygons with this function.
  */
void CartesianCanvas::drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a Concave polygon with multicolored fill and monocolored outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of said vertices.
  *   \param yverts An array of y positions of said vertices.
  *   \param fillColor An array of colors for the concave polygon's fill.
  *   \param outlineColor A color for the concave polygon's outline.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw Convex polygons with this function.
  */
void CartesianCanvas::drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a Concave polygon with monocolored fill and multicolored outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of said vertices.
  *   \param yverts An array of y positions of said vertices.
  *   \param fillColor A color for the concave polygon's fill.
  *   \param outlineColor An array of colors for the concave polygon's outline.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw Convex polygons with this function.
  */
void CartesianCanvas::drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor[]) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a Concave polygon with different multicolored fill and outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of said vertices.
  *   \param yverts An array of y positions of said vertices.
  *   \param fillColor An array of colors for the concave polygon's fill.
  *   \param outlineColor An array of colors for the concave polygon's outline.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw Convex polygons with this function.
  */
void CartesianCanvas::drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor[]) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a convex polygon with monocolored fill or outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of the x positions of said vertices.
  *   \param yverts An array of the y positions of said vertices.
  *   \param color A color for the said vertices.
  *   \param filled Whether the ConvexPolygon should be filled in or not
  *     (set to true by default).
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void CartesianCanvas::drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color, bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a convex polygon with multicolored fill or outline.
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
  */
void CartesianCanvas::drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a convex polygon with different monocolored fill and outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of the x positions of said vertices.
  *   \param yverts An array of the y positions of said vertices.
  *   \param fillColor A color for the polygon's fill.
  *   \param outlineColor A color for the polygon's outline.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void CartesianCanvas::drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a convex polygon with multicolored fill and monocoloredoutline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of the x positions of said vertices.
  *   \param yverts An array of the y positions of said vertices.
  *   \param fillColor An array of colors for the polygon's fill.
  *   \param outlineColor A color for the polygon's outline.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void CartesianCanvas::drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a convex polygon with monocolored fill and multicolored outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of the x positions of said vertices.
  *   \param yverts An array of the y positions of said vertices.
  *   \param fillColor A color for the polygon's fill.
  *   \param outlineColor An array of colors for the polygon's outline.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void CartesianCanvas::drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor[]) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws a convex polygon with different multicolored fill and outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of the x positions of said vertices.
  *   \param yverts An array of the y positions of said vertices.
  *   \param fillColor An array of colors for the polygon's fill.
  *   \param outlineColor An array of colors for the polygon's outline.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void CartesianCanvas::drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor[]) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Plots a function on the screen.
  * \details This function receives a TSGL Function instance as a parameter and plots the function on the CartesianCanvas.
  *   \param function Reference to the Function to plot.
  *   \param sleepTime Time to sleep between plotting points
  *   \param color The color of the vertices of the plotted function (set to BLACK by default).
  */
void CartesianCanvas::drawFunction(const Function &function, float sleepTime, ColorFloat color) {
  if (sleepTime > 0.0f) {
    bool first = true;
    Decimal lastX, lastY, y;
    for (Decimal x = minX; x < maxX; x += pixelWidth) {
        if (!isOpen()) break;
        y = function.valueAt(x);
        if (!first)
          drawLine(lastX,lastY,x,y,color);
        first = false;
        lastX = x;
        lastY = y;
        if (y > minY && y < maxY)  //Don't waste time if it's offscreen
          sleepFor(sleepTime);
    }
  } else {
    int screenX = 0, screenY = 0;
    int size = (maxX - minX) / pixelWidth;
    Polyline *p = new Polyline(size);
    Decimal x = minX;
    for (int i = 0; i < size; ++i) {
        getScreenCoordinates(x, function.valueAt(x), screenX, screenY);
        p->addVertex(screenX, screenY, color);
        x += pixelWidth;
    }
    drawDrawable(p);
  }
}

 /*!
  * \brief Plots a function on the screen.
  * \details This function receives a pointer to a function method as a parameter and plots the function on
  *   the CartesianCanvas.
  *   \param function Pointer to the function-drawing method to plot.
  *   \param sleepTime Time to sleep between plotting points
  *   \param color The color of the vertices of the plotted function (set to BLACK by default).
  * \note <code>function</code> must receive exactly one Decimal x parameter, and return a Decimal y parameter.
  */
void CartesianCanvas::drawFunction(functionPointer &function, float sleepTime, ColorFloat color) {
  drawPartialFunction(function,minX,maxX,sleepTime,color);
}

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
void CartesianCanvas::drawImage(std::string function, Decimal x, Decimal y, Decimal w, Decimal h, float a) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX1, actualY1);
    getScreenCoordinates(x + w, y - h, actualX2, actualY2);

    Canvas::drawImage(function, actualX1, actualY1, actualX2 - actualX1, actualY2 - actualY1, a);
}

 /*!
  * \brief Draws a monocolored line.
  * \details This function draws a Line at the given coordinates with the given color.
  *   \param x1 The x position of the start of the line.
  *   \param y1 The y position of the start of the line.
  *   \param x2 The x position of the end of the line.
  *   \param y2 The y position of the end of the line.
  *   \param color The color of the line
  *     (set to BLACK by default).
  *  \note Identical to Canvas::drawLine().
  */
void CartesianCanvas::drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);

    Canvas::drawLine(actualX1, actualY1, actualX2, actualY2, color);
}

 /*!
  * \brief Draws a multicolored line.
  * \details This function draws a Line at the given coordinates with the given coloring.
  *   \param x1 The x position of the start of the line.
  *   \param y1 The y position of the start of the line.
  *   \param x2 The x position of the end of the line.
  *   \param y2 The y position of the end of the line.
  *   \param color An array of colors of the line endpoints
  *     (set to BLACK by default).
  *  \note Identical to Canvas::drawLine().
  */
void CartesianCanvas::drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color[]) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);

    Canvas::drawLine(actualX1, actualY1, actualX2, actualY2, color);
}

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
void CartesianCanvas::drawPartialFunction(functionPointer &function, Decimal min, Decimal max, float sleepTime, ColorFloat color) {
  if (sleepTime > 0.0f) {
    bool first = true;
    Decimal lastX, lastY, y;
    for (Decimal x = min; x < max; x += pixelWidth) {
        if (!isOpen()) break;
        y = (function)(x);
        if (!first)
          drawLine(lastX,lastY,x,y,color);
        first = false;
        lastX = x;
        lastY = y;
        if (y > minY && y < maxY)  //Don't waste time if it's offscreen
          sleepFor(sleepTime);
    }
  } else {
    int screenX = 0, screenY = 0;
    int size = 1 + ceil((max - min) / pixelWidth);
    Polyline *p = new Polyline(size);
    Decimal x = min;
    for (int i = 0; i < size; ++i) {
        getScreenCoordinates(x, (function)(x), screenX, screenY);
        p->addVertex(screenX, screenY, color);
        x += pixelWidth;
    }
    drawDrawable(p);
  }
}

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
void CartesianCanvas::drawPixel(Decimal row, Decimal col, ColorFloat color) {
    drawPoint(col, row, color);
}

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
void CartesianCanvas::drawPoint(Decimal x, Decimal y, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    if (atiCard)
      Canvas::drawPoint(actualX, actualY-1, color);
    else
      Canvas::drawPoint(actualX, actualY, color);
}

 /*!
  * \brief Draws a rectangle with monocolored fill or outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's BOTTOM edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param color The color of the rectangle
  *     (set to BLACK by default).
  *   \param filled Whether the rectangle should be filled
  *     (set to true by default).
  * \warning This method's x and y parameters are NOT the same as the x and y of Canvas::drawRectangle.
  */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, ColorFloat color, bool filled) {
    int actualX, actualY, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);
    Canvas::drawRectangle(actualX, actualY, actualX2 - actualX, actualY- actualY2, color, filled);
}

 /*!
  * \brief Draws a rectangle with multicolored fill or outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and coloring.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's BOTTOM edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param color An array of colors for the rectangle
  *     (set to BLACK by default).
  *   \param filled Whether the rectangle should be filled
  *     (set to true by default).
  * \warning This method's x and y parameters are NOT the same as the x and y of Canvas::drawRectangle.
  */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, ColorFloat color[], bool filled) {
    int actualX, actualY, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);
    Canvas::drawRectangle(actualX, actualY, actualX2 - actualX, actualY- actualY2, color, filled);
}

 /*!
  * \brief Draws a rectangle with different monocolored fill and outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and coloring.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's BOTTOM edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor A color for the Rectangle's fill
  *   \param outlineColor A color for the Rectangle's outline
  * \warning This method's x and y parameters are NOT the same as the x and y of Canvas::drawRectangle.
  */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, ColorFloat fillColor, ColorFloat outlineColor) {
    int actualX, actualY, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);
    Canvas::drawRectangle(actualX, actualY, actualX2 - actualX, actualY2 - actualY, fillColor, outlineColor);
}

 /*!
  * \brief Draws a rectangle with multicolored fill and monocolored outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and coloring.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's BOTTOM edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor An array of colors for the Rectangle's fill
  *   \param outlineColor A color for the Rectangle's outline
  * \warning This method's x and y parameters are NOT the same as the x and y of Canvas::drawRectangle.
  */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, ColorFloat fillColor[], ColorFloat outlineColor) {
    int actualX, actualY, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);
    Canvas::drawRectangle(actualX, actualY, actualX2 - actualX, actualY2 - actualY, fillColor, outlineColor);
}

 /*!
  * \brief Draws a rectangle with monocolored fill and multicolored outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and coloring.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's BOTTOM edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor A color for the Rectangle's fill
  *   \param outlineColor An array of colors for the Rectangle's outline
  * \warning This method's x and y parameters are NOT the same as the x and y of Canvas::drawRectangle.
  */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, ColorFloat fillColor, ColorFloat outlineColor[]) {
    int actualX, actualY, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);
    Canvas::drawRectangle(actualX, actualY, actualX2 - actualX, actualY2 - actualY, fillColor, outlineColor);
}

 /*!
  * \brief Draws a rectangle with different multicolored fill and outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and coloring.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's BOTTOM edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor An array of colors for the Rectangle's fill
  *   \param outlineColor An array of colors for the Rectangle's outline
  * \warning This method's x and y parameters are NOT the same as the x and y of Canvas::drawRectangle.
  */
void CartesianCanvas::drawRectangle(Decimal x, Decimal y, Decimal w, Decimal h, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    int actualX, actualY, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x + w, y + h, actualX2, actualY2);
    Canvas::drawRectangle(actualX, actualY, actualX2 - actualX, actualY2 - actualY, fillColor, outlineColor);
}

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
void CartesianCanvas::drawText(std::string text, Decimal x, Decimal y, unsigned size, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawText(text, actualX, actualY, size, color);
}

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
void CartesianCanvas::drawText(std::wstring text, Decimal x, Decimal y, unsigned size, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawText(text, actualX, actualY, size, color);
}

 /*!
  * \brief Draw a triangle with monocolored fill or outline.
  * \details This function draws a Triangle with the given vertices.
  *      \param x1 The x coordinate of the first vertex of the Triangle.
  *      \param y1 The y coordinate of the first vertex of the Triangle.
  *      \param x2 The x coordinate of the second vertex of the Triangle.
  *      \param y2 The y coordinate of the second vertex of the Triangle.
  *      \param x3 The x coordinate of the third vertex of the Triangle.
  *      \param y3 The y coordinate of the third vertex of the Triangle.
  *      \param color A color for the Triangle (set to BLACK by default).
  *      \param filled Whether the Triangle should be filled (set to true by default).
  */
void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color, bool filled) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, color, filled);
}

 /*!
  * \brief Draw a triangle with multicolored fill or outline.
  * \details This function draws a Triangle with the given vertices.
  *      \param x1 The x coordinate of the first vertex of the Triangle.
  *      \param y1 The y coordinate of the first vertex of the Triangle.
  *      \param x2 The x coordinate of the second vertex of the Triangle.
  *      \param y2 The y coordinate of the second vertex of the Triangle.
  *      \param x3 The x coordinate of the third vertex of the Triangle.
  *      \param y3 The y coordinate of the third vertex of the Triangle.
  *      \param color An array of colors for the Triangle.
  *      \param filled Whether the Triangle should be filled (set to true by default).
  */
void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color[], bool filled) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, color, filled);
}

 /*!
  * \brief Draw a triangle with different monocolored fill and outline.
  * \details This function draws a Triangle with the given vertices.
  *      \param x1 The x coordinate of the first vertex of the Triangle.
  *      \param y1 The y coordinate of the first vertex of the Triangle.
  *      \param x2 The x coordinate of the second vertex of the Triangle.
  *      \param y2 The y coordinate of the second vertex of the Triangle.
  *      \param x3 The x coordinate of the third vertex of the Triangle.
  *      \param y3 The y coordinate of the third vertex of the Triangle.
  *      \param fillColor A color for the Triangle's fill.
  *      \param outlineColor A color for the Triangle's outline.
  */
void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor, ColorFloat outlineColor) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, fillColor, outlineColor);
}

 /*!
  * \brief Draw a triangle with multicolored fill and monocolored outline.
  * \details This function draws a Triangle with the given vertices.
  *      \param x1 The x coordinate of the first vertex of the Triangle.
  *      \param y1 The y coordinate of the first vertex of the Triangle.
  *      \param x2 The x coordinate of the second vertex of the Triangle.
  *      \param y2 The y coordinate of the second vertex of the Triangle.
  *      \param x3 The x coordinate of the third vertex of the Triangle.
  *      \param y3 The y coordinate of the third vertex of the Triangle.
  *      \param fillColor An array of colors for the Triangle's fill.
  *      \param outlineColor A color for the Triangle's outline.
  */
void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor[], ColorFloat outlineColor) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, fillColor, outlineColor);
}

 /*!
  * \brief Draw a triangle with monocolored fill and multicolored outline.
  * \details This function draws a Triangle with the given vertices.
  *      \param x1 The x coordinate of the first vertex of the Triangle.
  *      \param y1 The y coordinate of the first vertex of the Triangle.
  *      \param x2 The x coordinate of the second vertex of the Triangle.
  *      \param y2 The y coordinate of the second vertex of the Triangle.
  *      \param x3 The x coordinate of the third vertex of the Triangle.
  *      \param y3 The y coordinate of the third vertex of the Triangle.
  *      \param fillColor A color for the Triangle's fill.
  *      \param outlineColor An array of colors for the Triangle's outline.
  */
void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor, ColorFloat outlineColor[]) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, fillColor, outlineColor);
}

 /*!
  * \brief Draw a triangle with different multicolored fill and outline.
  * \details This function draws a Triangle with the given vertices.
  *      \param x1 The x coordinate of the first vertex of the Triangle.
  *      \param y1 The y coordinate of the first vertex of the Triangle.
  *      \param x2 The x coordinate of the second vertex of the Triangle.
  *      \param y2 The y coordinate of the second vertex of the Triangle.
  *      \param x3 The x coordinate of the third vertex of the Triangle.
  *      \param y3 The y coordinate of the third vertex of the Triangle.
  *      \param fillColor An array of colors for the Triangle's fill.
  *      \param outlineColor An array of colors for the Triangle's outline.
  */
void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, fillColor, outlineColor);
}

 /*!
  * \brief Draws an arbitrary triangle strip with monocolored fill or outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of the vertices.
  *   \param yverts An array of y positions of the vertices.
  *   \param color A color for the vertices.
  *   \param filled Whether the triangle strip should be filled (true) or not (false)
  *     (set to true by default).
  */
void CartesianCanvas::drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat color, bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawTriangleStrip(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws an arbitrary triangle strip with multicolored fill or outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of the vertices.
  *   \param yverts An array of y positions of the vertices.
  *   \param color An array of colors for the vertices.
  *   \param filled Whether the triangle strip should be filled (true) or not (false)
  *     (set to true by default).
  */
void CartesianCanvas::drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawTriangleStrip(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws an arbitrary triangle strip with different monocolored fill and outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of the vertices.
  *   \param yverts An array of y positions of the vertices.
  *   \param fillColor A color for the triangle strip's fill.
  *   \param outlineColor A color for the triangle strip's outline.
  */
void CartesianCanvas::drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawTriangleStrip(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws an arbitrary triangle strip with multicolored fill and monocolored outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of the vertices.
  *   \param yverts An array of y positions of the vertices.
  *   \param fillColor An array of colors for the triangle strip's fill.
  *   \param outlineColor A color for the triangle strip's outline.
  */
void CartesianCanvas::drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawTriangleStrip(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws an arbitrary triangle strip with monocolored fill and multicolored outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of the vertices.
  *   \param yverts An array of y positions of the vertices.
  *   \param fillColor A color for the triangle strip's fill.
  *   \param outlineColor An array of colors for the triangle strip's outline.
  */
void CartesianCanvas::drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor[]) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawTriangleStrip(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Draws an arbitrary triangle strip with different multicolored fill and outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param xverts An array of x positions of the vertices.
  *   \param yverts An array of y positions of the vertices.
  *   \param fillColor An array of colors for the triangle strip's fill.
  *   \param outlineColor An array of colors for the triangle strip's outline.
  */
void CartesianCanvas::drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor[]) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(xverts[i], yverts[i], int_x[i], int_y[i]);
    }
    Canvas::drawTriangleStrip(size, int_x, int_y, fillColor, outlineColor);

    delete int_x;
    delete int_y;
}

 /*!
  * \brief Translates Cartesian coordinates into window coordinates.
  * \details getCartesianCoordinates() takes a pair of on-screen coordinates and translates them to Cartesian
  *  coordinates.
  *    \param screenX The window's x coordinate.
  *    \param screenY The window's y coordinate.
  *    \param cartX A reference variable to be filled with screenX's Cartesian position.
  *    \param cartY A reference variable to be filled with screenY's Cartesian position.
  */
void CartesianCanvas::getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY) {
    cartX = (screenX * cartWidth) / getWindowWidth() + minX;
    cartY = minY - (screenY - getWindowHeight()) * cartHeight / getWindowHeight();
}

 /*!
  * \brief Accessor for the CartesianCanvas's Cartesian height.
  * \return The Cartesian height of the CartesianCanvas.
  */
Decimal CartesianCanvas::getCartHeight() {
    return cartHeight;
}

 /*!
  * \brief Accessor for the CartesianCanvas's Cartesian width.
  * \return The Cartesian width of the CartesianCanvas.
  */
Decimal CartesianCanvas::getCartWidth() {
    return cartWidth;
}

 /*!
  * \brief Accessor for the CartesianCanvas's effective pixel height.
  * \return The height corresponding to a single pixel in the current CartesianCanvas.
  */
Decimal CartesianCanvas::getPixelHeight() {
    return pixelHeight;
}

 /*!
  * \brief Accessor for the CartesianCanvas's effective pixel width.
  * \return The width corresponding to a single pixel in the current CartesianCanvas.
  */
Decimal CartesianCanvas::getPixelWidth() {
    return pixelWidth;
}

 /*!
  * \brief Accessor for the CartesianCanvas's right bound.
  * \return The real number corresponding the right of the CartesianCanvas.
  */
Decimal CartesianCanvas::getMaxX() {
    return maxX;
}

 /*!
  * \brief Accessor for the CartesianCanvas's top bound.
  * \return The real number corresponding the top of the CartesianCanvas.
  */
Decimal CartesianCanvas::getMaxY() {
    return maxY;
}

 /*!
  * \brief Accessor for the CartesianCanvas's left bound.
  * \return The real number corresponding the left of the CartesianCanvas.
  */
Decimal CartesianCanvas::getMinX() {
    return minX;
}

 /*!
  * \brief Accessor for the CartesianCanvas's bottom bound.
  * \return The real number corresponding the bottom of the CartesianCanvas.
  */
Decimal CartesianCanvas::getMinY() {
    return minY;
}

 /*!
  * \brief Translates window coordinates into Cartesian coordinates.
  * \details getScreenCoordinates() takes a pair of Cartesian coordinates and translates them to on-screen
  *   coordinates.
  *   \param cartX The Cartesian x coordinate.
  *   \param cartY The Cartesian y coordinate.
  *   \param screenX A reference variable to be filled with cartX's window position.
  *   \param screenY A reference variable to be filled with cartY's window position.
  */
void CartesianCanvas::getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY) {
    screenX = round((cartX - minX) / pixelWidth);
    if (atiCard)
      screenY = getWindowHeight() - round((cartY - minY) / pixelHeight + pixelHeight*0.5f);
    else
      screenY = getWindowHeight() - 1 - round((cartY - minY) / pixelHeight + pixelHeight*0.5f);
}

 /*!
  * \brief Recomputes the CartesianCanvas's bounds.
  * \details This function recomputes the size variables of CartesianCanvas according to new bounds.
  *   \param xMin A real number corresponding to the new left edge of the CartesianCanvas.
  *   \param YMin A real number corresponding to the new bottom edge of the CartesianCanvas.
  *   \param xMax A real number corresponding to the new right edge of the CartesianCanvas.
  *   \param xMax A real number corresponding to the new top edge of the CartesianCanvas.
  */
void CartesianCanvas::recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax) {
    minX = xMin;
    minY = yMin;
    maxX = xMax;
    maxY = yMax;
    cartWidth = maxX - minX;
    cartHeight = maxY - minY;
    pixelWidth = cartWidth / (getWindowWidth() - 1);
    pixelHeight = cartHeight / (getWindowHeight() - 1);  //Minor hacky fix
}

 /*!
  * \brief Start the CartesianCanvas, run a function on it, and wait for the user to close it
  * \details This function binds another function to the current CartesianCanvas, waits until that function is
  *   complete, and waits for the user to close the CartesianCanvas.  This function effectively calls start(),
  *   <code>myFunction</code>(), and wait() in sequence.
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&) ) {
  start(); myFunction(*this); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param i An integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, int), int i) {
  start(); myFunction(*this, i); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param u An unsigned integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, unsigned), unsigned u) {
  start(); myFunction(*this, u); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param i1 An integer argument to myFunction
  * \param i2 An integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, int, int), int i1, int i2) {
  start(); myFunction(*this, i1, i2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param u1 An unsigned integer argument to myFunction
  * \param u2 An unsigned integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, unsigned, unsigned), unsigned u1, unsigned u2) {
  start(); myFunction(*this, u1, u2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param s A string argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, std::string),std::string s) {
  start(); myFunction(*this, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param i An integer argument to myFunction
  * \param s A string argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, int, std::string), int i, std::string s) {
  start(); myFunction(*this, i, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param s A string argument to myFunction
  * \param i An integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, std::string, int), std::string s, int i) {
  start(); myFunction(*this, s, i); wait();
}

 /*!
  * \brief Sleeps the internal drawing timer of a CartesianCanvas object.
  * \details A timer is put to sleep until a subsequent event is ready to occur.
  * \details The drawing timer is put to sleep until the next drawing frame is ready to occur.
  * \note Identical to Canvas::sleep().
  */
void CartesianCanvas::sleep() {
    Canvas::sleep();
}

 /*!
  * \brief Zoom the CartesianCanvas with a given center.
  * \details This function will re-center the CartesianCanvas at the given coordinates, then zoom with
  *   respect to the given scale.
  *   \param x The coordinate to re-center the screen on.
  *   \param y The coordinate to re-center the screen on.
  *   \param scale The zoom scale compared to the original. Less than 1 zooms in, greater than 1 zooms out.
  * \note This function will automatically maintain the current aspect ratio.
  */
void CartesianCanvas::zoom(Decimal x, Decimal y, Decimal scale) {
    Decimal newWidth = cartWidth * scale;
    Decimal newHeight = cartHeight * scale;
    recomputeDimensions(x - .5 * newWidth, y - .5 * newHeight, x + .5 * newWidth, y + .5 * newHeight);
}

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
void CartesianCanvas::zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2) {
    Decimal scale = (std::abs(x2 - x1) / cartWidth + std::abs(y2 - y1) / cartHeight) / 2.0;
    zoom((x2 + x1) / 2, (y2 + y1) / 2, scale);
}

//-----------------------Unit testing-------------------------------------------------
 /*!
  * \brief Runs the Unit tests for CartesianCanvas.
  */
void CartesianCanvas::runTests() {
  TsglDebug("Testing CartesianCanvas class...");
  CartesianCanvas c1(0.0f);
  c1.setBackgroundColor(WHITE);
  c1.start();

  std::this_thread::sleep_for(std::chrono::seconds(1));
  tsglAssert(testZoom(c1), "Unit test for zoom() functions failed!");
  tsglAssert(testRecomputeDimensions(c1), "Unit test for recomputing dimensions failed!");
  c1.stop();

  CartesianCanvas c2(-1, -1, 800, 600, -1, -1, 3, 2,"");
  c2.setBackgroundColor(WHITE);
  c2.start();
  tsglAssert(testDraw(c2), "Unit test for drawing functions failed!");
  c2.wait();

  TsglDebug("Unit tests for CartesianCanvas complete.");
}

bool CartesianCanvas::testDraw(CartesianCanvas& can) {
  int passed = 0;
  int failed = 0;

  float pw = can.getPixelWidth();
  float ph = can.getPixelHeight();

  //Test 1: Physical to Cartesian point mapping
  can.drawPoint(-1.0f,-1.0f,BLACK); //outer bottomleft
  can.drawPoint(3.0f,-1.0f,BLACK);  //outer bottomright
  can.drawPoint(-1.0f,2.0f,BLACK);  //outer topleft
  can.drawPoint(3.0f,2.0f,BLACK);   //outer topright
  can.drawPoint(0.0f,0.0f,BLACK);   //1/4 over, 1/3 down (origin)

  can.drawPoint(-1.0f+pw,-1.0f+ph,BLACK); //inner bottomleft
  can.drawPoint(3.0f-pw,-1.0f+ph,BLACK);  //inner bottomright
  can.drawPoint(-1.0f+pw,2.0f-ph,BLACK);  //inner topleft
  can.drawPoint(3.0f-pw,2.0f-ph,BLACK);   //inner topright
  can.sleepFor(1.0f);

  if(can.getPoint(200,399).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, origin pixel for testDraw() failed!");
  }
  if(can.getPoint(0,0).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, outer topleft pixel for testDraw() failed!");
  }
  if(can.getPoint(799,0).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, outer topright pixel for testDraw() failed!");
  }
  if(can.getPoint(0,599).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, outer bottomleft pixel for testDraw() failed!");
  }
  if(can.getPoint(799,599).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, outer bottomright pixel for testDraw() failed!");
  }

  if(can.getPoint(1,1).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, inner topleft pixel for testDraw() failed!");
  }
  if(can.getPoint(798,1).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, inner topright pixel for testDraw() failed!");
  }
  if(can.getPoint(1,598).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, inner bottomleft pixel for testDraw() failed!");
  }
  if(can.getPoint(798,598).R == 0) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, inner bottomright pixel for testDraw() failed!");
  }

  //Results:
  if(failed == 0) {
    TsglDebug("Unit test for drawing passed!");
    return true;
  } else {
    TsglErr("This many passed for testDraw(): ");
    std::cout << " " << passed << std::endl;
    TsglErr("This many failed for testDraw(): ");
    std::cout << " " << failed << std::endl;
    return false;
  }
}

bool CartesianCanvas::testZoom(CartesianCanvas& can) {
    int passed = 0;
    int failed = 0;
    //Test 1: Zooming out
    //Had to use round() because there was a floating-point error that
    //propagated to the rest of the tests after the first two
    can.zoom(0, 0, 1.5);
    if(round(can.getCartWidth()) == 1200 && round(can.getCartHeight()) == 900) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, Zooming out for testZoom() failed!");
    }

    //Test 2: Zooming in
    can.zoom(0, 0, .5);
    if(round(can.getCartWidth()) == 600 && round(can.getCartHeight()) == 450) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Zooming in for testZoom() failed!");
    }

    //Test 3: Zooming out/in on a different point

    //Zooming out....
    can.zoom(10, 10, 1.2);
    if(round(can.getCartWidth()) == 720 && round(can.getCartHeight()) == 540) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Zooming out on a different point for testZoom() failed!");
    }

    //Zooming in....
    can.zoom(15, 20, .9);
    if(round(can.getCartWidth()) == 648 && round(can.getCartHeight()) == 486) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Zooming in on a different point for testZoom() failed!");
    }

    //Results:
    if(passed == 4 && failed == 0) {
      TsglDebug("Unit test for zooming in & out passed!");
      return true;
    } else {
      TsglErr("This many passed for testZoom(): ");
      std::cout << " " << passed << std::endl;
      TsglErr("This many failed for testZoom(): ");
      std::cout << " " << failed << std::endl;
      return false;
    }
}

bool CartesianCanvas::testRecomputeDimensions(CartesianCanvas& can) {
   int passed = 0;
   int failed = 0;
   Decimal xMin, xMax;
   Decimal yMin, yMax;
   //Test 1: Positive values only (with 0.0)
   xMin = 0.0;
   xMax = 500.0;
   yMin = 0.0;
   yMax = 500.0;
   can.recomputeDimensions(xMin, yMin, xMax, yMax);
   if(can.getCartWidth() == 500.0 && can.getCartHeight() == 500.0) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 1, Positive values only for testRecomputeDimensions() failed!");
   }

   //Test 2: Negative values included
   xMin = xMax = yMin = yMax = 0.0;
   xMin = -300.0;
   xMax = 900.0;
   yMin = -500.0;
   yMax = 1000.0;
   can.recomputeDimensions(xMin, yMin, xMax, yMax);

   if(can.getCartWidth() == 1200.0 && can.getCartHeight() == 1500.0) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 2, Negative values for testRecomputeDimensions() failed!");
   }

   //Test 3: Same as Test 2, but negative values are max
   xMin = xMax = yMin = yMax = 0.0;
   xMin = -900.0;
   xMax = -100.0;
   yMin = -800.0;
   yMax = -50.0;
   can.recomputeDimensions(xMin, yMin, xMax, yMax);

   if(can.getCartWidth() == 800.0 && can.getCartHeight() == 750.0) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 3, Max negative values for testRecomputeDimensions() failed!");
   }

   if(passed == 3 && failed == 0) {
     TsglDebug("Unit test for recomputing dimensions passed!");
     return true;
   } else {
     TsglErr("This many tests passed for testRecomputeDimensions(): ");
     std::cout << " " << passed << std::endl;
     TsglErr("This many tests failed for testRecomputeDimensions(): ");
     std::cout << " " << failed << std::endl;
     return false;
   }
}
//-----------------End Unit testing----------------------------------------------------
}
