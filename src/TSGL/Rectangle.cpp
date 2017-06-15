#include "Rectangle.h"

namespace tsgl {

Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color, bool filled = false) {
    vertices[0] = x;
    vertices[1] = y;
    vertices[6] = x + width;
    vertices[7] = y;
    vertices[12] = x;
    vertices[13] = y + height;
    vertices[18] = x + width;
    vertices[19] = y + height;
    vertices[2] = vertices[8] = vertices[14] = vertices[20] = color.R;  // Color of the coords
    vertices[3] = vertices[9] = vertices[15] = vertices[21] = color.G;
    vertices[4] = vertices[10] = vertices[16] = vertices[22] = color.B;
    vertices[5] = vertices[11] = vertices[17] = vertices[23] = color.A;
}

float* getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
}


// Rectangle::Rectangle(int x, int y, int width, int height, const ColorFloat &color) {
//     vertices[0] = x;
//     vertices[1] = y;
//     vertices[6] = x + width;
//     vertices[7] = y;
//     vertices[12] = x;
//     vertices[13] = y + height;
//     vertices[18] = x + width;
//     vertices[19] = y + height;
//     vertices[2] = vertices[8] = vertices[14] = vertices[20] = color.R;  // Color of the coords
//     vertices[3] = vertices[9] = vertices[15] = vertices[21] = color.G;
//     vertices[4] = vertices[10] = vertices[16] = vertices[22] = color.B;
//     vertices[5] = vertices[11] = vertices[17] = vertices[23] = color.A;
// }

void Rectangle::draw() {
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}
