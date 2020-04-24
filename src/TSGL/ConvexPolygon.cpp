#include "ConvexPolygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new ConvexPolygon.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the complete ConvexPolygon will have.
  *   \param filled Whether or not the ConvexPolygon is filled.
  *   \param outlined Whether or not the ConvexPolygon is outlined.
  * \return A new ConvexPolygon with a buffer for storing the specified numbered of vertices.
  */
ConvexPolygon::ConvexPolygon(float centerX, float centerY, float centerZ,  int numVertices, float yaw, float pitch, float roll) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_POLYGON;
    numberOfVertices = numberOfOutlineVertices = numVertices;
    outlineGeometryType = GL_LINE_LOOP;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();    
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with monocolored fill or outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color The color of the ConvexPolygon.
  *   \param filled Whether or not the ConvexPolygon is filled.
  *     (set to true by default).
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(float centerX, float centerY, float centerZ,  int numVertices, GLfloat x[], GLfloat y[], float yaw, float pitch, float roll, ColorGLfloat color) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_POLYGON;
    numberOfVertices = numberOfOutlineVertices = numVertices;
    outlineGeometryType = GL_LINE_LOOP;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();    
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with multicolored fill or outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color An array of colors for the ConvexPolygon.
  *   \param filled Whether or not the ConvexPolygon is filled.
  *     (set to true by default).
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(float centerX, float centerY, float centerZ,  int numVertices, GLfloat x[], GLfloat y[], float yaw, float pitch, float roll, ColorGLfloat color[]) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_POLYGON;
    numberOfVertices = numberOfOutlineVertices = numVertices;
    outlineGeometryType = GL_LINE_LOOP;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();    
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color[i]);
    }
}

}
