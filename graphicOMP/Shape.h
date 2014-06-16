/*
 * Shape.h provides a base class from which to extend other drawable shapes
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/10/2014
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>

class Shape {
protected:
	int myR, myG, myB, myA; 			// Color data for the shape
	float myFR, myFG, myFB, myFA;		// Floating point data for the alpha channel
	bool useDefaultColor;				// Whether or not the shape uses the global drawing color
public:
	Shape();							// Default constructor for the Shape
	Shape(int r, int g, int b, int a);	// Explicit constructor for the Shape
	virtual ~Shape() {}					// Default destructor for the Shape
	void setColor(int r, int g, int b, int a);	// Mutator for the Shape's color
	int getColorR();					// Accessor for the red component of the Shape's color
	int getColorG();					// Accessor for the green component of the Shape's color
	int getColorB();					// Accessor for the blue component of the Shape's color
	int getColorA();					// Accessor for the alpha component of the Shape's color
	float getColorFR();					// Accessor for the red component of the Shape's float color
	float getColorFG();					// Accessor for the green component of the Shape's float color
	float getColorFB();					// Accessor for the blue component of the Shape's float color
	float getColorFA();					// Accessor for the alpha component of the Shape's float color
	virtual bool getIsPoint() = 0;		// Abstract method for the shape to return its pointness
	bool getUsesDefaultColor();			// Accessor for useDefaultColor
	virtual void draw() = 0;			// Abstract method for actually drawing the shape
};


/*
 * Default constructor for the Shape class
 * Returns: a new Shape using the global drawing color
 */
Shape::Shape() {
	myR = -1;	//Set all colors to -1 so our accessors don't return a color
	myG = -1;
	myB = -1;
	myB = 255;
	myFR = myFG = myFB = 0.0f;
	myFA = 1.0f;
	useDefaultColor = true;
}

/*
 * Explicit constructor for the Shape class
 * Parameters:
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * 		a, the alpha component
 * Returns: a new Shape with the specified color
 */
Shape::Shape(int r, int g, int b, int a) {
	myR = r; myFR = myR / 255.0f;
	myG = g; myFG = myG / 255.0f;
	myB = b; myFB = myB / 255.0f;
	myA = a; myFA = myA / 255.0f;
	useDefaultColor = false;	//We no longer use the global drawing color (if we did before)
}

/*
 * setColor sets the color for the Shape class
 * Parameters:
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 		a, the alpha component
 */
void Shape::setColor(int r, int g, int b, int a) {
	myR = r; myFR = myR / 255.0f;
	myG = g; myFG = myG / 255.0f;
	myB = b; myFB = myB / 255.0f;
	myA = a; myFA = myA / 255.0f;
	useDefaultColor = false;	//We no longer use the global drawing color (if we did before)
}

// Accessor functions for the individual components of the shape's color
int Shape::getColorR() { return myR; }
int Shape::getColorG() { return myG; }
int Shape::getColorB() { return myB; }
int Shape::getColorA() { return myA; }
float Shape::getColorFR() { return myFR; }
float Shape::getColorFG() { return myFG; }
float Shape::getColorFB() { return myFB; }
float Shape::getColorFA() { return myFA; }

// Accessor function for useDefaultColor
bool Shape::getUsesDefaultColor() { return useDefaultColor; }

#endif /* SHAPER_H_ */
