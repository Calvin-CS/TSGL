#include "Triangle.h"

namespace tsgl {

Triangle::Triangle(int x1, int y1, int x2, int y2, int x3, int y3, const ColorFloat &color) {
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[6] = x2;
    vertices[7] = y2;
    vertices[12] = x3;
    vertices[13] = y3;
    vertices[2] = vertices[8] = vertices[14] = color.R;
    vertices[3] = vertices[9] = vertices[15] = color.G;
    vertices[4] = vertices[10] = vertices[16] = color.B;
    vertices[5] = vertices[11] = vertices[17] = color.A;
}

void Triangle::draw() {
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}

float* Triangle::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

}
