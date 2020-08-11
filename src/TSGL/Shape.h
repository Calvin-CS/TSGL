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
    int currentVertex = 0;
    GLenum geometryType;
    virtual void addVertex(GLfloat x, GLfloat y, GLfloat z, const ColorFloat &color = WHITE);  
    bool isFilled = true;  

   int numberOfOutlineVertices;
   int currentOutlineVertex = 0;
   GLenum outlineGeometryType;
   virtual void addOutlineVertex(GLfloat x, GLfloat y, GLfloat z, const ColorFloat &color = WHITE);
   GLfloat * outlineVertices;
   bool isOutlined = true;

   bool outlineInit = false;

 public:
    Shape(float x, float y, float z, float yaw, float pitch, float roll);

    virtual void draw(Shader * shader);

    virtual void setColor(ColorFloat c);
    virtual void setColor(ColorFloat c[]);
    virtual void setOutlineColor(ColorFloat c);
    virtual ColorFloat getColor();
    virtual void getColors(std::vector<ColorFloat> &colorVec);

    virtual bool isProcessed() { return outlineInit && init; }

    /*! \brief Set whether or not the Shape will be filled.
     *  \details Sets the isFilled instance variable to the value of the parameter.
     *  \param status Boolean value to which isFilled will be set equivalent.
     *  \warning Disabling fill on some 3D Shapes, like Cone and Cylinder, can be awkward visually.
     */
    virtual void setIsFilled(bool status) { isFilled = status; }

    /*! \brief Set whether or not the Shape will be outlined.
     *  \details Sets the isOutlined instance variable to the value of the parameter.
     *  \param status Boolean value to which isOutlined will be set equivalent.
     *  \warning Disabling outlines on monocolored 3D Shapes can be awkward visually.
     */
    virtual void setIsOutlined(bool status) { isOutlined = status; }

    ~Shape() { /* delete [] vertices; */ }
};

}

#endif /* SHAPE_H_ */
