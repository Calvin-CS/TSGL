/*
 * color.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: mbv26
 */

#include "color.h"

RGBfloatType RGBintToRGBfloat(int r, int g, int b, int a) {
    if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0 || a > 255 || a < 0) {
        throw std::out_of_range("Values must be between 0 and 255 inclusive");
    }
    return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}
RGBfloatType RGBintToRGBfloat(const RGBintType& color) {
    return RGBintToRGBfloat(color.R, color.G, color.B, color.A);
}

RGBfloatType HSVToRGBfloat(float h, float s, float v, float a) {
    if (h > 6 || h < 0 || s > 1 || s < 0 || v > 1 || v < 0 || a > 1 || a < 0) {
        throw std::out_of_range(
            "H must be between 0 and 6 inclusive, S, V, and A must be between 0 and 1 inclusive");
    }

    float m, n, f;
    RGBfloatType color;
    color.A = a;

    int i = floor(h);
    f = h - i;                // Decimal part of h
    if (!(i & 1)) f = 1 - f;  // if i is even
    m = v * (1 - s);
    n = v * (1 - s * f);
    switch (i) {
        case 6:
        case 0:
            color.R = v;
            color.G = n;
            color.B = m;
            return color;
        case 1:
            color.R = n;
            color.G = v;
            color.B = m;
            return color;
        case 2:
            color.R = m;
            color.G = v;
            color.B = n;
            return color;
        case 3:
            color.R = m;
            color.G = n;
            color.B = v;
            return color;
        case 4:
            color.R = n;
            color.G = m;
            color.B = v;
            return color;
        case 5:
            color.R = v;
            color.G = m;
            color.B = n;
            return color;
        default:
            throw std::out_of_range("Bad H value");
    }
}
RGBfloatType HSVToRGBfloat(const HSVType& color) {
    return HSVToRGBfloat(color.H, color.S, color.V, color.A);
}

RGBfloatType divideIntoSections(unsigned int sections, unsigned int section, float value,
                                       float alpha) {
    if (value > 1 || value < 0 || alpha > 1 || alpha < 0) throw std::out_of_range(
        "Values must be between 0 and 1 inclusive");
    return HSVToRGBfloat(6.0f / sections * section, 1.0f, value, alpha);
}
RGBfloatType divideIntoSections(unsigned int sections, unsigned int section) {
    return HSVToRGBfloat(6.0f / sections * section, 1.0f, 1.0f, 1.0f);
}

RGBfloatType randomColor(unsigned int seed, float alpha) {
    if (alpha > 1 || alpha < 0) {
        throw std::out_of_range("Alpha must be between 0 and 1 inclusive");
    }
    srand(seed);
    return {rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, alpha};
}

RGBfloatType blendedColor(RGBfloatType c1, RGBfloatType c2, float bias) {
    if (bias > 1 || bias < 0) {
        throw std::out_of_range("Bias must be between 0 and 1 inclusive");
    } else if (bias == 1) {
        return c1;
    } else if (bias == 0) {
        return c2;
    } else {
        return {c1.R*bias+c2.R*(1-bias),c1.G*bias+c2.G*(1-bias),c1.B*bias+c2.B*(1-bias),c1.A*bias+c2.A*(1-bias)};
    }
}

