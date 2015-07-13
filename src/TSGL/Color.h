/*
 * Color.h provides color types and methods of converting between them and generating them.
 *
 * Authors: Patrick Crain, Mark Vander Stel, Chris Dilley.
 * Last Modified: Patrick Crain, 7/30/2014
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>        // Needed for conversions
#include <stdexcept>    // Needed for exceptions
#include <cstdlib>      // Needed for rand()
#include <sstream>      // Needed for Windows integer / float to string conversion

#include "Util.h"       // Clamp()
#include "Error.h"      // TsglErr() / TsglDebug()

namespace tsgl {

struct ColorFloat;
struct ColorInt;
struct ColorHSV;

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
    float R, G, B, A;

    /*!
     * \brief Default ColorFloat constructor method.
     * \details This is the default constructor for the ColorFloat struct.
     * \note R, G, B, and A are all set to 1.0f by default.
     * \return A ColorFloat struct with default R, G, B, and A values.
     */
    ColorFloat();

    /*!
     * \brief Basic explicit ColorFloat constructor method.
     * \details This is the basic explicit constructor for the ColorFloat struct.
     *    \param v The value component of the color.
     *    \param a The alpha component of the struct (set to 1.0f by default).
     * \warning An invariant is set where if any of the specified R, G, B, or A values
     *    is out of the range 0 - 1 inclusive then an error message is given.
     * \return A ColorFloat struct with equal R, G, and B values set to <code>v</code>,
     *    and the specified A value.
     */
    ColorFloat(float v, float a = 1.0f);

    /*!
     * \brief Full explicit ColorFloat constructor method.
     * \details This is the full explicit constructor for the ColorFloat struct.
     *    \param r The red component of the struct.
     *    \param g The green component of the struct.
     *    \param b The blue component of the struct.
     *    \param a The alpha component of the struct (set to 1.0f by default).
     * \warning An invariant is set where if any of the specified R, G, B, or A values
     *    is out of the range 0 - 1 inclusive then an error message is given.
     * \return A ColorFloat struct with the specified R, G, B, and A values.
     */
    ColorFloat(float r, float g, float b, float a = 1.0f);

    /*!
     * \brief Returns a string representation of the ColorFloat.
     * \details This function returns a std::string representation of the ColorFloat.
     * \return A string representation of the ColorFloat.
     */
    std::string asString();

    /*!
     * \brief Implicit conversion from ColorFloat to ColorHSV.
     * \details This defines the implicit conversion operator from a floating point color type (ColorFloat) to an
     *   HSV color type (ColorHSV).
     */
    operator ColorHSV();

    /*!
     * \brief Multiplies the values of a ColorFloat by a float
     * \details This operator multiplies each of the components of a ColorFloat
     *   by amount <code>f</code>.
     * \param f Amount to multiply each component by
     * \returns A new ColorFloat constructed as ColorFloat(orig.R*f, orig.G*f, orig.b*f, orig.A*f)
     */
    ColorFloat operator*(float f);

    /*!
     * \brief Determines if two ColorFloats are equivalent.
     * \details Equality operator for two ColorFloats. Determines if they are equivalent.
     *    \param c2 Reference to the ColorFloat struct that is the second one in the equivalence comparison.
     * \note This function relies on (*this), which is a dereferenced pointer to the first ColorFloat struct in the comparison.
     *    (its the one on the left side of the == sign).
     * \returns true if the two ColorFloats are equivalent, false if otherwise.
     */
    bool operator==(ColorFloat& c2);

    /*!
     * \brief Determines if two ColorFloats are *NOT* equivalent.
     * \details Inequality operator for two ColorFloats. Determines if they are *NOT* equivalent.
     *    \param c2 Reference to the ColorFloat struct that is the second one in the inequality comparison.
     * \note This function relies on (*this), which is a dereferenced pointer to the first ColorFloat struct in the inequality comparison.
     *       (its the one on the left side of the != sign).
     * \returns true if the two ColorFloats are not equivalent, false if otherwise.
     */
    bool operator!=(ColorFloat& c2);
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

    /*!
     * \brief Default ColorInt constructor method.
     * \details This is the default constructor for the ColorInt struct.
     * \note R, G, B, and A are all set to 255 by default.
     * \return A ColorInt struct with default R, G, B, and A values.
     */
    ColorInt();

    /*!
     * \brief Basic explicit ColorInt constructor method.
     * \details This is the basic explicit constructor for the ColorInt struct.
     *   \param v The value component of the color.
     *   \param a The alpha component of the struct (set to 255 by default).
     * \warning An invariant is held where if any of the specified values are out of the
     *   range 0 - 255 inclusive then an error message is given.
     * \return A ColorInt struct with equal R, G, and B values set to <code>v</code>,
     *   and the specified A value.
     */
    ColorInt(int v, int a = 255);

    /*!
     * \brief Full explicit ColorInt constructor method.
     * \details This is the full explicit constructor for the ColorInt struct.
     *   \param r The red component of the ColorInt struct.
     *   \param g The green component of the ColorInt struct.
     *   \param b The blue component of the ColorInt struct.
     *   \param a The alpha component of the ColorInt struct (set to 255 by default).
     * \warning An invariant is held where if any of the specified values are out of the
     *   range 0 - 255 inclusive then an error message is given.
     * \return A ColorInt struct with the specified R, G, B, and A values.
     */
    ColorInt(int r, int g, int b, int a = 255);

    /*!
     * \brief Returns a string representation of the ColorInt.
     * \details This function returns a std::string representation of the ColorInt.
     * \return A string representation of the ColorInt.
     */
    std::string asString();

    /*!
     * \brief Implicit conversion from ColorInt to ColorFloat.
     * \details This defines the implicit conversion operator from an integer color type (ColorInt) to a
     *   floating point color type (ColorFloat).
     */
    operator ColorFloat();

    /*!
     * \brief Implicit conversion from ColorInt to ColorHSV.
     * \details This defines the implicit conversion operator from an integer color type (ColorInt) to an
     *   HSV color type (ColorHSV).
     */
    operator ColorHSV();

    /*!
     * \brief Determines if two ColorInts are equivalent.
     * \details Equality operator for two ColorInts. Determines if they are equivalent.
     *    \param c2 Reference to the ColorInt struct that is the second one in the equivalence comparison.
     * \note This function relies on (*this), which is a dereferenced pointer to the first ColorInt struct in the comparison.
     *    (its the one on the left side of the == sign).
     * \returns true if the two ColorInt are equivalent, false if otherwise.
     */
    bool operator==(ColorInt& c2);

    /*!
     * \brief Determines if two ColorInts are *NOT* equivalent.
     * \details Inequality operator for two ColorInts. Determines if they are *NOT* equivalent.
     *    \param c2 Reference to the ColorInt struct that is the second one in the inequality comparison.
     * \note This function relies on (*this), which is a dereferenced pointer to the first ColorInt struct in the inequality comparison.
     *    (its the one on the left side of the != sign).
     * \returns true if the two ColorInts are not equivalent, false if otherwise.
     */
    bool operator!=(ColorInt& c2);
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

    /*!
     * \brief Constructs a ColorHSV struct.
     * \details Default constructor for a ColorHSV struct.
     * \note H is set to 0.0f by default. S, V, and A are set to 1.0f by default.
     * \return A ColorHSV struct with default H, S, V, and A values.
     */
    ColorHSV();

    /*!
     * \brief Explicitly constructs a ColorHSV struct.
     * \details Explicit constructor for a ColorHSV struct.
     *    \param h Hue component of the ColorHSV struct.
     *    \param s Saturation component of the ColorHSV struct.
     *    \param v Value component of the ColorHSV struct.
     *    \param a Alpha component of the ColorHSV struct (set to 1.0f by default).
     * \warning An invariant is held for each of the components where if any of them are
     *    out of range then an error message is given.
     * \return A ColorHSV struct with specified H, S, V, and A values.
     */
    ColorHSV(float h, float s, float v, float a = 1.0f);

    /*!
     * \brief Implicit conversion from ColorHSV to ColorFloat.
     * \details This defines the implicit conversion operator from an HSV color type (ColorHSV) to an RGB
     *  color type (ColorFloat).
     */
    operator ColorFloat();

    /*!
     * \brief Returns a string representation of the ColorHSV.
     * \details This function returns a std::string representation of the ColorHSV.
     * \return A string representation of the ColorHSV.
     */
    std::string asString();
};

