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
Polyline::Polyline(float x, float y, float z, int numVertices, float yaw, float pitch, float roll) : Drawable(x,y,z,yaw,pitch,roll) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    attribMutex.lock();
    numberOfVertices = numVertices;
    myXScale = myYScale = myZScale = 1;
    vertices = new GLfloat[numberOfVertices * 7];
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
Polyline::Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color) : Drawable(x,y,z,yaw,pitch,roll) {
    if (numVertices < 2)
        TsglDebug("Cannot have a line with fewer than 2 vertices.");
    attribMutex.lock();
    numberOfVertices = numVertices;
    myXScale = myYScale = myZScale = 1;
    vertices = new GLfloat[numberOfVertices * 7];
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
Polyline::Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color[]) : Drawable(x,y,z,yaw,pitch,roll) {
    if (numVertices < 2)
        TsglDebug("Cannot have a line with fewer than 2 vertices.");
    attribMutex.lock();
    numberOfVertices = numVertices;
    myXScale = myYScale = myZScale = 1;
    vertices = new GLfloat[numberOfVertices * 7];
    attribMutex.unlock();
    for (int i = 0; i < numVertices; i++) {
        addVertex(lineVertices[3*i], lineVertices[3*i + 1], lineVertices[3*i + 2], color[i]);
    }
}

/*!
 * \brief Draw the Polyline.
 * \details This function actually draws the Polyline to the Canvas.
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the Polyline cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void Polyline::draw(Shader * shader) {
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

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices * 7, vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, numberOfVertices);
}

 /*!
  * \brief Adds another vertex to a Polyline.
  * \details This function initializes the next vertex in the Polyline and adds it to a Polyline buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param z The z position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Polyline::addVertex(GLfloat x, GLfloat y, GLfloat z, const ColorFloat &color) {
    if (init) {
        TsglDebug("Cannot add anymore vertices.");
        return;
    }
    attribMutex.lock();
    vertices[currentVertex] = x;
    vertices[currentVertex + 1] = y;
    vertices[currentVertex + 2] = z;
    vertices[currentVertex + 3] = color.R;
    vertices[currentVertex + 4] = color.G;
    vertices[currentVertex + 5] = color.B;
    vertices[currentVertex + 6] = color.A;
    currentVertex += 7;
    if (currentVertex == numberOfVertices*7) {
        init = true;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Polyline to a new color.
 * \param c The new ColorFloat.
 */
void Polyline::setColor(ColorFloat c) {
    attribMutex.lock();
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*7 + 3] = c.R;
        vertices[i*7 + 4] = c.G;
        vertices[i*7 + 5] = c.B;
        vertices[i*7 + 6] = c.A;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Polyline to a new array of colors.
 * \param c The new array of ColorFloats.
 */
void Polyline::setColor(ColorFloat c[]) {
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*7 + 3] = c[i].R;
        vertices[i*7 + 4] = c[i].G;
        vertices[i*7 + 5] = c[i].B;
        vertices[i*7 + 6] = c[i].A;
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
