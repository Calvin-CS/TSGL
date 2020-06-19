#include "ConcavePolygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new ConcavePolygon.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float yaw, float pitch, float roll) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    edgesOutlined = false;
    geometryType = GL_TRIANGLE_FAN;
    numberOfVertices = numVertices;
    numberOfOutlineVertices = numVertices;
    outlineGeometryType = GL_LINE_LOOP;
    vertices = new GLfloat[numberOfOutlineVertices * 7];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock();   
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with monocolored fill or outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color An array of colors for the ConcavePolygon's fill or outline.
  *   \param filled Whether the ConcavePolygon should be filled
  *     (set to true by default).
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    edgesOutlined = false;
    geometryType = GL_TRIANGLE_FAN;
    numberOfVertices = numVertices;
    numberOfOutlineVertices = numVertices;
    outlineGeometryType = GL_LINE_LOOP;
    vertices = new GLfloat[numberOfOutlineVertices * 7];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock(); 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color);
    }
}

 /*!
  * \brief Explicitly constructs a new ConcavePolygon with multicolored fill or outline.
  * \details Explicit constructor for a ConcavePolygon object.
  *   \param numVertices The number of vertices the complete ConcavePolygon will have.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color An array of colors for the ConcavePolygon's fill or outline.
  *   \param filled Whether the ConcavePolygon should be filled
  *     (set to true by default).
  * \return A new ConcavePolygon with a buffer for storing the specified number of vertices.
  */
ConcavePolygon::ConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[]) : Shape(centerX,centerY,centerZ,yaw,pitch,roll) {
    attribMutex.lock();
    edgesOutlined = false;
    geometryType = GL_TRIANGLE_FAN;
    numberOfVertices = numVertices;
    numberOfOutlineVertices = numVertices;
    outlineGeometryType = GL_LINE_LOOP;
    vertices = new GLfloat[numberOfOutlineVertices * 7];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock(); 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color[i]);
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

    /* extra stencil buffer stuff, because it's concave */
    glClearStencil(0);
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
    /* end */

    // if (edgesOutlined) {
    //     glVertexPointer(3, GL_FLOAT, outlineStride*sizeof(GLfloat)*3, vertices);
    //     glColorPointer(4, GL_FLOAT, 0, outlineArray);

    //     glDrawArrays(outlineGeometryType, 0, numberOfOutlineVertices);
    // }
}
}
