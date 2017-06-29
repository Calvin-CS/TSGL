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
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class Shape : public Drawable {
protected:
    bool init;          // Whether the vertex has been initialized completely
    float* vertices;    // Buffer for vertex data
    int size,           // Number of floating point numbers in vertices
        current,        // Current number of floating point numbers in vertices
        length;         // Number of vertices in vertices (size / 6)
    ColorFloat shapeColor;
    GLenum geometryType = GL_TRIANGLES;
 public:

    /*!
     * \brief Explicitly constructs a new Shape.
     * \details Explicit constructor for a Convex Shape object.
     *   \param numVertices the number of vertices the complete Shape will have.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new Shape with a buffer for storing the specified numbered of vertices.
     */
    Shape(int numVertices);

    /*!
     * \brief Destroys a Shape object.
     * \details Destructor for a Shape.
     * \details Frees up memory that was allocated to a Shape object.
     */
    virtual ~Shape();

    /*!
     * \brief Adds another vertex to a Shape.
     * \details This function initializes the next vertex in the Polyline and adds it to a Shape buffer.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     *   \param color The reference variable of the color of the vertex.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    virtual void addVertex(int x, int y, const ColorFloat &color);

    /*!
     * \brief Draw the Shape.
     * \details This function actually draws the Shape to the Canvas.
     * \note This function does nothing if the vertex buffer is not yet full.
     * \note A message is given indicating that the Shape is *NOT* ready to be drawn yet (vertex buffer = not full).
     */
    virtual void draw();

    //TODO: comment this, implement
    virtual float* getVerticesPointerForRenderer();

    /**
     * \brief Gets the current color of the Shape
     * \return The ColorFloat of the Shape.
     */
    virtual ColorFloat getColor() { return ColorFloat(vertices[2], vertices[3], vertices[4], vertices[5]); }

    /**
     * \brief Sets the Shape to a new color
     * \param c The new ColorFloat.
     */
    virtual void setColor(ColorFloat c);

    /**
     * \brief Moves the Shape to new coordinates
     * \param x The new center x coordinate.
     * \param y The new center y coordinate.
     */
    virtual void setCenter(int x, int y);

    /**
     * \brief Returns the x coordinate of the Shape
     * \return An int, the center x coordinate
     */
    virtual int getX();

    /**
     * \brief Returns the y coordinate of the Shape
     * \return An int, the center y coordinate
     */
    virtual int getY();
};

}

#endif /* SHAPE_H_ */
