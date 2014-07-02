/*
 * Shape.h provides a base class from which to extend other drawable shapes
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/2/2014
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <stdexcept>									// Needed for exceptions
#include "color.h"										// Needed for color type
#include "ColoredVertex.h"								// Needed for vertex data
// GL libraries
#include <GL/glew.h>									// Needed for GL functino calls

class Shape {
public:
	Shape(RGBfloatType);								// Explicit constructor for the Shape
	virtual ~Shape() {}								// Default destructor for the Shape
	virtual void draw() = 0;							// Abstract method for actually drawing the shape
};

/*
 * Explicit constructor for the Shape class
 * Parameters:		between 0 and 1
 * 		color, the color for the shape
 * Returns: a new Shape with the specified color
 */
Shape::Shape(RGBfloatType color) {}


#endif /* SHAPER_H_ */
