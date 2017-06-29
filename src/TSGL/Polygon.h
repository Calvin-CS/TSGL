/*
 * Polygon.h extends Shape and provides a class for drawing a polygon.
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Shape.h"       // For extending our Shape object
#include "Polyline.h"    // For outline
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Polygon
 *  \brief Draw an arbitrary Convex polygon with colored vertices.
 *  \details Polygon is a class for holding vertex data for a triangle strip with colored vertices.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling draw() before all vertices have been added will do nothing.
 */
class Polygon : public Shape {
protected:
  Polyline * outline;
  bool outlineOn;
public:

    /*!
     * \brief Explicitly constructs a new Polygon.
     * \details Explicit constructor for a Convex Polygon object.
     *   \param numVertices the number of vertices the complete Polygon will have.
     *   \param c The color of the Polygon.
     *   \param outlineC The color of the Polygon's outline.
     * \warning An invariant is held where if v is less than 3 then an error message is given.
     * \return A new Polygon with a buffer for storing the specified numbered of vertices.
     */
    Polygon(int numVertices, const ColorFloat& c, const ColorFloat& outlineColor);

    /*!
     * \brief Adds another vertex to a Polygon.
     * \details This function initializes the next vertex in the Polygon and adds it to a Shape buffer.
     *   \param x The x position of the vertex.
     *   \param y The y position of the vertex.
     *   \param color The reference variable of the color of the vertex.
     *   \param outlineColor The reference variables of the color of the vertex's outline.
     * \note This function does nothing if the vertex buffer is already full.
     * \note A message is given indicating when the vertex buffer is full.
     */
    virtual void addVertex(int x, int y);

    /**
     * \brief Draws the Outline of the Polygon
     * TODO: comment, maybe make protected
     */
    virtual void drawOutline();

    /*!
     * \brief Runs the Unit tests.
     * \details Runs the Unit tests for the Polygon class. addVertex() is tested.
     */
    static void runTests();

    /**
     * \brief Moves the Polygon to new coordinates
     * \param x The new center x coordinate.
     * \param y The new center y coordinate.
     */
    virtual void setCenter(int x, int y);

    /**
     * \brief Changes the outline setting
     * \param outlineSetting A bool determining whether the outline is drawn.
     */
    void setOutline(bool outlineSetting) { outlineOn = outlineSetting; }

    /**
    * \brief Rotates the Polygon by an angle
    * \details Rotates clockwise around the center of the shape
    * \param angle Angle to rotate by, in radians
    */
    virtual void rotate(float angle);

    // /**
    //  * \brief Rotates the Polygon by angle around a point
    //  * \details Rotates clockwise around (x, y) by angle
    //  * \param angle Angle to rotate by, in radians
    //  * \param x The x coordinate to rotate around
    //  * \param y The y coordinate to rotate around
    //  */
    // virtual void rotateAround(float angle, float x, float y);

    /**
     * \brief Rotates the Polygon by angle around (0, 0)
     * \param angle Angle to rotate by, in radians
     */
    virtual void rotateAroundOrigin(float angle);
};

}

#endif /* POLYGON_H_ */
