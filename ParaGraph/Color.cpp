/*
 * color.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: mbv26
 */

#include "Color.h"

Color::Color() {
    R = G = B = A = 1.0f;
}

Color::Color(float r, float g, float b, float a) {
    if (r > 1 || r < 0 || g > 1 || g < 0 || b > 1 || b < 0 || a > 1 || a < 0) {
        throw std::out_of_range("Values must be between 0 and 1 inclusive");
    }

    R = r;
    G = g;
    B = b;
    A = a;
}

Colori::Colori() {
    R = G = B = A = 255;
}

Colori::Colori(int r, int g, int b, int a) {
    if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0 || a > 255 || a < 0) {
        throw std::out_of_range("Values must be between 0 and 255 inclusive");
    }

    R = r;
    G = g;
    B = b;
    A = a;
}

Colori::operator Color() {
    return {R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f};
}

ColorHSV::ColorHSV() {
    H = 0.0f;
    S = V = A = 1.0f;
}

ColorHSV::ColorHSV(float h, float s, float v, float a) {
    if (h > 6 || h < 0 || s > 1 || s < 0 || v > 1 || v < 0 || a > 1 || a < 0) {
        throw std::out_of_range(
            "H must be between 0 and 6 inclusive, S, V, and A must be between 0 and 1 inclusive");
    }

    H = h;
    S = s;
    V = v;
    A = a;
}

ColorHSV::operator Color() {
    float m, n, f;
    Color color;
    color.A = A;

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
        default:
            throw std::out_of_range("Bad H value");
    }
}

Color divideIntoChromaticSections(unsigned int sections, unsigned int section, float value, float alpha) {
    if (value > 1 || value < 0 || alpha > 1 || alpha < 0) throw std::out_of_range(
        "Values must be between 0 and 1 inclusive");
    return ColorHSV(6.0f / sections * section, 1.0f, value, alpha);
}
Color divideIntoChromaticSections(unsigned int sections, unsigned int section) {
    return ColorHSV(6.0f / sections * section, 1.0f, 1.0f, 1.0f);
}

Color randomColor(float alpha) {
    if (alpha > 1 || alpha < 0) {
        throw std::out_of_range("Alpha must be between 0 and 1 inclusive");
    }
    if (alpha == 0.0f) alpha = rand() % 255 / 255.0f;
    return {rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, alpha};
}

Color blendedColor(Color c1, Color c2, float bias) {
    if (bias > 1 || bias < 0) {
        throw std::out_of_range("Bias must be between 0 and 1 inclusive");
    } else if (bias == 1) {
        return c2;
    } else if (bias == 0) {
        return c1;
    } else {
        return {c2.R*bias+c1.R*(1-bias),c2.G*bias+c1.G*(1-bias),c2.B*bias+c1.B*(1-bias),c2.A*bias+c1.A*(1-bias)};
    }
}

