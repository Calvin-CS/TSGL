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
Shape::Shape() : Drawable() {
    isTextured = false;
}

/*!
 * \brief Draw the Shape.
 * \details This function actually draws the Shape to the Canvas.
 * \note This function does nothing if the vertex buffer is not yet full.
 * \note A message indicating that the Shape cannot be drawn yet will be given
 *   if the above condition is met (vertex buffer = not full).
 */
void Shape::draw() {
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(geometryType, 0, numberOfVertices);
}

 /*!
  * \brief Adds another vertex to a Shape.
  * \details This function initializes the next vertex in the Shape and adds it to a Shape buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Shape::addVertex(float x, float y, const ColorFloat &color) {
    if (init) {
        TsglDebug("Cannot add anymore vertices.");
        return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    if (current == numberOfVertices*6) init = true;
}

}