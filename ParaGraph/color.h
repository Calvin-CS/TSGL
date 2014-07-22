/*
 * color.h provides color types and methods of converting between them and generating them
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/22014
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>        // Needed for conversions
#include <stdexcept>    // Needed for exceptions

struct RGBfloatType {
    float R, G, B, A;
};
struct RGBintType {
    int R, G, B, A;
};
struct HSVType {
    float H, S, V, A;
};
struct ColoredVertex {
    float x, y;
    RGBfloatType c;
};

const RGBfloatType BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
const RGBfloatType GREY  = { 0.75f, 0.75f, 0.75f, 1.0f };
const RGBfloatType WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };

RGBfloatType RGBintToRGBfloat(int r, int g, int b, int a = 255);
inline RGBfloatType RGBintToRGBfloat(const RGBintType& color) ;

RGBfloatType HSVToRGBfloat(float h, float s, float v, float a = 1.0f);
inline RGBfloatType HSVToRGBfloat(const HSVType& color);

RGBfloatType divideIntoSections(unsigned int sections, unsigned int section, float value,
                                       float alpha = 1.0f);
inline RGBfloatType divideIntoSections(unsigned int sections, unsigned int section);

RGBfloatType randomColor(unsigned int seed, float alpha = 1.0f);

RGBfloatType blendedColor(RGBfloatType c1, RGBfloatType c2, float bias);

#endif /* COLOR_H_ */
