/*
 * Shape.h provides a base class from which to extend other drawable shapes.
 */

#ifndef SHAPE_H_
#define SHAPE_H_

// #include <GL/glew.h>    // Needed for GL function calls
#include "../glad/glad.h"      // New loader for GL function calls TODO: fix the path here
#include "Color.h"      // Needed for color type

namespace tsgl {

/*! \class Shape
 *  \brief A class for drawing shapes onto a Canvas or CartesianCanvas.
 *  \warning <b><i>Though extending this class must be allowed due to the way the code is set up, attempting to do so
 *  could potentially mess up the internal GL calls the library uses. Proceed with great caution.</i></b>
 *  \details Shape provides a base class for drawing shapes to a Canvas or CartesianCanvas.
 *  \note Shape is abstract, and must be extended by the user.
 *  \details All Shape subclasses must override the draw() method. Though theoretically any GL calls can be used here,
 *  something like the following should be used:
 *    <code>
 *    glBufferData(GL_ARRAY_BUFFER, numberofvertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
 *    glDrawArrays(drawingmode, 0, numberofvertices);
 *    </code>
 *  \details <code>vertices</code> should be an array of floating point values in TSGL's vertex format.
 *  One vertex consists of 6 floating point values, signifying x,y,red,green,blue,and alpha components respectively.
 *  E.g., to draw a triangle, you would need 3 vertices = 18 floats -> vertices should be an array of length 18.
 *  \details <code>numberofvertices</code> should be the actual integer number of vertices to be drawn (e.g., *3* for a triangle).
 *  \details <code>drawingmode</code> should be one of GL's primitive drawing modes.
 *  See https://www.opengl.org/sdk/docs/man2/xhtml/glBegin.xml for further information.
 *  \details Theoretically, you could potentially extend the Shape class so that you can create another Shape class that suits your needs.
 *  \details However, this is not recommended for normal use of the TSGL library.
 */
class Shape {
 protected:
    bool isTextured; /*! Whether the shape is textured or not. If extending Shape, <B> you *must* leave this at false (unless you are working with an image). </B> */
    int renderLayer; // The depth index to control the drawing order of the shapes
    ColorFloat shapeColor;
    GLenum geometryType = GL_TRIANGLES;
 public:

    /*!
     * \brief Constructs a new Shape.
     * \details
     * - Usually <code>vertices</code> is filled with floating point values that represent the vertices of the shape to be drawn.
     * - You may define other items in the constructor that pertain to the attributes of the subclass that is extending Shape.
     * - At a minimum, you *MUST* fill an array of floating point values that pertain to the vertices of the shape.
     * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Shape.</b>
     * \note Refer to the Shape class description for more details.
     */
    Shape() {
      isTextured = false;
      renderLayer = -1;   // -1 is the uninitialized layer value for the shape.
                          // If it is not set in the object before adding to the
                          // canvas, the canvas sets the layer value to the canvas'
                          // current drawing layer
    }

    /*!
     * \brief Destructor for the Shape.
     */
    virtual ~Shape() {};

    /*!
     * \brief Actually draws the Shape to the Canvas.
     * \details This method renders the shape to the Canvas.
     *  - When you extend the Shape class, you *MUST* provide a definition for this method.
     *  - The definition must follow this format:
     *    <code>
     *    glBufferData(drawingMode, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
     *    glDrawArrays(drawingMode, 0, numberOfVertices);
     *    </code>
     *  - Really bad things could potentially happen if you do not follow this format. These two statements *MUST* be
     *    in the draw() method of the subclass that extends the Shape class.
     *  - You can add other statements in the subclass
     * \note Please refer to the class description for more information and warnings about overriding this method.
     */
    virtual void draw() = 0;  // Abstract method for actually drawing the shape
    virtual void render() {return;}
    GLenum getGeometryType() {return geometryType;}

    //TODO: comment this
    // virtual float* getVerticesPointerForRenderer() = 0;

    /*!
     * \brief Accessor for <code>isTextured</code>.
     * \return Whether the shape is a textured primitive or not.
     */
    bool getIsTextured() { return isTextured; }

    void setLayer(int n) { if (n>=0) renderLayer = n; }  //TODO: make this validate layer numbers and return an error if not ok
    int getLayer() { return renderLayer; }
};

}

#endif /* SHAPE_H_ */
