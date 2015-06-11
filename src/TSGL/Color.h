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

const int NUM_COLORS = 256, MAX_COLOR = 255;

namespace tsgl {

/*!
 * \brief Floating point RGBA color struct.
 * \details ColorFloat defines a color with floating point red, green, blue, and alpha components.
 *      \param R Red component, between 0 and 1 inclusive.
 *      \param G Green component, between 0 and 1 inclusive.
 *      \param B Blue component, between 0 and 1 inclusive.
 *      \param A Alpha component, between 0 and 1 inclusive.
 */
struct ColorFloat {
 public:
    float R, G, B, A;

    /*!
     * \brief Constructs a ColorFloat struct.
     * \details Default constructor for a ColorFloat struct.
     * \note R, G, B, and A are all set to 1.0f by default.
     * \return A ColorFloat struct with default R, G, B, and A values.
     */
    ColorFloat();

    /*!
     * \brief Explicitly constructs a ColorFloat struct.
     * \details Explicit constructor for a ColorFloat struct.
     *      \param r The red component of the struct.
     *      \param g The green component of the struct.
     *      \param b The blue component of the struct.
     *      \param a The alpha component of the struct (set to 1.0f by default).
     * \warning An invariant is set where if any of the specified R, G, B, or A values
     *          is out of the range 0 - 1 inclusive then an std::out_of_range exception
     *          is thrown.
     * \return A ColorFloat struct with the specified R, G, B, and A values.
     */
    ColorFloat(float r, float g, float b, float a = 1.0f);

    /*!
     * \brief Returns a string representation of the ColorFloat.
     * \details This function returns a std::string representation of the ColorFloat.
     * \return A string representation of the ColorFloat.
     */
    std::string AsString();

    /*!
     * NEW (For unit testing)
     */
    ColorFloat operator*(float f);
    bool operator==(ColorFloat& c1);
    bool operator!=(ColorFloat& c1);
};

/*!
 * \brief Integer RGBA color struct.
 * \details ColorInt defines a color with integer red, green, blue, and alpha components.
 *      \param R Red component, between 0 and 255 inclusive.
 *      \param G Green component, between 0 and 255 inclusive.
 *      \param B Blue component, between 0 and 255 inclusive.
 *      \param A Alpha component, between 0 and 255 inclusive.
 */
struct ColorInt {
 public:
    int R, G, B, A;

    /*!
     * \brief Constructs a ColorInt struct.
     * \details Default constructor for the ColorInt struct.
     * \note R, G, B, and A are all set to 255 by default.
     * \return A ColorInt struct with default R, G, B, and A values.
     */
    ColorInt();

    /*!
     * \brief Explicitly create a ColorInt struct.
     * \details Explicit constructor for a ColorInt struct.
     *      \param r The red component of the ColorInt struct.
     *      \param g The green component of the ColorInt struct.
     *      \param b The blue component of the ColorInt struct.
     *      \param a The alpha component of the ColorInt struct (set to 255 by default).
     * \warning An invariant is held where if any of the specified values is out of the
     *          range 0 - 255 inclusive then an std::out_of_range exception is thrown.
     * \return A ColorInt struct with the specified R, G, B, and A values.
     */
    ColorInt(int r, int g, int b, int a = 255);

    /*!
     * \brief Returns a string representation of the ColorInt.
     * \details This function returns a std::string representation of the ColorInt.
     * \return A string representation of the ColorInt.
     */
    std::string AsString();

    /*!
     * \brief Implicit conversion from ColorInt to ColorFloat.
     * \details This defines the implicit conversion operator from an integer color type (ColorInt) to a
     *  floating point color type (ColorFloat).
     */
    operator ColorFloat();

