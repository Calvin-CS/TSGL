/*
 * color.h provides color types and methods of converting between them and generating them
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/30/2014
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <cmath>        // Needed for conversions
#include <stdexcept>    // Needed for exceptions

/*!
 * \brief Floating point RGBA color struct.
 * \details Color defines a color with floating point red, green, blue, and alpha components
 *      \param R Red component, between 0 and 1 inclusive.
 *      \param G Green component, between 0 and 1 inclusive.
 *      \param B Blue component, between 0 and 1 inclusive.
 *      \param A Alpha component, between 0 and 1 inclusive.
 */
struct 	Color 	{ float R, G, B, A; };
	const 	Color BLACK = { 0.0f,  0.0f,  0.0f,  1.0f };
	const 	Color GREY  = { 0.75f, 0.75f, 0.75f, 1.0f };
	const 	Color WHITE = { 1.0f,  1.0f,  1.0f,  1.0f };

/*!
 * 	\brief Constructs a Color from integer RGBA components.
 * 	\details This function converts a set of integer RGBA components to a Color type.
 * 		\param r Red component, between 0 and 255 inclusive.
 *      \param g Green component, between 0 and 255 inclusive.
 *      \param b Blue component, between 0 and 255 inclusive.
 *      \param a Alpha component, between 0 and 255 inclusive.
 *  \return a Color struct with floating point RGBA values
 */
Color RGBintToRGBfloat(int r, int g, int b, int a = 255);
/*!
 * \brief Integer RGBA color struct.
 * \details Colori defines a color with integer red, green, blue, and alpha components
 *      \param R Red component, between 0 and 255 inclusive.
 *      \param G Green component, between 0 and 255 inclusive.
 *      \param B Blue component, between 0 and 255 inclusive.
 *      \param A Alpha component, between 0 and 255 inclusive.
 */
struct Colori 		{
	int 	R, G, B, A;
	/*!
	 * \brief Implicit conversion from Colori to Color
	 * \details This defines the implicit conversion operator from an integer color type (Colori) to a floating point color type (Color)
	 */
	operator Color() {return RGBintToRGBfloat(R,G,B,A);}
};

/*!
 * 	\brief Constructs a Color from floating point HSVA components.
 * 	\details This function converts a set of floating point HSVA components to a Color type.
 * 		\param h Hue component, between 0 and 6 inclusive.
 *      \param s Saturation component, between 0 and 1 inclusive.
 *      \param v Value component, between 0 and 1 inclusive.
 *      \param a Alpha component, between 0 and 1 inclusive.
 *  \return a Color struct with floating point RGBA values
 */
Color HSVToRGBfloat(float h, float s, float v, float a = 1.0f);
/*!
 * \brief Floating pont HSVA color struct.
 * \details ColorHSV defines a color with floating point hue, saturation, value, and alpha components
 * 		\param H Hue component, between 0 and 6 inclusive.
 *      \param S Saturation component, between 0 and 1 inclusive.
 *      \param V Value component, between 0 and 1 inclusive.
 *      \param A Alpha component, between 0 and 1 inclusive.
 */
struct ColorHSV {
public:
	float H, S, V, A;
	/*!
	 * \brief Implicit conversion from ColorHSV to Color
	 * \details This defines the implicit conversion operator from an HSV color type (ColorHSV) to an RGB color type (Color)
	 */
	operator Color() {return HSVToRGBfloat(H,S,V,A);}
};

/*!
 * \brief Returns an HSVA color with a hue dependent on the number of sections
 * \details This function returns a ColorHSV whose hue is calculated from the provided section number and the total number of sections.
 * This function is used for creating a chromatic gradient from one part of the spectrum to another.
 * 		\param section Integer specifying the current section.
 *      \param sections Integer specifying the total number of sections.
 *      \param value Value component, between 0 and 1 inclusive.
 *      \param alpha Alpha component, between 0 and 1 inclusive.
 * \return A ColorHSV with a hue calculated as 6.0f/sections*section, saturation of 1.0, and the given value and alpha components.
 */
Color divideIntoChromaticSections(unsigned int sections, unsigned int section, float value, float alpha = 1.0f);

/*!
 * \brief Returns an HSVA color with a hue dependent on the number of sections
 * \details This function returns a ColorHSV whose hue is calculated from the provided section number and the total number of sections.
 * This function is used for creating a chromatic gradient from one part of the spectrum to another.
 * 		\param section Integer specifying the current section.
 *      \param sections Integer specifying the total number of sections.
 * \return A ColorHSV with a hue calculated as 6.0f/sections*section, and a saturation, value, and alpha of 1.0.
 */
Color divideIntoChromaticSections(unsigned int sections, unsigned int section);

/*!
 * \brief Generates a random color.
 * \details This function user rand() to generate a random Color() with an optional specified alpha value.
 * 		\param alpha Alpha of the random color to generate. An alpha of 0 will set the alpha to a random legal value.
 * \return A random Color.
 */
Color randomColor(float alpha = 0.0f);

/*!
 * \brief Blends two colors with a given bias towards the latter.
 * \details This function blends two Color structs together by taking a linear interpolation between the two and returns the result as a new Color.
 * 		\param c1 A Color.
 * 		\param c2 Another Color.
 * 		\param bias A bias between 0 and 1 inclusive.  A bias of 0 returns c1, a bias of 1 returns c2, and a bias in between returns a linear interpolation.
 * \return A Color linearly interpolated between c1 and c2 using the given bias as a weight.
 */
Color blendedColor(Color c1, Color c2, float bias);

#endif /* COLOR_H_ */
