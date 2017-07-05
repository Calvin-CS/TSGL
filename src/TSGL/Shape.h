/*
 * Shape.h extends Drawable and provides a class for drawing a Shape.
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "Drawable.h"       // For extending our Drawable object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Shape
 *  \brief Draw an arbitrary shape with colored vertices.
 *  \details Shape is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 */
class Shape : public Drawable {
protected:
    bool init;          // Whether the vertex has been initialized completely
    GLfloat* vertices;    // Buffer for vertex data
    ColorFloat color;
    int size,           // Number of floating point numbers in vertices
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 2)
 public:

    /*!
     * \brief Explicitly constructs a new Shape.
     * \details Explicit constructor for a Convex Shape object.
     *   \param numVertices the number of vertices the complete Shape will have.
     *   \param c The color of the Shape.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
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
    virtual int getNumberOfVertices() { return length; }

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
     * \brief Gets the current color of the Shape.
     * \return The ColorFloat of the Shape.
     */
    virtual ColorFloat getColor() { return color; }

    /**
     * \brief Gets pointer to the color of the Shape for renderer.
     * \return Pointer to ColorFloat of Shape
     */
    ColorFloat* getObjectColor() { return &color; }

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
