/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting.
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Canvas.h"     // We extend Canvas

namespace tsgl {

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

    // static bool testZoom(CartesianCanvas& can);                         // Unit test for zoom() methods
    // static bool testRecomputeDimensions(CartesianCanvas& can);          // Unit test for recomputeDimensions()
    // static bool testDraw(CartesianCanvas& can);                         // Unit test for drawing
public:
    CartesianCanvas(double timerLength = 0.0);

    CartesianCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax,
                    std::string t, ColorFloat backgroundColor = GRAY, double timerLength = 0.0);

    virtual CartesianBackground * getBackground() override { return (CartesianBackground *) myBackground; }

    void getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY);

    /*!
     * \brief Accessor for the CartesianCanvas's Cartesian height.
     * \return The Cartesian height of the CartesianCanvas.
     */
    Decimal getCartHeight() { return cartHeight; }

    /*!
     * \brief Accessor for the CartesianCanvas's Cartesian width.
     * \return The Cartesian width of the CartesianCanvas.
     */
    Decimal getCartWidth() { return cartWidth; }

    virtual float getMouseX();

    virtual float getMouseY();

    /*!
     * \brief Accessor for the CartesianCanvas's effective pixel height.
     * \return The height corresponding to a single pixel in the current CartesianCanvas.
     */
    Decimal getPixelHeight() { return pixelHeight; }

    /*!
     * \brief Accessor for the CartesianCanvas's effective pixel width.
     * \return The width corresponding to a single pixel in the current CartesianCanvas.
     */
    Decimal getPixelWidth() { return pixelWidth; }

    /*!
     * \brief Accessor for the CartesianCanvas's right bound.
     * \return The real number corresponding the right of the CartesianCanvas.
     */
    Decimal getMaxX() { return maxX; }

    /*!
     * \brief Accessor for the CartesianCanvas's top bound.
     * \return The real number corresponding the top of the CartesianCanvas.
     */
    Decimal getMaxY() { return maxY; }

    /*!
     * \brief Accessor for the CartesianCanvas's left bound.
     * \return The real number corresponding the left of the CartesianCanvas.
     */
    Decimal getMinX() { return minX; }

    /*!
     * \brief Accessor for the CartesianCanvas's bottom bound.
     * \return The real number corresponding the bottom of the CartesianCanvas.
     */
    Decimal getMinY()  { return minY; }

    void getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY);

    void recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax);

    virtual void selectShaders(unsigned int sType) override;

    void run(void (*myFunction)(CartesianCanvas&));

    void run(void (*myFunction)(CartesianCanvas&, int), int i);

    void run(void (*myFunction)(CartesianCanvas&, unsigned), unsigned u);

    void run(void (*myFunction)(CartesianCanvas&, int, int), int i1, int i2);

    void run(void (*myFunction)(CartesianCanvas&, unsigned, unsigned), unsigned u1, unsigned u2);

    void run(void (*myFunction)(CartesianCanvas&, std::string),std::string s);

    void run(void (*myFunction)(CartesianCanvas&, int, std::string), int i, std::string s);

    void run(void (*myFunction)(CartesianCanvas&, std::string, int), std::string s, int i);

    void zoom(Decimal x, Decimal y, Decimal scale);

    void zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2);

    // static void runTests();
};

}

#endif /* CARTESIANCANVAS_H_- */
