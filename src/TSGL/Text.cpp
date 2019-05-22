#include "Text.h"
#include "iostream"

namespace tsgl {

Text::Text(std::wstring text, TextureHandler &loader, int x, int y, unsigned int fontsize, const ColorFloat &color) {
    isTextured = true;  // Let the Canvas know we're a textured object
    myString = text;
    myLoader = &loader;
    myX = x;
    myY = y;
    myFontSize = fontsize;
    myColor = color;
}

void Text::draw() {
    float *vertices = new float[32];                                        // Allocate the vertices

    vertices[0]  = myX;                                                     // Pre-init the array with the start coords
    vertices[1]  = myY;

    vertices[2] = vertices[10] = vertices[18] = vertices[26] = myColor.R;   // Texture color of the coords
    vertices[3] = vertices[11] = vertices[19] = vertices[27] = myColor.G;   // (Default to opaque white)
    vertices[4] = vertices[12] = vertices[20] = vertices[28] = myColor.B;
    vertices[5] = vertices[13] = vertices[21] = vertices[29] = myColor.A;

    vertices[6]  = vertices[7] = 0.0f;          // Texture coords of top left
    vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right

    myLoader->drawText(myString, myFontSize, vertices);

    delete[] vertices;
}

}