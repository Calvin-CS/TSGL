#include "Color.h"

//Workaround for VS not defining NAN (
#ifdef _MSC_VER
#define INFINITY (DBL_MAX + DBL_MAX)
#define NAN (INFINITY - INFINITY)
#endif

namespace tsgl {

// in foo.cpp
//static const unsigned char Foo_Msg_data[] = {0x00,0x01};

/*!
 * \brief Contains ColorInts that are used in the function highContrastColor().
 */
static const ColorFloat DISTINCT_ARRAY_DATA[] = {
   ColorInt(0,255,0, 255),
   ColorInt(0,0,255, 255),
   ColorInt(255,0,0, 255),
   ColorInt(1,255,254, 255),
   ColorInt(255,166,254, 255),
   ColorInt(255,219,102, 255),
   ColorInt(0,100,1, 255),
   ColorInt(1,0,103, 255),
   ColorInt(149,0,58, 255),
   ColorInt(0,125,181, 255),
   ColorInt(255,0,246, 255),
   ColorInt(255,238,232, 255),
   ColorInt(119,77,0, 255),
   ColorInt(144,251,146, 255),
   ColorInt(0,118,255, 255),
   ColorInt(213,255,0, 255),
   ColorInt(255,147,126, 255),
   ColorInt(106,130,108, 255),
   ColorInt(255,2,157, 255),
   ColorInt(254,137,0, 255),
   ColorInt(122,71,130, 255),
   ColorInt(126,45,210, 255),
   ColorInt(133,169,0, 255),
   ColorInt(255,0,86, 255),
   ColorInt(164,36,0, 255),
   ColorInt(0,174,126, 255),
   ColorInt(104,61,59, 255),
   ColorInt(189,198,255, 255),
   ColorInt(38,52,0, 255),
   ColorInt(189,211,147, 255),
   ColorInt(0,185,23, 255),
   ColorInt(158,0,142, 255),
   ColorInt(0,21,68, 255),
   ColorInt(194,140,159, 255),
   ColorInt(255,116,163, 255),
   ColorInt(1,208,255, 255),
   ColorInt(0,71,84, 255),
   ColorInt(229,111,254, 255),
   ColorInt(120,130,49, 255),
   ColorInt(14,76,161, 255),
   ColorInt(145,208,203, 255),
   ColorInt(190,153,112, 255),
   ColorInt(150,138,232, 255),
   ColorInt(187,136,0, 255),
   ColorInt(67,0,44, 255),
   ColorInt(222,255,116, 255),
   ColorInt(0,255,198, 255),
   ColorInt(255,229,2, 255),
   ColorInt(98,14,0, 255),
   ColorInt(0,143,156, 255),
   ColorInt(152,255,82, 255),
   ColorInt(117,68,177, 255),
   ColorInt(181,0,255, 255),
   ColorInt(0,255,120, 255),
   ColorInt(255,110,65, 255),
   ColorInt(0,95,57, 255),
   ColorInt(107,104,130, 255),
   ColorInt(95,173,78, 255),
   ColorInt(167,87,64, 255),
   ColorInt(165,255,210, 255),
   ColorInt(255,177,103, 255),
   ColorInt(0,155,255, 255),
   ColorInt(232,94,190, 255),
   ColorInt(255,255,255, 255),
};
const ColorFloat* Colors::DISTINCT_ARRAY = DISTINCT_ARRAY_DATA;

/*!
 * \brief Default ColorFloat constructor method.
 * \details This is the default constructor for the ColorFloat struct.
 * \note R, G, B, and A are all set to 1.0f by default.
 * \return A ColorFloat struct with default R, G, B, and A values.
 */
ColorFloat::ColorFloat() {
    R = G = B = A = 1.0f;
}

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
ColorFloat::ColorFloat(GLfloat v, GLfloat a) {
    if (clamp(v,0,1))
      TsglDebug("Out of range parameter specified for ColorFloat");
    R = v; G = v; B = v; A = a;
}

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
ColorFloat::ColorFloat(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    bool oor = false;
    oor |= clamp(r,0,1);
    oor |= clamp(g,0,1);
    oor |= clamp(b,0,1);
    oor |= clamp(a,0,1);
    if (oor)
      TsglDebug("Out of range parameter specified for ColorFloat");
    R = r; G = g; B = b; A = a;
}

/*!
 * \brief Returns a string representation of the ColorFloat.
 * \details This function returns a std::string representation of the ColorFloat.
 * \return A string representation of the ColorFloat.
 */
std::string ColorFloat::asString() {
    std::stringstream ss;
    ss << R << "R," << G << "G," << B << "B," << A << "A";
    return ss.str();
}

//From http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
/*!
 * \brief Implicit conversion from ColorFloat to ColorHSV.
 * \details This defines the implicit conversion operator from a floating point color type (ColorFloat) to an
 *   HSV color type (ColorHSV).
 */
ColorFloat::operator ColorHSV() {
    ColorHSV    out;
    double      min, max, delta;

    min = R < G ? R : G;
    min = min  < B ? min  : B;

    max = R > G ? R : G;
    max = max  > B ? max  : B;

    out.V = max;                                // v
    delta = max - min;
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.S = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
            // s = 0, v is undefined
        out.S = 0.0;
        out.H = NAN;                            // its now undefined
        return out;
    }
    if( R >= max )                           // > is bogus, just keeps compilor happy
        out.H = ( G - B ) / delta;        // between yellow & magenta
    else
    if( G >= max )
        out.H = 2.0 + ( B - R ) / delta;  // between cyan & yellow
    else
        out.H = 4.0 + ( R - G ) / delta;  // between magenta & cyan

