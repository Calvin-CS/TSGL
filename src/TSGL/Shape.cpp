#include "Shape.h"

namespace tsgl {

/*!
 * \brief Constructs a new Shape.
 * \details
 * - Usually <code>vertices</code> is filled with floating point values that represent the vertices of the shape to be drawn.
 * - You may define other items in the constructor that pertain to the attributes of the subclass that is extending Shape.
 * - At a minimum, you *MUST* fill an array of floating point values that pertain to the vertices of the shape.
 * \warning <b>You <i>must</i> inherit the parent's constructor if you are extending Shape.</b>
 * \note Refer to the Shape class description for more details.
 */
Shape::Shape(float x, float y, float z, float yaw, float pitch, float roll) : Drawable(x,y,z,yaw,pitch,roll) { }

/*!
 * \brief Draw the Shape.
 * \details This function actually draws the Shape to the Canvas.
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the Shape cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void Shape::draw(Shader * shader) {
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
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices * 7, vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(geometryType, 0, numberOfVertices);
    }

    if (isOutlined) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfOutlineVertices * 7, outlineVertices, GL_DYNAMIC_DRAW);
        glDrawArrays(outlineGeometryType, 0, numberOfOutlineVertices);
    }
}

 /*!
  * \brief Adds another vertex to a Shape.
  * \details This function initializes the next vertex in the Shape and adds it to a Shape buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param z The z position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Shape::addVertex(GLfloat x, GLfloat y, GLfloat z, const ColorFloat &color) {
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

 /*!
  * \brief Adds another outline vertex to a Shape.
  * \details This function initializes the next vertex in the Shape and adds it to a Shape buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param z The z position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Shape::addOutlineVertex(GLfloat x, GLfloat y, GLfloat z, const ColorFloat &color) {
    if (outlineInit) {
        TsglDebug("Cannot add anymore vertices.");
        printf("added enough already tbh\n");
        return;
    }
    attribMutex.lock();
    outlineVertices[currentOutlineVertex] = x;
    outlineVertices[currentOutlineVertex + 1] = y;
    outlineVertices[currentOutlineVertex + 2] = z;
    outlineVertices[currentOutlineVertex + 3] = color.R;
    outlineVertices[currentOutlineVertex + 4] = color.G;
    outlineVertices[currentOutlineVertex + 5] = color.B;
    outlineVertices[currentOutlineVertex + 6] = color.A;
    currentOutlineVertex += 7;
    if (currentOutlineVertex == numberOfOutlineVertices*7) {
        outlineInit = true;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Shape to a new color.
 * \param c The new ColorFloat.
 */
void Shape::setColor(ColorFloat c) {
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
 * \brief Sets the Shape to a new array of colors.
 * \param c The new array of ColorFloats.
 */
void Shape::setColor(ColorFloat c[]) {
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*7 + 3] = c[i].R;
        vertices[i*7 + 4] = c[i].G;
        vertices[i*7 + 5] = c[i].B;
        vertices[i*7 + 6] = c[i].A;
    }
}

/**
 * \brief Sets the Shape's outline to a new color.
 * \param c The new ColorFloat.
 */
void Shape::setOutlineColor(ColorFloat c) {
    attribMutex.lock();
    for(int i = 0; i < numberOfOutlineVertices; i++) {
        outlineVertices[i*7 + 3] = c.R;
        outlineVertices[i*7 + 4] = c.G;
        outlineVertices[i*7 + 5] = c.B;
        outlineVertices[i*7 + 6] = c.A;
    }
    attribMutex.unlock();
}

}