const ColorFloat BLACK = ColorFloat(0.0f, 0.0f, 0.0f, 1.0f),
                 DARKGRAY = ColorFloat(0.5f, 0.5f, 0.5f, 1.0f),
                 GRAY = ColorFloat(0.75f, 0.75f, 0.75f, 1.0f),
                 WHITE = ColorFloat(1.0f, 1.0f, 1.0f, 1.0f),
                 RED = ColorFloat(1.0f, 0.0f, 0.0f, 1.0f),
                 ORANGE = ColorFloat(1.0f, 0.75f, 0.5f, 1.0f),
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
    /*!
     * \brief Returns an HSVA color with a hue dependent on the number of sections.
     * \details This function returns a ColorFloat whose hue is calculated from the provided section number and
     *  the total number of sections.
     *  This function is used for creating a chromatic gradient from one part of the spectrum to another.
     *    \param totalSections Unsigned integer specifying the total number of sections.
     *    \param index Unsigned integer specifying the current section.
     *    \param value Value component, between 0 and 1 inclusive.
     *    \param alpha Alpha component, between 0 and 1 inclusive (set to 1.0f by default).
     * \warning An invariant is held where if value or alpha is greater than 1 or less than 0 then an error message is given.
     * \return A ColorFloat with a hue calculated as 6.0f / sections*section, saturation of 1.0f, and the given value
     *  and alpha components.
     */
    static ColorFloat divideIntoChromaticSections(unsigned int totalSections, unsigned int index, float value, float alpha = 1.0f);

    /*!
     * \brief Returns an HSVA color with a hue dependent on the number of sections.
     * \details This function returns a ColorFloat whose hue is calculated from the provided section number and
     *  the total number of sections.
     *  This function is used for creating a chromatic gradient from one part of the spectrum to another.
     *    \param totalSections Unsigned integer specifying the total number of sections.
     *    \param index Unsigned integer specifying the current section.
     * \return A ColorFloat with a hue calculated as 6.0f / sections*section, and a saturation, value, and alpha of 1.0f.
     */
    static ColorFloat divideIntoChromaticSections(unsigned int totalSections, unsigned int index);

    /*!
     * \brief Generates a random color.
     * \details This function uses rand() to generate a random ColorFloat with an optional specified alpha value.
     *   \param alpha Alpha of the random color to generate. An alpha of 0 will set the alpha to a random
     *     legal value (set to 0.0f by default).
     * \warning An invariant is held for the alpha value where if its greater than 1 or less than 0
     *   then an error message is given.
     * \return A random ColorFloat.
     */
    static ColorFloat randomColor(float alpha = 0.0f);

    /*!
     * \brief Blends two colors with a given bias towards the latter.
     * \details This function blends two ColorFloat structs together by taking a linear interpolation between
     *   the two and returns the result as a new ColorFloat.
     *   \param c1 A ColorFloat.
     *   \param c2 Another ColorFloat.
     *   \param bias A bias between 0 and 1 inclusive.  A bias of 0 returns c1, a bias of 1 returns c2, and a
     *     bias in between returns a linear interpolation.
     * \warning An invariant is held for the bias where if its greater than 1 or less than 0 then
     *   an error message is given.
     * \return A ColorFloat linearly interpolated between c1 and c2 using the given bias as a weight.
     */
    static ColorFloat blendedColor(ColorFloat c1, ColorFloat c2, float bias);

    /*!
     * \brief Returns an HSV color with high contrast.
     * \details This function returns a ColorHSV with hue, saturation, and value calculated to
     *   contrast highly with colors with nearby indices.
     *   \param index Unsigned integer representing the current color index.
     *   \param offset Integer offset for the starting position of the calculation.
     * \return A ColorHSV representing a color visually distinct from its neighbors.
     */
    static ColorFloat highContrastColor(unsigned int index, int offset = 0);

 private:
    Colors();
    ~Colors();
    Colors(const Colors&);
    Colors& operator=(const Colors&);
//    static ColorFloat DISTINCT_ARRAY[64];
    static const ColorFloat* DISTINCT_ARRAY;
};

}

#endif /* COLOR_H_ */
