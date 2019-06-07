#include "Polygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Polygon.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices the number of vertices the complete Polygon will have.
  * \warning An invariant is held where if numVertices is less than 3 then an error message is given.
  * \return A new Polygon with a buffer for storing the specified numbered of vertices.
  */
Polygon::Polygon(int numVertices)  : Shape()  {
//  attribMutex.lock();
  if (numVertices < 3) {
    TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
  }
//  attribMutex.unlock();
}

/*!
 *  \brief This method actually draws the Polygon
 *  \details Depending on isFilled and hasOutline, draws either a series of connected lines outlining the Polygon or a filled version.
 */
void Polygon::draw()  {
  if(isFilled) {
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(geometryType, 0, numberOfVertices);
  }
  if(hasOutline) {
    glBufferData(GL_ARRAY_BUFFER, numberOfOutlineVertices * 6 * sizeof(float), outlineVertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_LOOP, 0, numberOfOutlineVertices);
  }
}

 /*!
  * \brief Adds another vertex to a Polygon.
  * \details This function initializes the next vertex in the Polygon and adds it to a Polygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Polygon::addVertex(float x, float y, const ColorFloat &color) {
    if (init) {
        TsglDebug("Cannot add anymore vertices.");
        return;
    }
    if(isFilled) {
      vertices[current] = x;
      vertices[current + 1] = y;
      vertices[current + 2] = color.R;
      vertices[current + 3] = color.G;
      vertices[current + 4] = color.B;
      vertices[current + 5] = color.A;
    }
    if(hasOutline) {
      outlineVertices[current] = x;
      outlineVertices[current + 1] = y;
      outlineVertices[current + 2] = color.R;
      outlineVertices[current + 3] = color.G;
      outlineVertices[current + 4] = color.B;
      outlineVertices[current + 5] = color.A;
    }
    current += 6;
    if (current == numberOfVertices*6) init = true;
}

 /*!
  * \brief Adds another vertex to a ConcavePolygon.
  * \details This function initializes the next vertex in the ConcavePolygon and adds it to a ConcavePolygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param fillColor The reference variable of the fill color of the vertex.
  *      \param outlineColor The reference variable of the outline color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Polygon::addVertex(float x, float y, const ColorFloat &fillColor, const ColorFloat &outlineColor) {
    if (init) {
        TsglDebug("Cannot add anymore vertices.");
        return;
    }
    if(isFilled) {
      vertices[current] = x;
      vertices[current + 1] = y;
      vertices[current + 2] = fillColor.R;
      vertices[current + 3] = fillColor.G;
      vertices[current + 4] = fillColor.B;
      vertices[current + 5] = fillColor.A;
    }
    if(hasOutline) {
      outlineVertices[current] = x;
      outlineVertices[current + 1] = y;
      outlineVertices[current + 2] = outlineColor.R;
      outlineVertices[current + 3] = outlineColor.G;
      outlineVertices[current + 4] = outlineColor.B;
      outlineVertices[current + 5] = outlineColor.A;
    }
    current += 6;
    if (current == numberOfVertices*6) init = true;
}

// GLfloat* Polygon::getPointerToOutlineVerticesArray() {
//   attribMutex.lock();
//   GLfloat* result = vertices;
//   attribMutex.unlock();
//   return result;
// }

// int Polygon::getOutlineNumberOfVertices() {
//   attribMutex.lock();
//   int l = length;
//   attribMutex.unlock();
//   return l;
// }

// ColorFloat* Polygon::getOutlineColor() {
//   attribMutex.lock();
//   ColorFloat* c = &outlineColor;
//   attribMutex.unlock();
//   return c;
// }

// void Polygon::setOutlineColor(const ColorFloat& newColor) {
//   attribMutex.lock();
//   outlineColor = newColor;
//   attribMutex.unlock();
// }

// void Polygon::setHasOutline(bool outline) {
//   attribMutex.lock();
//   hasOutline = outline;
//   attribMutex.unlock();
// }

 /*!
  * \brief Destructor for the Polygon.
  */
  Polygon::~Polygon() {
    if(hasOutline) { 
      delete[] outlineVertices; 
    }
    if(isFilled) {
      delete[] vertices;
    }
  }

}