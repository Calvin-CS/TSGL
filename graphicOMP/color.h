/*
 * color.h
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/19/2014
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>				// Needed for conversions
#include <stdexcept>			// Needed for exceptions
#include <iostream>

typedef struct {int R, G, B, A;} RGBintType;
typedef struct {float R, G, B, A;} RGBfloatType;
typedef struct {float H, S, V, A;} HSVType;

RGBfloatType RGBintToRGBfloat(int r, int g, int b, int a = 255) {
	if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0 || a > 255 || a < 0) {
		throw std::out_of_range("Values must be between 0 and 255 inclusive");
	}
	RGBfloatType color;
	color.R = r / 255.0f;
	color.G = g / 255.0f;
	color.B = b / 255.0f;
	color.A = a / 255.0f;
	return color;
}
inline RGBfloatType RGBintToRGBfloat(const RGBintType& color) {
	return RGBintToRGBfloat(color.R, color.G, color.B, color.A);
}

RGBintType RGBfloatToRGBint(float r, float g, float b, float a = 1.0) {
	if (r > 1 || r < 0 || g > 1 || g < 0 || b > 1 || b < 0 || a > 1 || a < 0) {
		throw std::out_of_range("Values must be between 0 and 1 inclusive");
	}
	RGBintType color;
	color.R = r * 255;
	color.G = g * 255;
	color.B = b * 255;
	color.A = a * 255;
	return color;
}
inline RGBintType RGBfloatToRGBint(const RGBfloatType& color) {
	return RGBfloatToRGBint(color.R, color.G, color.B, color.A);
}

RGBfloatType HSVToRGBfloat(float h, float s, float v, float a = 1.0) {
	if (h > 6 || h < 0 || s > 1 || s < 0 || v > 1 || v < 0 || a > 1 || a < 0) {
		std::cout << h << ' ' << s << ' ' << v << ' ' << a << std::endl << std::flush;
		throw std::out_of_range("H must be between 0 and 6 inclusive, S, V, and A must be between 0 and 1 inclusive");
	}
	float m, n, f;
	RGBfloatType color;
	color.A = a;

	int i = floor(h);
	f = h - i;						// Decimal part of h
	if ( !(i&1) ) f = 1 - f; 		// if i is even
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
inline RGBfloatType HSVToRGBfloat(const HSVType& color) {
	return HSVToRGBfloat(color.H, color.S, color.V, color.A);
}

inline RGBfloatType devideIntoSections(unsigned int sections, unsigned int section, float value, float alpha = 1.0) {
	if (value > 1 || value < 0 || alpha > 1 || alpha < 0) {
		throw std::out_of_range("Values must be between 0 and 1 inclusive");
	}
	return HSVToRGBfloat(6.0f/sections * section, 1.0f, value, alpha);
}
inline RGBfloatType devideIntoSections(unsigned int sections, unsigned int section) {
	return HSVToRGBfloat(6.0f/sections * section, 1.0f, 1.0f, 1.0f);
}

inline RGBfloatType randomColor(unsigned int seed, float alpha = 1.0) {
	if (alpha > 1 || alpha < 0) {
		throw std::out_of_range("Alpha must be between 0 and 1 inclusive");
	}
	srand(seed);
	return {rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, alpha};
}

#endif /* COLOR_H_ */
