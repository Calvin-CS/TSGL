/*
 * Shape.h provides a base class from which to extend other drawable shapes
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/2/2014
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <GL/glew.h>    // Needed for GL function calls
#include "Color.h"      // Needed for color type

class Shape {
 protected:
    bool isTextured;
 public:
    Shape() { isTextured = false; }
    virtual ~Shape() {}
    virtual void draw() = 0;  // Abstract method for actually drawing the shape
    bool getIsTextured() { return isTextured; }
};

#endif /* SHAPER_H_ */
