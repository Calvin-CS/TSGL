/*
 * Shape.h extends Drawable and provides a class for drawing a Shape.
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "Drawable.h"       // For extending our Drawable object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

  /*! \class Shape
   *  \brief A class for drawing shapes onto a Canvas or CartesianCanvas.
   *  \details Shape provides a base class for drawing shapes to a Canvas or CartesianCanvas.
   *  \note Shape is abstract, and must be extended.
   *  \details All Shape subclasses must override the getGeometryType() method. Something like the following should be used:
   *  \details <code>vertices</code> should be an array of floating point values in TSGL's vertex format.
   *  One vertex consists of 2 floating point values, signifying x and y components respectively.
   *  E.g., to draw a triangle, you would need 3 vertices = 6 floats -> vertices should be an array of length 6.
   *  \details <code>numVertices</code> should be the actual integer number of vertices to be drawn (e.g., *3* for a triangle).
   *  \details <code>drawingmode</code> should be one of GL's primitive drawing modes.
   *  See https://www.opengl.org/sdk/docs/man2/xhtml/glBegin.xml for further information.
   */
class Shape : public Drawable {
protected:
    float calcX(); ///< Calculates the Shape's center x coordinate. For use only where the thread has already locked the Shape's mutex.
    float calcY(); ///< Calculates the Shape's center y coordinate. For use only where the thread has already locked the Shape's mutex.
    bool init; ///< Whether the vertex has been initialized completely
    GLfloat* vertices; ///< Buffer of x, y coordinates
    ColorFloat color; ///< Color of the Shape
    int size,           ///< Number of floating point numbers in vertices
        current,        ///< Current number of floating point numbers in vertices
        length;         ///< Number of vertices in vertices (size / 2)
 public:

    /*!
     * \brief Explicitly constructs a new Shape.
     * \details Explicit constructor for a Convex Shape object.
     *   \param numVertices the number of vertices the complete Shape will have.
     *   \param c The color of the Shape.
     * \warning An invariant is held where if numVertices is less than 2 then an error message is given.
     * \return A new Shape with a buffer for storing the specified numbered of vertices.
     */
    Shape(int numVertices, const ColorFloat& c);

    /*!
     * \brief Destroys a Shape object.
     * \details Destructor for a Shape.
     * \details Frees up memory that was allocated to a Shape object.
     */
    virtual ~Shape();

    /**
     * \brief Returns a pointer to the vertices array for renderer.
     * \details Vertices specifies x and y coordinates for the Shape.
     * \return Pointer to vertices.
     */
    virtual GLfloat* getPointerToVerticesArray();

    /**
     * \brief Returns the number of vertices in the Shape for renderer.
     * \return An int specifying the number of vertices.
     */
    virtual int getNumberOfVertices();

    /**
     * \brief Returns the geometry type for drawing.
     */
    virtual GLenum getGeometryType() = 0;

    /*!
     * \brief Adds another vertex to a Shape.
     * \details This function initializes the next vertex in the Shape.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    virtual void addVertex(int x, int y);

    /**
     * \brief Accessor for the current color of the Shape.
     * \return The ColorFloat of the Shape.
     */
    virtual ColorFloat getColor();

    /**
     * \brief Accessor for the pointer to the color of the Shape for renderer.
     * \return Pointer to ColorFloat of Shape
     */
    ColorFloat* getObjectColor();

    /**
     * \brief Sets the Shape to a new color.
     * \param c The new ColorFloat.
     */
    virtual void setColor(const ColorFloat& c);

    /**
     * \brief Moves the Shape to new coordinates.
     * \param x The new center x coordinate.
     * \param y The new center y coordinate.
     */
    virtual void setCenter(float x, float y);

    /**
     * \brief Returns the x coordinate of the Shape.
     * \return A float, the center x coordinate.
     */
    virtual float getX();

    /**
     * \brief Returns the y coordinate of the Shape.
     * \return A float, the center y coordinate.
     */
    virtual float getY();

    /**
     * \brief Rotates the Shape by an angle.
     * \details Rotates clockwise around the center of the shape.
     * \param angle Angle to rotate by, in radians.
     */
    virtual void rotate(float angle);

    /**
     * \brief Rotates the Shape by angle around a point.
     * \details Rotates clockwise around (x, y) by angle.
     * \param angle Angle to rotate by, in radians.
     * \param x The x coordinate to rotate around.
     * \param y The y coordinate to rotate around.
     */
    virtual void rotateAround(float angle, float x, float y);
};

}

#endif /* SHAPE_H_ */