    if( out.H < 0.0 )
        out.H += 6.0;

    return out;
}

/*!
 * \brief Implicit conversion from ColorFloat to ColorInt.
 * \details This defines the implicit conversion operator from a floating point color type (ColorFloat) to an
 *   integer color type (ColorInt).
 */
ColorFloat::operator ColorInt() {
    return ColorInt(R*MAX_COLOR,G*MAX_COLOR,B*MAX_COLOR,A*MAX_COLOR);
}

/*!
 * \brief Determines if two ColorFloats are equivalent.
 * \details Equality operator for two ColorFloats. Determines if they are equivalent.
 *    \param c2 Reference to the ColorFloat struct that is the second one in the equivalence comparison.
 * \note This function relies on (*this), which is a dereferenced pointer to the first ColorFloat struct in the comparison.
 *    (its the one on the left side of the == sign).
 * \returns true if the two ColorFloats are equivalent, false if otherwise.
 */
bool ColorFloat::operator==(ColorFloat& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return true;
    } else {
      return false;
    }
}

/*!
 * \brief Determines if two ColorFloats are *NOT* equivalent.
 * \details Inequality operator for two ColorFloats. Determines if they are *NOT* equivalent.
 *    \param c2 Reference to the ColorFloat struct that is the second one in the inequality comparison.
 * \note This function relies on (*this), which is a dereferenced pointer to the first ColorFloat struct in the inequality comparison.
 *       (its the one on the left side of the != sign).
 * \returns true if the two ColorFloats are not equivalent, false if otherwise.
 */
bool ColorFloat::operator!=(ColorFloat& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return false;
    } else {
      return true;
    }
}

/*!
 * \brief Multiplies the values of a ColorFloat by a float
 * \details This operator multiplies each of the components of a ColorFloat
 *   by amount <code>f</code>.
 * \param f Amount to multiply each component by
 * \returns A new ColorFloat constructed as ColorFloat(orig.R*f, orig.G*f, orig.b*f, orig.A*f)
 * \note Individual channels are clamped between 0 and 1.
 */
ColorFloat ColorFloat::operator*(float f) {
    float newR = (*this).R*f; clamp(newR,0,1);
    float newG = (*this).G*f; clamp(newG,0,1);
    float newB = (*this).B*f; clamp(newB,0,1);
    float newA = (*this).A;
    return ColorFloat(newR,newG,newB,newA);
}

ColorFloat ColorFloat::getContrast() {
    float color = (R + G + B > 1.5) ? 0.0 : 1.0;
    return ColorFloat(color, color, color, A);
}

/*!
 * \brief Default ColorInt constructor method.
 * \details This is the default constructor for the ColorInt struct.
 * \note R, G, B, and A are all set to 255 by default.
 * \return A ColorInt struct with default R, G, B, and A values.
 */
ColorInt::ColorInt() {
    R = G = B = A = 255;
}

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
ColorInt::ColorInt(int v, int a) {
    if (clamp(v,0,255))
      TsglDebug("Out of range parameter specified for ColorFloat");
    R = v; G = v; B = v; A = a;
}

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
ColorInt::ColorInt(int r, int g, int b, int a) {
    bool oor = false;
    oor |= clamp(r,0,255);
    oor |= clamp(g,0,255);
    oor |= clamp(b,0,255);
    oor |= clamp(a,0,255);
    if (oor)
      TsglDebug("Out of range parameter specified for ColorInt");
    R = r; G = g; B = b; A = a;
}

