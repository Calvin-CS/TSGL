/*
 * Image.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: pretzel
 */

#include "Image.h"

#ifdef _WIN32
namespace tsgl {
#endif

/*
 * Explicit constructor for the Image class
 * Parameters:
 *      x, the x coordinate of the Image's left edge
 *      y, the y coordinate of the Image's top edge
 *      w, the width of the Image
 *      h, the height of the Image
 * Returns: a new Image with the specified top left corner and dimensions
 */
Image::Image(std::string f, TextureHandler &loader, int x, int y, int w, int h, float a) {
    isTextured = true;  // Let the Canvas know we're a textured object
    myFile = f;
    myLoader = &loader;
    vertices[0] = x;
    vertices[1] = y;
    vertices[8] = x + w;
    vertices[9] = y;
    vertices[16] = x;
    vertices[17] = y + h;
    vertices[24] = x + w;
    vertices[25] = y + h;
    vertices[2] = vertices[10] = vertices[18] = vertices[26] = 1.0f;  // Texture color of the coords
    vertices[3] = vertices[11] = vertices[19] = vertices[27] = 1.0f;
    vertices[4] = vertices[12] = vertices[20] = vertices[28] = 1.0f;
    vertices[5] = vertices[13] = vertices[21] = vertices[29] = a;
    vertices[6] = vertices[7] = 0.0f;           // Texture coords of top left
    vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right
}

/*!
 * Draws the image onto the Canvas by loading the image into the internal
 * Texture Handler, binding the texture, and then drawing it onto the Canvas.
 *
 * Parameters:
 *      None.
 * Returns: The Canvas with the image drawn on it.
 */
void Image::draw() {
    unsigned int w, h;
    GLtexture myTexture;
    myLoader->loadPicture(myFile, w, h, myTexture);

    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Image::runTests() {
    tsglAssert(true, "false");
}

#ifdef _WIN32
}
#endif
