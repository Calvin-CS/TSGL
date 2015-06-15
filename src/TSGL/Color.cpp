/*
 * Color.cpp contains the code for creating colors on a Canvas.
 *
 *  Created on: Jul 22, 2014
 *      Author: mbv26
 */

#include "Color.h"

namespace tsgl {

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

bool ColorFloat::operator==(ColorFloat& c1) {
    if((*this).R == c1.R && (*this).G == c1.G && (*this).B == c1.B) {
      return true;
    } else {
      return false;
    }
}

bool ColorFloat::operator!=(ColorFloat& c1) {
    if((*this).R == c1.R && (*this).G == c1.G && (*this).B == c1.B) {
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

bool ColorInt::operator==(ColorInt& c1) {
    if((*this).R == c1.R && (*this).G == c1.G && (*this).B == c1.B) {
      return true;
    } else {
      return false;
    }
}

bool ColorInt::operator!=(ColorInt& c1) {
    if((*this).R == c1.R && (*this).G == c1.G && (*this).B == c1.B) {
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
    const unsigned int PRIME1 = 47, PRIME2 = 71;
    float hue = ((start + PRIME1 * section) % 255) / 255.0f;
    float sat = (255 - (section-start + PRIME2 * (section-start)) % 64) / 255.0f;
    float val = (11 - (section*3  % 7)) / 11.0f;
    return ColorHSV(hue * 6.0f, sat, val, 1.0f);
}

}