/*!
 * \brief Returns a string representation of the ColorInt.
 * \details This function returns a std::string representation of the ColorInt.
 * \return A string representation of the ColorInt.
 */
std::string ColorInt::asString() {
    std::stringstream ss;
    ss << R << "R," << G << "G," << B << "B," << A << "A";
    return ss.str();
}

/*!
 * \brief Implicit conversion from ColorInt to ColorFloat.
 * \details This defines the implicit conversion operator from an integer color type (ColorInt) to a
 *   floating point color type (ColorFloat).
 */
ColorInt::operator ColorFloat() {
    return ColorFloat(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}

/*!
 * \brief Implicit conversion from ColorInt to ColorHSV.
 * \details This defines the implicit conversion operator from an integer color type (ColorInt) to an
 *   HSV color type (ColorHSV).
 */
ColorInt::operator ColorHSV() {
    return (ColorHSV)ColorFloat(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}

/*!
 * \brief Determines if two ColorInts are equivalent.
 * \details Equality operator for two ColorInts. Determines if they are equivalent.
 *    \param c2 Reference to the ColorInt struct that is the second one in the equivalence comparison.
 * \note This function relies on (*this), which is a dereferenced pointer to the first ColorInt struct in the comparison.
 *    (its the one on the left side of the == sign).
 * \returns true if the two ColorInt are equivalent, false if otherwise.
 */
bool ColorInt::operator==(ColorInt& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return true;
    } else {
      return false;
    }
}

/*!
 * \brief Determines if two ColorInts are *NOT* equivalent.
 * \details Inequality operator for two ColorInts. Determines if they are *NOT* equivalent.
 *    \param c2 Reference to the ColorInt struct that is the second one in the inequality comparison.
 * \note This function relies on (*this), which is a dereferenced pointer to the first ColorInt struct in the inequality comparison.
 *    (its the one on the left side of the != sign).
 * \returns true if the two ColorInts are not equivalent, false if otherwise.
 */
bool ColorInt::operator!=(ColorInt& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return false;
    } else {
    return true;
    }
}

/*!
 * \brief Multiplies the values of a ColorInt by a float
 * \details This operator multiplies each of the components of a ColorInt
 *   by amount <code>f</code>.
 * \param f Amount to multiply each component by
 * \returns A new ColorInt constructed as ColorInt(orig.R*f, orig.G*f, orig.b*f, orig.A*f)
 * \note Individual channels are clamped between 0 and MAX_COLOR.
 */
ColorInt ColorInt::operator*(float f) {
    float newR = (*this).R*f; clamp(newR,0,MAX_COLOR);
    float newG = (*this).G*f; clamp(newG,0,MAX_COLOR);
    float newB = (*this).B*f; clamp(newB,0,MAX_COLOR);
    float newA = (*this).A;
    return ColorInt(newR,newG,newB,newA);
}

/*!
 * \brief Constructs a ColorHSV struct.
 * \details Default constructor for a ColorHSV struct.
 * \note H is set to 0.0f by default. S, V, and A are set to 1.0f by default.
 * \return A ColorHSV struct with default H, S, V, and A values.
 */
ColorHSV::ColorHSV() {
    H = 0.0f;
    S = V = A = 1.0f;
}

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
ColorHSV::ColorHSV(float h, float s, float v, float a) {
    bool oor = false;
    oor |= clamp(h,0,6);
    oor |= clamp(s,0,1);
    oor |= clamp(v,0,1);
    oor |= clamp(a,0,1);
    if (oor)
      TsglDebug("Out of range parameter specified for ColorHSV");
    H = h; S = s; V = v; A = a;
}

/*!
 * \brief Returns a string representation of the ColorHSV.
 * \details This function returns a std::string representation of the ColorHSV.
 * \return A string representation of the ColorHSV.
 */
std::string ColorHSV::asString() {
    std::stringstream ss;
    ss << H << "H," << S << "S," << V << "V," << A << "A";
    return ss.str();
}

/*!
 * \brief Implicit conversion from ColorHSV to ColorFloat.
 * \details This defines the implicit conversion operator from an HSV color type (ColorHSV) to a floating
 *   point color type (ColorFloat).
 */
