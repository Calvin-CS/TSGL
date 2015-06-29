/*
 * Color.cpp contains the code for creating colors on a Canvas.
 *
 *  Created on: Jul 22, 2014
 *      Author: mbv26
 */

#include "Color.h"

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

ColorFloat::ColorFloat() {
    R = G = B = A = 1.0f;
}

ColorFloat::ColorFloat(float r, float g, float b, float a) {
    bool oor = false;
    oor |= clamp(r,0,1);
    oor |= clamp(g,0,1);
    oor |= clamp(b,0,1);
    oor |= clamp(a,0,1);
    if (oor)
      TsglErr("Out of range parameter specified for ColorFloat");
    R = r; G = g; B = b; A = a;
}

std::string ColorFloat::AsString() {
    std::stringstream ss;
    ss << R << "R," << G << "G," << B << "B," << A << "A";
    return ss.str();
}

//From http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both
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

bool ColorFloat::operator==(ColorFloat& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return true;
    } else {
      return false;
    }
}

bool ColorFloat::operator!=(ColorFloat& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return false;
    } else {
      return true;
    }
}

ColorFloat ColorFloat::operator*(float f) {
    return ColorFloat((*this).R*f,(*this).G*f,(*this).B*f,(*this).A);
}

ColorInt::ColorInt() {
    R = G = B = A = 255;
}

ColorInt::ColorInt(int r, int g, int b, int a) {
    bool oor = false;
    oor |= clamp(r,0,255);
    oor |= clamp(g,0,255);
    oor |= clamp(b,0,255);
    oor |= clamp(a,0,255);
    if (oor)
      TsglErr("Out of range parameter specified for ColorInt");
    R = r; G = g; B = b; A = a;
}

std::string ColorInt::AsString() {
    std::stringstream ss;
    ss << R << "R," << G << "G," << B << "B," << A << "A";
    return ss.str();
}

ColorInt::operator ColorFloat() {
    return ColorFloat(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}

ColorInt::operator ColorHSV() {
    return (ColorHSV)ColorFloat(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
}

bool ColorInt::operator==(ColorInt& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return true;
    } else {
      return false;
    }
}

bool ColorInt::operator!=(ColorInt& c2) {
    if((*this).R == c2.R && (*this).G == c2.G && (*this).B == c2.B) {
      return false;
    } else {
    return true;
    }
}

ColorHSV::ColorHSV() {
    H = 0.0f;
    S = V = A = 1.0f;
}

ColorHSV::ColorHSV(float h, float s, float v, float a) {
    bool oor = false;
    oor |= clamp(h,0,6);
    oor |= clamp(s,0,1);
    oor |= clamp(v,0,1);
    oor |= clamp(a,0,1);
    if (oor)
      TsglErr("Out of range parameter specified for ColorHSV");
    H = h; S = s; V = v; A = a;
}

std::string ColorHSV::AsString() {
    std::stringstream ss;
    ss << H << "H," << S << "S," << V << "V," << A << "A";
    return ss.str();
}

ColorHSV::operator ColorFloat() {
    float m, n, f;
    ColorFloat color;
    color.A = A;

    if (clamp(H,0,6))
      TsglErr("Hue must be between 0 and 6 inclusive");
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

ColorFloat Colors::divideIntoChromaticSections(unsigned int sections, unsigned int section, float value, float alpha) {
    if (clamp(value,0,1) || clamp(alpha,0,1))
      TsglErr("Values must be between 0 and 1 inclusive");
    return ColorHSV(6.0f / sections * section, 1.0f, value, alpha);
}

ColorFloat Colors::divideIntoChromaticSections(unsigned int sections, unsigned int section) {
    return divideIntoChromaticSections(sections, section, 1.0f, 1.0f);
}

ColorFloat Colors::randomColor(float alpha) {
    if (clamp(alpha,0,1))
        TsglErr("Alpha must be between 0 and 1 inclusive");
    if (alpha == 0.0f) alpha = rand() % 255 / 255.0f;
    return ColorFloat(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, alpha);
}

ColorFloat Colors::blendedColor(ColorFloat c1, ColorFloat c2, float bias) {
    if (clamp(bias,0,1))
        TsglErr("Bias must be between 0 and 1 inclusive");
    return ColorFloat(c2.R * bias + c1.R * (1 - bias), c2.G * bias + c1.G * (1 - bias),
                      c2.B * bias + c1.B * (1 - bias), c2.A * bias + c1.A * (1 - bias));
}

ColorFloat Colors::highContrastColor(unsigned int section, int start) {
//    return DISTINCT_ARRAY_DATA[(section+start)%64];
    const unsigned int PRIME1 = 61, PRIME2 = 71;
    float hue = ((start + PRIME1 * section) % 255) / 255.0f;
    float sat = (255 - (section-start + PRIME2 * (section-start)) % 80) / 255.0f;
    float val = (11 - (section*3  % 7)) / 11.0f;
    return ColorHSV(hue * 6.0f, sat, val, 1.0f);
}

}
