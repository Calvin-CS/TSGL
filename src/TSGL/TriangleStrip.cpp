#include <TriangleStrip.h>

namespace tsgl {

/*!
 *  \brief Explicitly construct a new TriangleStrip with monocolored fill or outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param x
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param color The color of the TriangleStrip
 *      \param filled Whether the TriangleStrip should be filled
 *          (set to true by default).
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll,  ColorFloat color) : ConvexPolygon(centerX,centerY,centerZ,numVertices,yaw,pitch,roll) { 
    attribMutex.lock();
    geometryType = GL_TRIANGLE_STRIP;
    outlineGeometryType = GL_LINE_STRIP;
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], z[i], color);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with multicolored fill or outline.
 *  \details Explicit constructor for a TriangleStrip object.
 *      \param numVertices The number of vertices.
 *      \param x An array of x parameters for the vertices
 *      \param y An array of y parameters for the vertices
 *      \param color An array of colors for the TriangleStrip
 *      \param filled Whether the TriangleStrip should be filled
 *          (set to true by default).
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(centerX,centerY,centerZ,numVertices,yaw,pitch,roll) { 
    attribMutex.lock();
    geometryType = GL_TRIANGLE_STRIP;
    outlineGeometryType = GL_LINE_STRIP;
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], z[i], color[i]);
    }
}
}
