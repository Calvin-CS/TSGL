#include "Polyline.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Polyline.
  * \details Explicit constructor for a new Polyline object.
  *   \param x The x coordinate of the center of the Polyline.
  *   \param y The y coordinate of the center of the Polyline.
  *   \param z The z coordinate of the center of the Polyline.
  *   \param numVertices The number of vertices in the Polyline.
  *   \param yaw The Polyline's yaw.
  *   \param pitch The Polyline's pitch.
  *   \param roll The Polyline's roll.
  * \warning An invariant is held where if v is less than 2 then an error message is given.
  * \return A new Polyline with a buffer for storing the specified numbered of vertices.
  */
Polyline::Polyline(float x, float y, float z, int numVertices, float yaw, float pitch, float roll) : Shape(x,y,z,yaw,pitch,roll) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    attribMutex.lock();
    numberOfVertices = numVertices;
    numberOfOutlineVertices = 0;
    edgesOutlined = false;
    myXScale = myYScale = myZScale = 1;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    geometryType = GL_LINE_STRIP;
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new monocolored Polyline.
  * \details Explicit constructor for a new Polyline object.
  *   \param x The x coordinate of the center of the Polyline.
  *   \param y The y coordinate of the center of the Polyline.
  *   \param z The z coordinate of the center of the Polyline.
  *   \param numVertices The number of vertices in the Polyline.
  *   \param lineVertices An array of vertices for the Polyline. 3 floats * numVertices, x-y-z.
  *   \param yaw The Polyline's yaw.
  *   \param pitch The Polyline's pitch.
  *   \param roll The Polyline's roll.
  *   \param color Color of the Polyline, as a ColorFloat.
  * \warning An invariant is held where if v is less than 2 then an error message is given.
  * \return A new Polyline with a buffer for storing the specified numbered of vertices.
  */
Polyline::Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color) : Shape(x,y,z,yaw,pitch,roll) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    attribMutex.lock();
    numberOfVertices = numVertices;
    numberOfOutlineVertices = 0;
    edgesOutlined = false;
    myXScale = myYScale = myZScale = 1;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    geometryType = GL_LINE_STRIP;
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(lineVertices[3*i], lineVertices[3*i + 1], lineVertices[3*i + 2], color);
    }
}

 /*!
  * \brief Explicitly constructs a new multicolored Polyline.
  * \details Explicit constructor for a new Polyline object.
  *   \param x The x coordinate of the center of the Polyline.
  *   \param y The y coordinate of the center of the Polyline.
  *   \param z The z coordinate of the center of the Polyline.
  *   \param numVertices The number of vertices in the Polyline.
  *   \param lineVertices An array of vertices for the Polyline. 3 floats * numVertices, x-y-z.
  *   \param yaw The Polyline's yaw.
  *   \param pitch The Polyline's pitch.
  *   \param roll The Polyline's roll.
  *   \param color Array of ColorFloats for the Polyline; 1 ColorFloat per vertex.
  * \warning An invariant is held where if v is less than 2 then an error message is given.
  * \return A new Polyline with a buffer for storing the specified numbered of vertices.
  */
Polyline::Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color[]) : Shape(x,y,z,yaw,pitch,roll) {
    if (numVertices < 2)
        TsglDebug("Cannot have a line with fewer than 2 vertices.");
    attribMutex.lock();
    numberOfVertices = numVertices;
    numberOfOutlineVertices = 0;
    edgesOutlined = false;
    myXScale = myYScale = myZScale = 1;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    geometryType = GL_LINE_STRIP;
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(lineVertices[3*i], lineVertices[3*i + 1], lineVertices[3*i + 2], color[i]);
    }
}

/*!
 *  \brief Overrides isProcessed() in Drawable.h
 *  \details Overrides Drawable::isProcessed() to include invariant.
 */
bool Polyline::isProcessed() {
    if (numberOfVertices < 2) {
      TsglDebug("Cannot draw uninitialized Polyline.");
      return false;
    }
    return init;
}

}
