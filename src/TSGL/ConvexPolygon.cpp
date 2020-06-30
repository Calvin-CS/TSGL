#include "ConvexPolygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new ConvexPolygon.
  * \details Protected explicit constructor for a Convex Polygon object. Used as superclass constructor.
 *   \param centerX The x coordinate of the ConvexPolygon's center.
 *   \param centerY The y coordinate of the ConvexPolygon's center.
 *   \param centerZ The z coordinate of the ConvexPolygon's center.
 *   \param numVertices The number of vertices that make up the ConvexPolygon.
 *   \param yaw The ConvexPolygon's yaw in 3D space.
 *   \param pitch The ConvexPolygon's pitch in 3D space.
 *   \param roll The ConvexPolygon's roll in 3D space..
  * \return A new ConvexPolygon with a buffer for storing the specified numbered of vertices.
  */
ConvexPolygon::ConvexPolygon(float centerX, float centerY, float centerZ,  int numVertices, float yaw, float pitch, float roll) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_TRIANGLE_FAN;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfVertices = numberOfOutlineVertices = numVertices;
    vertices = new GLfloat[numberOfVertices * 7];
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();    
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with monocolored fill.
  * \details Explicit constructor for a Convex Polygon object.
 *   \param centerX The x coordinate of the ConvexPolygon's center.
 *   \param centerY The y coordinate of the ConvexPolygon's center.
 *   \param centerZ The z coordinate of the ConvexPolygon's center.
 *   \param numVertices The number of vertices that make up the ConvexPolygon.
 *   \param x An array of the ConvexPolygon's x vertices.
 *   \param y An array of the ConvexPolygon's y vertices.
 *   \param yaw The ConvexPolygon's yaw in 3D space.
 *   \param pitch The ConvexPolygon's pitch in 3D space.
 *   \param roll The ConvexPolygon's roll in 3D space.
 *   \param color A ColorFloat, the ConvexPolygon's fill color.
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(float centerX, float centerY, float centerZ,  int numVertices, GLfloat x[], GLfloat y[], float yaw, float pitch, float roll, ColorFloat color) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_TRIANGLE_FAN;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfVertices = numberOfOutlineVertices = numVertices;
    vertices = new GLfloat[numberOfVertices * 7];
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();    
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color);
        addOutlineVertex(x[i], y[i], 0, GRAY);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with multicolored fill.
  * \details Explicit constructor for a Convex Polygon object.
 *   \param centerX The x coordinate of the ConvexPolygon's center.
 *   \param centerY The y coordinate of the ConvexPolygon's center.
 *   \param centerZ The z coordinate of the ConvexPolygon's center.
 *   \param numVertices The number of vertices that make up the ConvexPolygon.
 *   \param x An array of the ConvexPolygon's x vertices.
 *   \param y An array of the ConvexPolygon's y vertices.
 *   \param yaw The ConvexPolygon's yaw in 3D space.
 *   \param pitch The ConvexPolygon's pitch in 3D space.
 *   \param roll The ConvexPolygon's roll in 3D space.
 *   \param color An array of ColorFloats, the ConvexPolygon's fill color.
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(float centerX, float centerY, float centerZ,  int numVertices, GLfloat x[], GLfloat y[], float yaw, float pitch, float roll, ColorFloat color[]) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_TRIANGLE_FAN;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfVertices = numberOfOutlineVertices = numVertices;
    vertices = new GLfloat[numberOfVertices * 7];
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();    
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color[i]);
        addOutlineVertex(x[i], y[i], 0, GRAY);
    }
}

}