ColorHSV::operator ColorFloat() {
    float m, n, f;
    ColorFloat color;
    color.A = A;

    if (clamp(H,0,6))
      TsglDebug("Hue must be between 0 and 6 inclusive");
    int i = floor(H);
    f = H - i;                // Decimal part of h
    if (!(i & 1)) f = 1 - f;  // if i is even
    m = V * (1 - S);
    n = V * (1 - S * f);
    switch (i) {
        case 6:
        case 0:
        default:
            color.R = V;
            color.G = n;
            color.B = m;
            return color;
        case 1:
            color.R = n;
            color.G = V;
            color.B = m;
            return color;
        case 2:
            color.R = m;
            color.G = V;
            color.B = n;
            return color;
        case 3:
            color.R = m;
            color.G = n;
            color.B = V;
            return color;
        case 4:
            color.R = n;
            color.G = m;
            color.B = V;
            return color;
        case 5:
            color.R = V;
            color.G = m;
            color.B = n;
            return color;
    }
}

/*!
 * \brief Implicit conversion from ColorHSV to ColorInt.
 * \details This defines the implicit conversion operator from an HSV color type (ColorHSV) to an integer
 *   color type (ColorInt).
 */
ColorHSV::operator ColorInt() {
  return (ColorInt)((ColorFloat)(*this));
}

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
ColorFloat Colors::divideIntoChromaticSections(unsigned int totalSections, unsigned int index, float value, float alpha) {
    if (clamp(value,0,1) || clamp(alpha,0,1))
      TsglDebug("Values must be between 0 and 1 inclusive");
    return ColorHSV(6.0f / totalSections * index, 1.0f, value, alpha);
}

/*!
 * \brief Returns an HSVA color with a hue dependent on the number of sections.
 * \details This function returns a ColorFloat whose hue is calculated from the provided section number and
 *  the total number of sections.
 *  This function is used for creating a chromatic gradient from one part of the spectrum to another.
 *    \param totalSections Unsigned integer specifying the total number of sections.
 *    \param index Unsigned integer specifying the current section.
 * \return A ColorFloat with a hue calculated as 6.0f / sections*section, and a saturation, value, and alpha of 1.0f.
 */
ColorFloat Colors::divideIntoChromaticSections(unsigned int totalSections, unsigned int index) {
    return divideIntoChromaticSections(totalSections, index, 1.0f, 1.0f);
}

/*!
 * \brief Generates a random color.
 * \details This function uses rand() to generate a random ColorFloat with an optional specified alpha value.
 *   \param alpha Alpha of the random color to generate. An alpha of 0 will set the alpha to a random
 *     legal value (set to 0.0f by default).
 * \warning An invariant is held for the alpha value where if its greater than 1 or less than 0
 *   then an error message is given.
 * \return A random ColorFloat.
 */
ColorFloat Colors::randomColor(float alpha) {
    if (clamp(alpha,0,1))
      TsglDebug("Alpha must be between 0 and 1 inclusive");
    if (alpha == 0.0f) alpha = rand() % 255 / 255.0f;
    return ColorFloat(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, alpha);
}

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
ColorFloat Colors::blend(ColorFloat c1, ColorFloat c2, float bias) {
    if (clamp(bias,0,1))
      TsglDebug("Bias must be between 0 and 1 inclusive");
    return ColorFloat(c2.R * bias + c1.R * (1 - bias), c2.G * bias + c1.G * (1 - bias),
                      c2.B * bias + c1.B * (1 - bias), c2.A * bias + c1.A * (1 - bias));
}

/*!
 * \brief Returns an HSV color with high contrast.
 * \details This function returns a ColorHSV with hue, saturation, and value calculated to
 *   contrast highly with colors with nearby indices.
 *   \param index Unsigned integer representing the current color index.
 *   \param offset Integer offset for the starting position of the calculation.
 * \return A ColorHSV representing a color visually distinct from its neighbors.
 */
ColorFloat Colors::highContrastColor(unsigned int index, int offset) {
    const unsigned int PRIME1 = 61, PRIME2 = 71;
    float hue = ((offset + PRIME1 * index) % 255) / 255.0f;
    float sat = (255 - (index-offset + PRIME2 * (index-offset)) % 80) / 255.0f;
    float val = (11 - (index*3  % 7)) / 11.0f;
    return ColorHSV(hue * 6.0f, sat, val, 1.0f);
}

}
