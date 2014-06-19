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


RGBfloatType RGBintToRGBfloat(int r, int g, int b, int a) {
	if (r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0 || a > 255 || a < 0) {
		throw std::out_of_range("Values must be between 0 and 255 inclusive");
	}
	RGBfloatType color;
	color.R = r / 255;
	color.G = g / 255;
	color.B = b / 255;
	color.A = a / 255;
	return color;
}
inline RGBfloatType RGBintToRGBfloat(const RGBintType& color) {
	return RGBintToRGBfloat(color.R, color.G, color.B, color.A);
}

RGBintType RGBfloatToRGBint(float r, float g, float b, float a) {
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

RGBfloatType HSVToRGBfloat(float h, float s, float v, float a) {
	if (h > 6 || h < 0 || s > 1 || s < 0 || v > 1 || v < 0 || a > 1 || a < 0) {
		std::cout << h << ' ' << s << ' ' << v << ' ' << a << std::endl;
		throw std::out_of_range("H must be between 0 and 6 inclusive, S, V, and A must be between 0 and 1 inclusive");
	}
	float m, n, f;
	RGBfloatType color;

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

#endif /* COLOR_H_ */
