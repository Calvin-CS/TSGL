#include "Polygon.h"

namespace tsgl {

Polygon::Polygon(int numVertices)  : Shape()  {
//  attribMutex.lock();
  if (numVertices < 3) {
    TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
  }
//  attribMutex.unlock();
}

void Polygon::draw()  {
  if(isFilled) {
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(geometryType, 0, numberOfVertices);
  }
  if(hasOutline) {
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_LOOP, 0, numberOfVertices);
  }
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

}