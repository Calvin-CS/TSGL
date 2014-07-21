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

typedef long double Decimal;  // Define the variable type to use for coordinates

class CartesianCanvas : public Canvas {
private:
    Decimal cartWidth,                                                  // maxX-minX
            cartHeight;                                                 // maxY-minY
    Decimal minX, maxX, minY, maxY;                                     // Bounding Cartesian coordinates for the window
    Decimal pixelWidth, pixelHeight;                                    // cartWidth/window.w(), cartHeight/window.h()
public:
    CartesianCanvas(unsigned int b);                                    // Default constructor for our CartesianCanvas
    CartesianCanvas(int xx, int yy, int w, int h, Decimal xMin,
                    Decimal yMin, Decimal xMax, Decimal yMax,
                    unsigned int b, std::string = "");                  // Explicit constructor for our CartesianCanvas

    void    drawAxes(Decimal x, Decimal y, Decimal dx, Decimal dy);     // Draws axes crossing at the input coordinates
    void    drawCircle(Decimal x, Decimal y, Decimal radius, int res,
                RGBfloatType color = BLACK, bool filled = true);
    void    drawFunction(const Function* f);                            // Draws the Function on the screen
    void    drawImage(std::string fname, Decimal x, Decimal y,
                      Decimal w, Decimal h, float a = 1.0f);            // Draws an image at the given coordinates with the given dimensions
    void    drawLine(Decimal x1, Decimal y1, Decimal x2,
                     Decimal y2, RGBfloatType color = BLACK);           // Draws a line at the given coordinates with the given color
    void    drawPoint(Decimal x, Decimal y,
                      RGBfloatType color = BLACK);                      // Draws a point at the given coordinates with the given color
    void    drawRectangle(Decimal x, Decimal y, Decimal w,
                          Decimal h, RGBfloatType color = BLACK,
                          bool filled = true);                          // Draws a rectangle at the given coordinates with the given dimensions and color
    void    drawShinyPolygon(int size, int x[], int y[],
                             RGBfloatType color[], bool filled = true); // Draws an arbitrary polygon with colored vertices
    void    drawText(std::string s, Decimal x, Decimal y,
                     RGBfloatType color = BLACK);                       // Draws a string of text at the given coordinates with the given color
    void    drawTriangle(int x1, int y1, int x2, int y2,
                         int x3, int y3, RGBfloatType color = BLACK,
                         bool filled = true);                           // Draws a triangle with the given vertices and color

    void    getCartesianCoordinates(int screenX, int screenY,
                                    Decimal &cartX, Decimal &cartY);    // Returns the equivalent Cartesian coordinates for the specified screen ones
    Decimal getCartHeight()       { return cartHeight; }                // Accessor for cartHeight
    Decimal getCartWidth()        { return cartWidth; }                 // Accessor for cartWidth
    Decimal getPixelWidth()       { return pixelWidth; }                // Accessor for pixelWidth
    Decimal getPixelHeight()      { return pixelHeight; }               // Accessor for pixelHeight
    Decimal getMaxY()             { return maxY; }                      // Accessor for maxY
    Decimal getMaxX()             { return maxX; }                      // Accessor for maxX
    Decimal getMinX()             { return minX; }                      // Accessor for minX
    Decimal getMinY()             { return minY; }                      // Accessor for minY
    void    getScreenCoordinates(Decimal cartX, Decimal cartY,
                                 int &screenX, int &screenY);           // Returns the equivalent screen coordinates for the specified Cartesian ones

    void    recomputeDimensions(Decimal xMin, Decimal yMin,
                                Decimal xMax, Decimal yMax);            // Recomputes CartesianCanvas' size variables
    void    zoom(Decimal x, Decimal y, Decimal scale);
    void    zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2);
};

#endif /* CARTESIANCANVAS_H_- */