    bool operator==(ColorInt& c1);
    bool operator!=(ColorInt& c1);
};

/*!
 * \brief Floating point HSVA color struct.
 * \details ColorHSV defines a color with floating point hue, saturation, value, and alpha components.
 *      \param H Hue component, between 0 and 6 inclusive.
 *      \param S Saturation component, between 0 and 1 inclusive.
 *      \param V Value component, between 0 and 1 inclusive.
 *      \param A Alpha component, between 0 and 1 inclusive.
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
     *      \param h Hue component of the ColorHSV struct.
     *      \param s Saturation component of the ColorHSV struct.
     *      \param v Value component of the ColorHSV struct.
     *      \param a Alpha component of the ColorHSV struct (set to 1.0f by default).
     * \warning An invariant is held for each of the components where if any of them are
     *          out of range then an std::out_of_range exception.
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
    std::string AsString();
};

/*!
 * \var BLACK
 * \details A ColorFloat constant which represents the color black.
 */
const ColorFloat BLACK = ColorFloat(0.0f, 0.0f, 0.0f, 1.0f);

/*!
 * \var GREY
 * \details A ColorFloat which constant represents the color grey.
 */
const ColorFloat GREY = ColorFloat(0.75f, 0.75f, 0.75f, 1.0f);

/*!
 * \var WHITE
 * \details A ColorFloat constant which represents the color white.
 */
const ColorFloat WHITE = ColorFloat(1.0f, 1.0f, 1.0f, 1.0f);

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
     *      \param sections Integer specifying the total number of sections.
     *      \param section Integer specifying the current section.
     *      \param value Value component, between 0 and 1 inclusive.
     *      \param alpha Alpha component, between 0 and 1 inclusive (set to 1.0f by default).
     * \warning An invariant is held where if value or alpha is greater than 1 or less than 0 then a std::out_of_range
     *          exception is thrown.
     * \return A ColorFloat with a hue calculated as 6.0f / sections*section, saturation of 1.0, and the given value
     *  and alpha components.
     */
    static ColorFloat divideIntoChromaticSections(unsigned int sections, unsigned int section, float value, float alpha = 1.0f);

    /*!
     * \brief Returns an HSVA color with a hue dependent on the number of sections.
     * \details This function returns a ColorFloat whose hue is calculated from the provided section number and
     *  the total number of sections.
     *  This function is used for creating a chromatic gradient from one part of the spectrum to another.
     *      \param sections Integer specifying the total number of sections.
     *      \param section Integer specifying the current section.
     * \return A ColorFloat with a hue calculated as 6.0f / sections*section, and a saturation, value, and alpha of 1.0.
     */
    static ColorFloat divideIntoChromaticSections(unsigned int sections, unsigned int section);

    /*!
     * \brief Generates a random color.
     * \details This function uses rand() to generate a random ColorFloat with an optional specified alpha value.
     *      \param alpha Alpha of the random color to generate. An alpha of 0 will set the alpha to a random
     *       legal value (set to 0.0f by default).
     * \warning An invariant is held for the alpha value where if its greater than 1 or less than 0
     *          then an std::out_of_range exception is thrown.
     * \return A random ColorFloat.
     */
    static ColorFloat randomColor(float alpha = 0.0f);

    /*!
     * \brief Blends two colors with a given bias towards the latter.
     * \details This function blends two ColorFloat structs together by taking a linear interpolation between
     *  the two and returns the result as a new ColorFloat.
     *      \param c1 A ColorFloat.
     *      \param c2 Another ColorFloat.
     *      \param bias A bias between 0 and 1 inclusive.  A bias of 0 returns c1, a bias of 1 returns c2, and a
     *       bias in between returns a linear interpolation.
     * \warning An invariant is held for the bias where if its greater than 1 or less than 0 then
     *          an std::out_of_range exception is thrown.
     * \return A ColorFloat linearly interpolated between c1 and c2 using the given bias as a weight.
     */
    static ColorFloat blendedColor(ColorFloat c1, ColorFloat c2, float bias);

    /*!
     * \brief Returns an HSV color with high contrast.
     * \details This function returns a ColorHSV with hue, saturation, and value all calculated
     *  from the section number and start.
     *      \param section Integer representing the current section.
     *      \param start Integer representing where to start at that section (set to 0 by default).
     * \return A ColorFloat.
     */
    static ColorFloat highContrastColor(unsigned int section, int start = 0);

 private:
    Colors();
    ~Colors();
    Colors(const Colors&);
    Colors & operator=(const Colors&);
};

}

#endif /* COLOR_H_ */
