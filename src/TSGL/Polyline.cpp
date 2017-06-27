#include "Polyline.h"

namespace tsgl {

Polyline::Polyline(int numVertices) : Shape(numVertices) { }

void Polyline::draw() {
    if (!init || length < 2) {
      TsglDebug("Cannot draw uninitialized Polyline.");
      return;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, length);
}

float* Polyline::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

}
