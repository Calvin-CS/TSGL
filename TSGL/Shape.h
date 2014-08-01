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

/*! \class Shape
 *  \brief A class for drawing shapes onto a Canvas or CartesianCanvas.
 *  \warning <B><I>Though extending this class must be allowed due to the way the code is set up, attempting to do so
 *  could potentially mess up the internal GL calls the library uses.  Proceed with great caution.</I></B>
 *  \details Shape provides a base class for drawing shapes to a Canvas or CartesianCanvas.
 *  Shape is abstract, and must be extended by the user.
 *  \details All Shape subclasses must override the draw() method. Though theoretically any GL calls can be used here,
 *  something like the following should be used:
 *      \code
 *      glBufferData(GL_ARRAY_BUFFER, <numberofvertices> * 6 * sizeof(float), <vertices>, GL_DYNAMIC_DRAW);
 *      glDrawArrays(<drawingmode>, 0, <numberofvertices>);
 *      \endcode
 *  \details <vertices> should be an array of floating point values in TSGL's vertex format.
 *  One vertex consists of 6 floating point values, signifying x,y,red,green,blue,and alpha components respectively.
 *  E.g., to draw a triangle, you would need 3 vertices = 18 floats -> vertices should be an array of length 18.
 *  \details <numberofvertices> should be the actual integer number of vertices to be drawn (e.g., *3* for a triangle).
 *  \details <drawingmode> should be one of GL's primitive drawing modes.
 *  See https://www.opengl.org/sdk/docs/man2/xhtml/glBegin.xml for further information.
 *
 */
class Shape {
 protected:
    bool isTextured; /*! Whether the shape is textured or not. If extending Shape, <B> you *must* leave this at false.</B> */
 public:
    /*!
     * \brief Constructs a new Shape.
     * \warning You <B>must</B> inherit the parent's constructor if you are extending Shape.
     */
    Shape() { isTextured = false; }
    /*!
     * \brief Destructor for the Shape.
     * \warning You <B>must</B> inherit the parent's constructor if you are extending Shape.
     */
    virtual ~Shape() {}
    /*!
     * \brief Actually draws the shape to the Canvas.
     * \details This method renders the shape to the Canvas.
     * Please refer to the class description for information and warning about overriding this method.
     */
    virtual void draw() = 0;  // Abstract method for actually drawing the shape
    /*!
     *  \brief Accessor for *isTextured*.
     */
    bool getIsTextured() { return isTextured; }
};

#endif /* SHAPER_H_ */
