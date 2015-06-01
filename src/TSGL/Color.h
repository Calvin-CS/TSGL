/*
 * Color.h provides color types and methods of converting between them and generating them
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/30/2014
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>        // Needed for conversions
#include <stdexcept>    // Needed for exceptions
#include <cstdlib>      // Needed for rand()
#include <sstream>      // Needed for Windows integer / float to string conversion

const int NUM_COLORS = 256, MAX_COLOR = 255;

/*!
 * \brief Floating point RGBA color struct.
 * \details ColorFloat defines a color with floating point red, green, blue, and alpha components
 *      \param R Red component, between 0 and 1 inclusive.
 *      \param G Green component, between 0 and 1 inclusive.
 *      \param B Blue component, between 0 and 1 inclusive.
 *      \param A Alpha component, between 0 and 1 inclusive.
 */
struct ColorFloat {
 public:
    float R, G, B, A;

    /*!
     *
     */
    ColorFloat();

    /*!
     *
     */
    ColorFloat(float r, float g, float b, float a = 1.0f);

    /*!
     * \brief Returns a string representation of the ColorFloat
     * \details This function returns a std::string representation of the ColorFloat.
     * \return A string representation of the ColorFloat.
     */
    std::string AsString();
};

/*!
 * \brief Integer RGBA color struct.
 * \details ColorInt defines a color with integer red, green, blue, and alpha components
 *      \param R Red component, between 0 and 255 inclusive.
 *      \param G Green component, between 0 and 255 inclusive.
 *      \param B Blue component, between 0 and 255 inclusive.
 *      \param A Alpha component, between 0 and 255 inclusive.
 */
struct ColorInt {
 public:
    int R, G, B, A;

    /*!
     *
     */
    ColorInt();

    /*!
     *
     */
    ColorInt(int r, int g, int b, int a = 255);

    /*!
     * \brief Returns a string representation of the ColorInt
     * \details This function returns a std::string representation of the ColorInt.
     * \return A string representation of the ColorInt.
     */
    std::string AsString();

    /*!
     * \brief Implicit conversion from ColorInt to ColorFloat
     * \details This defines the implicit conversion operator from an integer color type (ColorInt) to a
     *  floating point color type (ColorFloat)
     */
    operator ColorFloat();
};

/*!
 * \brief Floating pont HSVA color struct.
 * \details ColorHSV defines a color with floating point hue, saturation, value, and alpha components
 *      \param H Hue component, between 0 and 6 inclusive.
 *      \param S Saturation component, between 0 and 1 inclusive.
 *      \param V Value component, between 0 and 1 inclusive.
 *      \param A Alpha component, between 0 and 1 inclusive.
 */
struct ColorHSV {
 public:
    float H, S, V, A;

    /*!
     *
     */
    ColorHSV();

    /*!
     *
     */
    ColorHSV(float h, float s, float v, float a = 1.0f);

    /*!
     * \brief Implicit conversion from ColorHSV to ColorFloat
     * \details This defines the implicit conversion operator from an HSV color type (ColorHSV) to an RGB
     *  color type (ColorFloat)
     */
    operator ColorFloat();

    /*!
     * \brief Returns a string representation of the ColorHSV
     * \details This function returns a std::string representation of the ColorHSV.
     * \return A string representation of the ColorHSV.
     */
    std::string AsString();
};

const ColorFloat BLACK = ColorFloat(0.0f, 0.0f, 0.0f, 1.0f);
const ColorFloat GREY = ColorFloat(0.75f, 0.75f, 0.75f, 1.0f);
const ColorFloat WHITE = ColorFloat(1.0f, 1.0f, 1.0f, 1.0f);

/*!
 * \brief Color utility class.
 * \details Colors defines color utility methods to construct colors.
 */
class Colors {
 public:
    /*!
     * \brief Returns an HSVA color with a hue dependent on the number of sections
     * \details This function returns a ColorFloat whose hue is calculated from the provided section number and
     *  the total number of sections.
     *  This function is used for creating a chromatic gradient from one part of the spectrum to another.
     *      \param section Integer specifying the current section.
     *      \param sections Integer specifying the total number of sections.
     *      \param value Value component, between 0 and 1 inclusive.
     *      \param alpha Alpha component, between 0 and 1 inclusive.
     * \return A ColorFloat with a hue calculated as 6.0f/sections*section, saturation of 1.0, and the given value
     *  and alpha components.
     */
    static ColorFloat divideIntoChromaticSections(unsigned int sections, unsigned int section, float value, float alpha = 1.0f);

    /*!
     * \brief Returns an HSVA color with a hue dependent on the number of sections
     * \details This function returns a ColorFloat whose hue is calculated from the provided section number and
     *  the total number of sections.
     *  This function is used for creating a chromatic gradient from one part of the spectrum to another.
     *      \param section Integer specifying the current section.
     *      \param sections Integer specifying the total number of sections.
     * \return A ColorFloat with a hue calculated as 6.0f/sections*section, and a saturation, value, and alpha of 1.0.
     */
    static ColorFloat divideIntoChromaticSections(unsigned int sections, unsigned int section);

    /*!
     * \brief Generates a random color.
     * \details This function user rand() to generate a random ColorFloat with an optional specified alpha value.
     *      \param alpha Alpha of the random color to generate. An alpha of 0 will set the alpha to a random
     *       legal value.
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
     * \return A ColorFloat linearly interpolated between c1 and c2 using the given bias as a weight.
     */
    static ColorFloat blendedColor(ColorFloat c1, ColorFloat c2, float bias);

    /*!
     *
     */
    static ColorFloat highContrastColor(unsigned int section, int seed = 0);

 private:
    Colors();
    ~Colors();
    Colors(const Colors&);
    Colors & operator=(const Colors&);
};

#endif /* COLOR_H_ */
