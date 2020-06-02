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
    vertices = new GLfloat[numberOfOutlineVertices * 3];
    colors = new GLfloat[numberOfOutlineVertices * 4];
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
    vertices = new GLfloat[numberOfOutlineVertices * 3];
    colors = new GLfloat[numberOfOutlineVertices * 4];
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
    vertices = new GLfloat[numberOfOutlineVertices * 3];
    colors = new GLfloat[numberOfOutlineVertices * 4];
    myXScale = myYScale = myZScale = 1;
    attribMutex.unlock(); 
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], 0, color[i]);
    }
}

 /*!
  * \brief Adds another vertex to a ConcavePolygon.
  * \details This function initializes the next vertex in the ConcavePolygon and adds it to a ConcavePolygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void ConcavePolygon::addVertex(float x, float y, float z, ColorFloat &color) {
    if (init) {
    TsglDebug("Cannot add anymore vertices.");
    return;
    }
    attribMutex.lock();
    vertices[currentVertex] = x;
    vertices[currentVertex + 1] = y;
    vertices[currentVertex + 2] = z;
    colors[currentColor] = color.R;
    colors[currentColor + 1] = color.G;
    colors[currentColor + 2] = color.B;
    colors[currentColor + 3] = color.A;
    currentVertex += 3;
    currentColor += 4;
    attribMutex.unlock();

    if (currentVertex == numberOfVertices*3) {
        attribMutex.lock();
        outlineArray = new GLfloat[numberOfOutlineVertices*4];
        std::fill_n(outlineArray, numberOfOutlineVertices*4, 0.75);
        init = true;
        attribMutex.unlock();
    }
}

/*!
 * \brief Draw the ConcavePolygon.
 * \details This function actually draws the ConcavePolygon to the Canvas.
 * \note This function overrides Drawable::draw()
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the Drawable cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void ConcavePolygon::draw() {
    if (!init) {
        TsglDebug("Vertex buffer is not full.");
        return;
    }
    glPushMatrix();
    glTranslatef(myRotationPointX, myRotationPointY, myRotationPointZ);
    glRotatef(myCurrentYaw, 0, 0, 1);
    glRotatef(myCurrentPitch, 0, 1, 0);
    glRotatef(myCurrentRoll, 1, 0, 0); 
    glTranslatef(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ);
    glScalef(myXScale, myYScale, myZScale);

    /* extra stencil buffer stuff, because it's concave */
    glClearStencil(0);
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_NEVER, 0, 1);
    glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
    /* end */

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);

    glDrawArrays(geometryType, 0, numberOfVertices);

    /* extra stencil buffer stuff, because it's concave */
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);

    glDrawArrays(geometryType, 0, numberOfVertices);

    glDisable(GL_STENCIL_TEST);
    /* end */

    if (edgesOutlined) {
        glVertexPointer(3, GL_FLOAT, outlineStride*sizeof(GLfloat)*3, vertices);
        glColorPointer(4, GL_FLOAT, 0, outlineArray);

        glDrawArrays(outlineGeometryType, 0, numberOfOutlineVertices);
    }

    glPopMatrix();

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
}
