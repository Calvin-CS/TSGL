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
// GL libraries
#include <GL/glew.h>									// Needed for GL functino calls

class Shape {
public:
	Shape() {}											// Default constructor for the Shape
	virtual ~Shape() {}									// Default destructor for the Shape
	virtual void draw() = 0;							// Abstract method for actually drawing the shape
};

#endif /* SHAPER_H_ */
