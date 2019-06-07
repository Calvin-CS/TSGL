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
 * \brief Canvas extended with Cartesian drawing operations.
 * \details CartesianCanvas provides a Canvas with a Cartesian coordinate system for ease of plotting.
 * \note While on a regular Canvas, pixels higher on the screen have a lower y-value, <b>on a CartesianCanvas,
 *   pixels higher on the screen have a higher y-value.</b>
 */
class CartesianCanvas : public Canvas {
private:
    Decimal cartWidth;                                                  // maxX-minX
    Decimal cartHeight;                                                 // maxY-minY
    Decimal minX, maxX, minY, maxY;                                     // Bounding Cartesian coordinates for the window
    Decimal pixelWidth, pixelHeight;                                    // cartWidth/window.w(), cartHeight/window.h()

    static bool testZoom(CartesianCanvas& can);                         // Unit test for zoom() methods
    static bool testRecomputeDimensions(CartesianCanvas& can);          // Unit test for recomputeDimensions()
    static bool testDraw(CartesianCanvas& can);                         // Unit test for drawing
public:
    CartesianCanvas(double timerLength = 0.0);

    CartesianCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax,
                    std::string t, double timerLength = 0.0);

    void drawAxes(Decimal originX, Decimal originY, Decimal spacingX, Decimal spacingY);

    void drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat color, bool filled = true);

    void drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat color[], bool filled = true);

    void drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor, ColorFloat outlineColor);

    void drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor[], ColorFloat outlineColor);

    void drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor, ColorFloat outlineColor[]);

    void drawCircle(Decimal x, Decimal y, Decimal radius, ColorFloat fillColor[], ColorFloat outlineColor[]);

    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color, bool filled = true);

    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled = true);

    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor);

    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor);

    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor[]);

    void drawConcavePolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor[]);

    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color, bool filled = true);

    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled = true);

    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor);
    
    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor);

    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor[]);

    void drawConvexPolygon(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor[]);

    void drawFunction(const Function &function, float sleepTime = 0.0f, ColorFloat color = BLACK);

    void drawFunction(functionPointer &function, float sleepTime = 0.0f, ColorFloat color = BLACK);

    void drawImage(std::string filename, Decimal x, Decimal y, Decimal w, Decimal h, float a = 1.0f);

    void drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color = BLACK);

    void drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color[]);

    void drawPartialFunction(functionPointer &function, Decimal min, Decimal max,
                             float sleepTime = 0.0f, ColorFloat color = BLACK);

    void drawPixel(Decimal row, Decimal col, ColorFloat color = BLACK);

    void drawPoint(Decimal x, Decimal y, ColorFloat color = BLACK);

    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color = BLACK, bool filled = true);

    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color[], bool filled = true);

    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat fillColor, ColorFloat outlineColor);

    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat fillColor[], ColorFloat outlineColor);

    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat fillColor, ColorFloat outlineColor[]);

    void drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat fillColor[], ColorFloat outlineColor[]);

    void drawText(std::string text, Decimal x, Decimal y, unsigned size, ColorFloat color = BLACK);

    void drawText(std::wstring text, Decimal x, Decimal y, unsigned size, ColorFloat color = BLACK);

    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color = BLACK, bool filled = true);

    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color[], bool filled = true);

    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor, ColorFloat outlineColor);

    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor[], ColorFloat outlineColor);

    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor, ColorFloat outlineColor[]);

    void drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat fillColor[], ColorFloat outlineColor[]);

    void drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat color, bool filled = true);

    void drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat color[], bool filled = true);

    void drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor);

    void drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor);

    void drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor, ColorFloat outlineColor[]);

    void drawTriangleStrip(int size, Decimal xverts[], Decimal yverts[], ColorFloat fillColor[], ColorFloat outlineColor[]);

    void getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY);

    Decimal getCartHeight();

    Decimal getCartWidth();

    Decimal getPixelHeight();

    Decimal getPixelWidth();

    Decimal getMaxX();

    Decimal getMaxY();

    Decimal getMinX();

    Decimal getMinY();

    void getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY);

    void recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax);

    void run(void (*myFunction)(CartesianCanvas&));

    void run(void (*myFunction)(CartesianCanvas&, int), int i);

    void run(void (*myFunction)(CartesianCanvas&, unsigned), unsigned u);

    void run(void (*myFunction)(CartesianCanvas&, int, int), int i1, int i2);

    void run(void (*myFunction)(CartesianCanvas&, unsigned, unsigned), unsigned u1, unsigned u2);

    void run(void (*myFunction)(CartesianCanvas&, std::string),std::string s);

    void run(void (*myFunction)(CartesianCanvas&, int, std::string), int i, std::string s);

    void run(void (*myFunction)(CartesianCanvas&, std::string, int), std::string s, int i);

    void sleep();

    void zoom(Decimal x, Decimal y, Decimal scale);

    void zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2);

    static void runTests();
};

}

#endif /* CARTESIANCANVAS_H_- */
