/*
 * Shape.h provides a base class from which to extend other drawable shapes
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/19/2014
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <stdexcept>									// Needed for exceptions
#include "color.h"										// Needed for color type
#include "Renderer.h"
#include "ColoredVertex.h"

// GL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glx.h>

class Shape {
protected:
	float myR, myG, myB, myA; 							// Color data for the shape
	bool useDefaultColor;								// Whether or not the shape uses the global drawing color
public:
	Shape();											// Default constructor for the Shape
	Shape(RGBfloatType);								// Explicit constructor for the Shape
	virtual ~Shape() {}									// Default destructor for the Shape
	void setColor(RGBfloatType);						// Mutator for the Shape's color
	RGBfloatType getColor();							// Accessor for the Shape's color
	float getColorR();									// Accessor for the red component of the Shape's color
	float getColorG();									// Accessor for the green component of the Shape's color
	float getColorB();									// Accessor for the blue component of the Shape's color
	float getColorA();									// Accessor for the alpha component of the Shape's color
	virtual bool getIsPoint() = 0;						// Abstract method for the shape to return its pointness
	bool getUsesDefaultColor();							// Accessor for useDefaultColor
	virtual void draw() = 0;							// Abstract method for actually drawing the shape
};

/*
 * Default constructor for the Shape class
 * Returns: a new Shape using the global drawing color
 */
Shape::Shape() {
	myR = myG = myB = 0.0f;
	myA = 1.0f;
	useDefaultColor = true;
}

/*
 * Explicit constructor for the Shape class
 * Parameters:		between 0 and 1
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 		a, the alpha component
 * Returns: a new Shape with the specified color
 */
Shape::Shape(RGBfloatType color) {
	setColor(color);
}

/*
 * setColor sets the color for the Shape class
 * Parameters:
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 		a, the alpha component
 */
void Shape::setColor(RGBfloatType color) {
	if (color.R > 1 || color.R < 0 || color.G > 1 || color.G < 0 || color.B > 1 || color.B < 0 || color.A > 1 || color.A < 0) {
		throw std::out_of_range("Values must be between 0 and 1 inclusive");
	}
	myR = color.R;
	myG = color.G;
	myB = color.B;
	myA = color.A;
	useDefaultColor = false;	// We no longer use the global drawing color (if we did before)
}

// Accessor for the shape's color
RGBfloatType Shape::getColor() { return {myR, myG, myB, myA}; }
// Accessor functions for the individual components of the shape's color
float Shape::getColorR() { return myR; }
float Shape::getColorG() { return myG; }
float Shape::getColorB() { return myB; }
float Shape::getColorA() { return myA; }

// Accessor function for useDefaultColor
bool Shape::getUsesDefaultColor() { return useDefaultColor; }

#endif /* SHAPER_H_ */
