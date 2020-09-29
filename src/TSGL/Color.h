/*
 * Color.h provides color types and methods of converting between them and generating them.
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>        // Needed for conversions
#include <stdexcept>    // Needed for exceptions
#include <cstdlib>      // Needed for rand()
#include <sstream>      // Needed for Windows integer / float to string conversion
#include <gl_includes.h>

#include "Util.h"       // Clamp()
#include "Error.h"      // TsglErr() / TsglDebug()

namespace tsgl {

struct ColorFloat;  //Forward declarations
struct ColorInt;
struct ColorHSV;
struct ColorFloat;

/*!
 * \brief Floating point RGBA color struct.
 * \details ColorFloat defines a color with floating point red, green, blue, and alpha components.
 *    \param R Red component, between 0 and 1 inclusive.
 *    \param G Green component, between 0 and 1 inclusive.
 *    \param B Blue component, between 0 and 1 inclusive.
 *    \param A Alpha component, between 0 and 1 inclusive.
 */
struct ColorFloat {
 public:
    GLfloat R, G, B, A;

    ColorFloat();

    ColorFloat(GLfloat v, GLfloat a = 1.0f);

    ColorFloat(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

    std::string asString();

    operator ColorHSV();

    operator ColorInt();

    ColorFloat operator*(float f);

    bool operator==(ColorFloat& c2);

    bool operator!=(ColorFloat& c2);

    ColorFloat getContrast();
};

/*!
 * \brief Integer RGBA color struct.
 * \details ColorInt defines a color with integer red, green, blue, and alpha components.
 *    \param R Red component, between 0 and 255 inclusive.
 *    \param G Green component, between 0 and 255 inclusive.
 *    \param B Blue component, between 0 and 255 inclusive.
 *    \param A Alpha component, between 0 and 255 inclusive.
 */
struct ColorInt {
 public:
    int R, G, B, A;

    ColorInt();

    ColorInt(int v, int a = 255);

    ColorInt(int r, int g, int b, int a = 255);

    std::string asString();

    operator ColorFloat();

    operator ColorHSV();

    bool operator==(ColorInt& c2);

    bool operator!=(ColorInt& c2);

    ColorInt operator*(float f);
};

/*!
 * \brief Floating point HSVA color struct.
 * \details ColorHSV defines a color with floating point hue, saturation, value, and alpha components.
 *    \param H Hue component, between 0 and 6 inclusive.
 *    \param S Saturation component, between 0 and 1 inclusive.
 *    \param V Value component, between 0 and 1 inclusive.
 *    \param A Alpha component, between 0 and 1 inclusive.
 */
struct ColorHSV {
 public:
    float H, S, V, A;

    ColorHSV();

    ColorHSV(float h, float s, float v, float a = 1.0f);

    operator ColorInt();

    operator ColorFloat();

    std::string asString();
};

/*!
 * \brief The various ColorFloat constants used throughout TSGL.
 */
const ColorFloat BLACK = ColorFloat(0.0f, 0.0f, 0.0f, 1.0f),
                 DARKGRAY = ColorFloat(0.5f, 0.5f, 0.5f, 1.0f),
                 GRAY = ColorFloat(0.75f, 0.75f, 0.75f, 1.0f),
                 WHITE = ColorFloat(1.0f, 1.0f, 1.0f, 1.0f),
                 RED = ColorFloat(1.0f, 0.0f, 0.0f, 1.0f),
                 ORANGE = ColorFloat(1.0f, 0.65f, 0.0f, 1.0f),
                 YELLOW = ColorFloat(1.0f, 1.0f, 0.0f, 1.0f),
                 GREEN = ColorFloat(0.0f, 1.0f, 0.0f, 1.0f),
                 BLUE = ColorFloat(0.0f, 0.0f, 1.0f, 1.0f),
                 PURPLE = ColorFloat(0.75f, 0.0f, 0.75f, 1.0f),
                 MAGENTA = ColorFloat(1.0f, 0.0f, 1.0f, 1.0f),
                 LIME = ColorFloat(0.5f, 1.0f, 0.5f, 1.0f),
                 CYAN = ColorFloat(0.0f, 1.0f, 1.0f, 1.0f),
                 BROWN = ColorFloat(0.5f, 0.3f, 0.0f, 1.0f);

/*!
 * \brief Color utility class.
 * \details Colors defines color utility methods to construct colors.
 */
class Colors {
 public:

    static ColorFloat divideIntoChromaticSections(unsigned int totalSections, unsigned int index, float value, float alpha = 1.0f);

    static ColorFloat divideIntoChromaticSections(unsigned int totalSections, unsigned int index);

    static ColorFloat randomColor(float alpha = 0.0f);

    static ColorFloat blend(ColorFloat c1, ColorFloat c2, float bias = 0.5f);

    static ColorFloat highContrastColor(unsigned int index, int offset = 0);

 private:
    Colors();
    ~Colors();
    Colors(const Colors&);
    Colors& operator=(const Colors&);
    static const ColorFloat* DISTINCT_ARRAY;
};

}

#endif /* COLOR_H_ */
