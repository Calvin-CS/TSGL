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
    currentRotation = 0;
    centerX = centerY = 0;
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
    if (current == numberOfVertices*6) {
        init = true;
        attribMutex.lock();

        float minX, maxX;
        minX = maxX = vertices[0];
        //Find min and max X
        for(int i = 0; i < numberOfVertices; i++) {
            if( vertices[i*6] < minX )
            minX = vertices[i*6];
            else if( vertices[i*6] > maxX )
            maxX = vertices[i*6];
        }
        centerX = (minX+maxX)/2;

        float minY, maxY;
        minY = maxY = vertices[1];
        //Find min and max X
        for(int i = 0; i < numberOfVertices; i++) {
            if( vertices[i*6+1] < minY )
            minY = vertices[i*6+1];
            else if( vertices[i*6+1] > maxY )
            maxY = vertices[i*6+1];
        }
        centerY = (minY+maxY)/2;

        attribMutex.unlock();
    }
}

/**
 * \brief Sets the Shape to a new color.
 * \param c The new ColorFloat.
 */
void Shape::setColor(ColorFloat c) {
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*6 + 2] = c.R;
        vertices[i*6 + 3] = c.G;
        vertices[i*6 + 4] = c.B;
        vertices[i*6 + 5] = c.A;
    }
}

/**
 * \brief Sets the Shape to a new array of colors.
 * \param c The new array of ColorFloats.
 */
void Shape::setColor(ColorFloat c[]) {
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*6 + 2] = c[i].R;
        vertices[i*6 + 3] = c[i].G;
        vertices[i*6 + 4] = c[i].B;
        vertices[i*6 + 5] = c[i].A;
    }
}

/**
 * \brief Alters the Shape's vertex locations.
 * \param deltaX The difference between the new and old vertex X coordinates.
 * \param deltaY The difference between the new and old vertex Y coordinates.
 */
void Shape::moveShapeBy(float deltaX, float deltaY) {
    attribMutex.lock();
    for(int i = 0; i < numberOfVertices; i++) {
      vertices[i*6]     += deltaX; //Transpose x
      vertices[(i*6)+1] += deltaY; //Transpose y
    }
    centerX += deltaX;
    centerY += deltaY;
    attribMutex.unlock();
}

/**
 * \brief Moves the Shape to new coordinates.
 * \param x The new center x coordinate.
 * \param y The new center y coordinate.
 */
void Shape::setCenter(float x, float y) {
    float deltaX = x - centerX; //Change for x
    float deltaY = y - centerY; //Change for y
    attribMutex.lock();

    centerX = x;
    centerY = y;

    for(int i = 0; i < numberOfVertices; i++) {
      vertices[i*6]     += deltaX; //Transpose x
      vertices[(i*6)+1] += deltaY; //Transpose y
    }
    attribMutex.unlock();
}

/**
 * \brief Returns the x coordinate of the Shape.
 * \return A float, the center x coordinate.
 */
float Shape::getX() { //TODO: decide if this is the best system to protect x and y
    return centerX;
}

/**
 * \brief Returns the y coordinate of the Shape.
 * \return A float, the center y coordinate.
 */
float Shape::getY() {
    return centerY;
}

/*!
 * \brief Mutator for the rotation of the Shape.
 * \details Rotates the Shape vertices around centerX, centerY.
 * \param radians Float value denoting how many radians to rotate the Shape.
 */
void Shape::setRotation(float radians) {
  float pivotX = getX();
  float pivotY = getY();
  float s = sin(radians - currentRotation);
  float c = cos(radians - currentRotation);
  currentRotation = radians;
  for(int i = 0; i < numberOfVertices; i++) {
    float x = vertices[6*i];
    float y = vertices[6*i+1];
    x -= pivotX;
    y -= pivotY;
    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    x = xnew + pivotX;
    y = ynew + pivotY;
    vertices[6*i] = x;
    vertices[6*i+1] = y;
  }
}

}