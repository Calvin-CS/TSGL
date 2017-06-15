#include "Polyline.h"

namespace tsgl {

Polyline::Polyline(int numVertices) {
    if (numVertices < 2)
      TsglDebug("Cannot have a line with fewer than 2 vertices.");
    length = numVertices;
    size = length * 6;
    current = 0;
    vertices = new float[size];
    init = false;
}

Polyline::~Polyline() {
    delete[] vertices;
}

void Polyline::addNextVertex(int x, int y, const ColorFloat &color) {
    if (init)  {
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
    if (current == size) init = true;
}

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
