/*
 * CartesianBackground.h extends Drawable and provides a class for drawing TSGL primitives procedurally onto a cartesian background.
 */

#ifndef CARTESIAN_BACKGROUND_H_
#define CARTESIAN_BACKGROUND_H_

#include "Background.h"
#include "Function.h"   // For drawing math functions on the screen
#include "Util.h"

namespace tsgl {

typedef Decimal (*functionPointer)(Decimal x);

/*! \class CartesianBackground
 *  \brief Draw a procedurally drawn CartesianBackground for the Canvas.
 *  \details CartesianBackground is a class for holding a framebuffer of color data.
 */
class CartesianBackground : public Background {
protected:
    Decimal myCartWidth, myCartHeight;
    Decimal myXMin, myXMax, myYMin, myYMax;
    Decimal pixelWidth, pixelHeight;                                    // cartWidth/window.w(), cartHeight/window.h()

    virtual void selectShaders(unsigned int sType) override;
public:
    CartesianBackground(GLint width, GLint height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax, const ColorFloat &c = WHITE);

    virtual void draw();

    virtual void drawAxes(Decimal originX, Decimal originY, Decimal spacingX, Decimal spacingY);

    void drawFunction(const Function &function, ColorFloat color = BLACK);

    void drawFunction(functionPointer &function, ColorFloat color = BLACK);

    void drawPartialFunction(functionPointer &function, Decimal min, Decimal max, ColorFloat color = BLACK);

    virtual void drawPixel(float x, float y, ColorInt c);
                             
    /*!
     * \brief Accessor for the CartesianBackground's Cartesian height.
     * \return The Cartesian height of the CartesianBackground.
     */
    Decimal getCartHeight() { return myCartHeight; }

    /*!
     * \brief Accessor for the CartesianBackground's Cartesian width.
     * \return The Cartesian width of the CartesianBackground.
     */
    Decimal getCartWidth() { return myCartWidth; }

    /*!
     * \brief Accessor for the CartesianBackground's right bound.
     * \return The real number corresponding the right of the CartesianBackground.
     */
    Decimal getMaxX() { return myXMax; }

    /*!
     * \brief Accessor for the CartesianBackground's top bound.
     * \return The real number corresponding the top of the CartesianBackground.
     */
    Decimal getMaxY() { return myYMax; }

    /*!
     * \brief Accessor for the CartesianBackground's left bound.
     * \return The real number corresponding the left of the CartesianBackground.
     */
    Decimal getMinX() { return myXMin; }

    /*!
     * \brief Accessor for the CartesianBackground's bottom bound.
     * \return The real number corresponding the bottom of the CartesianBackground.
     */
    Decimal getMinY()  { return myYMin; }

    virtual ColorInt getPixel(float x, float y);

    void zoom(Decimal x, Decimal y, Decimal scale);

    void zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2);
};

}

#endif /* CARTESIAN_BACKGROUND_H_ */