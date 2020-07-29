#include <TriangleStrip.h>

namespace tsgl {

/*!
 *  \brief Explicitly construct a new TriangleStrip with monocolored fill.
 *  \details Explicit constructor for a TriangleStrip object.
  *   \param centerX The x coordinate of the center of the TriangleStrip.
  *   \param centerY The y coordinate of the center of the TriangleStrip.
  *   \param centerZ The z coordinate of the center of the TriangleStrip.
 *    \param numVertices The number of vertices.
 *    \param x An array of numVertices x parameters for the vertices of the TriangleStrip.
 *    \param y An array of numVertices y parameters for the vertices of the TriangleStrip.
 *    \param z An array of numVertices z parameters for the vertices of the TriangleStrip.
  *   \param yaw The TriangleStrip's yaw.
  *   \param pitch The TriangleStrip's pitch.
  *   \param roll The TriangleStrip's roll.
 *    \param color The color of the TriangleStrip
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll,  ColorFloat color) : ConvexPolygon(centerX,centerY,centerZ,numVertices,yaw,pitch,roll) { 
    attribMutex.lock();
    geometryType = GL_TRIANGLE_STRIP;
    outlineGeometryType = GL_LINE_STRIP;
    numberOfOutlineVertices = numVertices;
    isOutlined = false;
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i] - centerX, y[i] - centerY, z[i] - centerZ, color);
        addOutlineVertex(x[i] - centerX, y[i] - centerY, z[i] - centerZ, GRAY);
    }
}

/*!
 *  \brief Explicitly construct a new TriangleStrip with multicolored fill.
 *  \details Explicit constructor for a TriangleStrip object.
  *   \param centerX The x coordinate of the center of the TriangleStrip.
  *   \param centerY The y coordinate of the center of the TriangleStrip.
  *   \param centerZ The z coordinate of the center of the TriangleStrip.
 *    \param numVertices The number of vertices.
 *    \param x An array of numVertices x parameters for the vertices of the TriangleStrip.
 *    \param y An array of numVertices y parameters for the vertices of the TriangleStrip.
 *    \param z An array of numVertices z parameters for the vertices of the TriangleStrip.
  *   \param yaw The TriangleStrip's yaw.
  *   \param pitch The TriangleStrip's pitch.
  *   \param roll The TriangleStrip's roll.
 *    \param color An array of colors for the TriangleStrip
 *  \return A new TriangleStrip with the specified vertices and color.
 */
TriangleStrip::TriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color[]) : ConvexPolygon(centerX,centerY,centerZ,numVertices,yaw,pitch,roll) { 
    attribMutex.lock();
    geometryType = GL_TRIANGLE_STRIP;
    outlineGeometryType = GL_LINE_STRIP;
    numberOfOutlineVertices = numVertices;
    isOutlined = false;
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i] - centerX, y[i] - centerY, z[i] - centerZ, color[i]);
        addOutlineVertex(x[i] - centerX, y[i] - centerY, z[i] - centerZ, GRAY);
    }
}
}
