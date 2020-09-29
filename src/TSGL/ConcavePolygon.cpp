#include "ConcavePolygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new ConcavePolygon.
  * \details Protected explicit constructor for a ConcavePolygon object. Used as a superclass constructor.
 *   \param centerX The x coordinate of the ConcavePolygon's center.
 *   \param centerY The y coordinate of the ConcavePolygon's center.
 *   \param centerZ The z coordinate of the ConcavePolygon's center.
 *   \param numVertices The number of vertices that make up the ConcavePolygon.
 *   \param yaw The ConcavePolygon's yaw in 3D space.
 *   \param pitch The ConcavePolygon's pitch in 3D space.
 *   \param roll The ConcavePolygon's roll in 3D space.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float yaw, float pitch, float roll) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
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
  * \brief Explicitly constructs a new ConcavePolygon with monocolored fill.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param centerX The x coordinate of the ConcavePolygon's center.
  *   \param centerY The y coordinate of the ConcavePolygon's center.
  *   \param centerZ The z coordinate of the ConcavePolygon's center.
  *   \param numVertices The number of vertices that make up the ConcavePolygon.
  *   \param x An array of the ConcavePolygon's x vertices.
  *   \param y An array of the ConcavePolygon's y vertices.
  *   \param yaw The ConcavePolygon's yaw in 3D space.
  *   \param pitch The ConcavePolygon's pitch in 3D space.
  *   \param roll The ConcavePolygon's roll in 3D space.
  *   \param color A ColorFloat, the ConcavePolygon's fill color.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_TRIANGLE_FAN;
    numberOfVertices = numVertices;
    vertices = new GLfloat[numberOfVertices * 7];
    myXScale = myYScale = myZScale = 1;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfOutlineVertices = numVertices;
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    attribMutex.unlock(); 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i] - centerX, y[i] - centerY, 0, color);
        addOutlineVertex(x[i] - centerX, y[i] - centerY, 0, GRAY);
    }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with multicolored fill.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param centerX The x coordinate of the ConcavePolygon's center.
  *   \param centerY The y coordinate of the ConcavePolygon's center.
  *   \param centerZ The z coordinate of the ConcavePolygon's center.
  *   \param numVertices The number of vertices that make up the ConcavePolygon.
  *   \param x An array of the ConcavePolygon's x vertices.
  *   \param y An array of the ConcavePolygon's y vertices.
  *   \param yaw The ConcavePolygon's yaw in 3D space.
  *   \param pitch The ConcavePolygon's pitch in 3D space.
  *   \param roll The ConcavePolygon's roll in 3D space.
  *   \param color An array of ColorFloats, the ConcavePolygon's fill color.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  * \warning Can sometimes incorrectly render; if this occurs, try shifting your last vertex to be your first vertex, or otherwise adjusting vertex order.
  */
ConcavePolygon::ConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[]) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    geometryType = GL_TRIANGLE_FAN;
    numberOfVertices = numVertices;
    vertices = new GLfloat[numberOfVertices * 7];
    myXScale = myYScale = myZScale = 1;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfOutlineVertices = numVertices;
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    attribMutex.unlock(); 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i] - centerX, y[i] - centerY, 0, color[i]);
        addOutlineVertex(x[i] - centerX, y[i] - centerY, 0, GRAY);
    }
}

/*!
 * \brief Draw the ConcavePolygon.
 * \details This function actually draws the ConcavePolygon to the Canvas.
 * \note This function overrides Drawable::draw()
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the ConcavePolygon cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void ConcavePolygon::draw(Shader * shader) {
    if (!init) {
        TsglDebug("Vertex buffer is not full.");
        return;
    }
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(myRotationPointX, myRotationPointY, myRotationPointZ));
    model = glm::rotate(model, glm::radians(myCurrentYaw), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(myCurrentPitch), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(myCurrentRoll), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ));
    model = glm::scale(model, glm::vec3(myXScale, myYScale, myZScale));

    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    if (isFilled) {
        /* extra stencil buffer stuff, because it's concave */
        glClearStencil(0);
        glClear(GL_STENCIL_BUFFER);
        glEnable(GL_STENCIL_TEST);
        glDisable(GL_CULL_FACE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_NEVER, 0, 1);
        glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
        /* end */

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices * 7, vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(geometryType, 0, numberOfVertices);

        /* extra stencil buffer stuff, because it's concave */
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices * 7, vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(geometryType, 0, numberOfVertices);

        glDisable(GL_STENCIL_TEST);
    }

    if (isOutlined) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfOutlineVertices * 7, outlineVertices, GL_DYNAMIC_DRAW);
        glDrawArrays(outlineGeometryType, 0, numberOfOutlineVertices);
    }
}
}
