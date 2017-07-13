/*
 * Polygon.h extends Shape and provides a class for drawing a polygon.
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Shape.h"       // For extending our Shape object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Polygon
 *  \brief Draw an arbitrary Convex polygon with colored vertices.
 *  \details Polygon is a class for holding vertex data for a Shape with at least three vertices.
 *  \note Polygon is abstract, and must be extended.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Adding to a Canvas before all vertices have been added will do nothing.
 */
class Polygon : public Shape {
protected:
  ColorFloat outlineColor; ///< Color of the Polygon's outline.
public:

    /*!
     * \brief Explicitly constructs a new Polygon.
     * \details Explicit constructor for a Convex Polygon object.
     *   \param numVertices the number of vertices the complete Polygon will have.
     *   \param c The color of the Polygon.
     *   \param outlineC The color of the Polygon's outline.
     * \warning An invariant is held where if numVertices is less than 3 then an error message is given.
     * \return A new Polygon with a buffer for storing the specified numbered of vertices.
     */
    Polygon(int numVertices, const ColorFloat& c, const ColorFloat& outlineC = BLACK);

    /**
     * \brief Returns a pointer to the vertices array for rendering the outline.
     * \details Vertices specifies x and y coordinates for the Shape.
     * \return Pointer to vertices.
     */
    virtual GLfloat* getPointerToOutlineVerticesArray();

    /**
     * \brief Returns the geometry type for drawing the outline.
     */
    virtual GLenum getOutlineGeometryType() { return GL_LINE_LOOP; }

    /**
     * \brief Returns the number of vertices in the Polygon's outline for renderer.
     * \return An int specifying the number of vertices in outline.
     */
    virtual int getOutlineNumberOfVertices();

    /**
     * \brief Returns the color of the Polygon's outline.
     * \return Pointer to color of the outline.
     */
    virtual ColorFloat* getOutlineColor();

    /**
     * \brief Sets the color of the Polygon's outline.
     */
    virtual void setOutlineColor(const ColorFloat& newColor);

    /**
     * \brief Sets whether the Polygon shows an outline.
     */
    void setHasOutline(bool outline);

};

}

#endif /* POLYGON_H_ */
