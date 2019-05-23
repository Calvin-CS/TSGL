/*
 * Shape.h provides a base class from which to extend other drawable shapes.
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include <GL/glew.h>    // Needed for GL function calls
#include "Color.h"      // Needed for color type
#include "Drawable.h"

namespace tsgl {

/*! \class Shape
 *  \brief A class for drawing shapes onto a Canvas or CartesianCanvas.
 *  \warning <b><i>Though extending this class must be allowed due to the way the code is set up, attempting to do so
 *  could potentially mess up the internal GL calls the library uses. Proceed with great caution.</i></b>
 *  \details Shape provides a base class for drawing shapes to a Canvas or CartesianCanvas.
 *  \note Shape is abstract, and must be extended by the user.
 *  \details <code>vertices</code> should be an array of floating point values in TSGL's vertex format.
 *  One vertex consists of 6 floating point values, signifying x,y,red,green,blue,and alpha components respectively.
 *  E.g., to draw a triangle, you would need 3 vertices = 18 floats -> vertices should be an array of length 18.
 *  \details <code>numberofvertices</code> should be the actual integer number of vertices to be drawn (e.g., *3* for a triangle).
 *  \details <code>drawingmode</code> should be one of GL's primitive drawing modes.
 *  See https://www.opengl.org/sdk/docs/man2/xhtml/glBegin.xml for further information.
 *  \details Theoretically, you could potentially extend the Shape class so that you can create another Shape class that suits your needs.
 *  \details However, this is not recommended for normal use of the TSGL library.
 */
class Shape : public Drawable {
 protected:
    int numberOfVertices;
    float* vertices;
    GLenum geometryType;
    bool init = false;
    int current = 0;
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
    Shape() { isTextured = false; }

    /*!
     * \brief Destructor for the Shape.
     */
    virtual ~Shape() { delete[] vertices; }

       /*!
     * \brief Draw the Shape.
     * \details This function actually draws the Shape to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     * \note A message indicating that the Shape cannot be drawn yet will be given
     *   if the above condition is met (vertex buffer = not full).
     */
    void draw() {
      glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
      glDrawArrays(geometryType, 0, numberOfVertices);
    }

    virtual void addVertex(float x, float y, const ColorFloat &color = BLACK) {
      if (init) {
         TsglDebug("Cannot add anymore vertices.");
         return;
      }
      vertices[current] = x;
      vertices[current + 1] = y;
      vertices[current + 2] = color.R;
      vertices[current + 3] = color.G;
      vertices[current + 4] = color.B;
      vertices[current + 5] = color.A;
      current += 6;
      if (current == numberOfVertices*6) init = true;
    }

    virtual bool isProcessed() {
      return init;
    }
};

}

#endif /* SHAPE_H_ */
