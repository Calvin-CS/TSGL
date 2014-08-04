/*
 * Text.cpp extends Shape and provides a class for drawing a string of text to the Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/11/2014
 */

#include "Text.h"
#include "iostream"

Text::Text(std::string s, ImageHandler& loader, int x, int y, const Color &color) {
    isTextured = true;  // Let the Canvas know we're a textured object
    myString = s;
    myLoader = loader;
    myX = x;
    myY = y;
    myColor = color;
}

void Text::draw() {
    TextLoader loader;
    int fontSize = 160;                                                       // Size in pixels of the rendered characters

    unsigned int width, height;
    loader.load_font("/usr/share/fonts/truetype/freefont/FreeMono.ttf");
    GLuint fontTexture = loader.load_char(rand() % 128, fontSize, width, height);

    std::cout << "Width: " << width << " Height: " << height << std::endl;

    glBindTexture(GL_TEXTURE_2D, fontTexture);                              // Set the current texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float *vertices = new float[32];                                           // Allocate the vertices

    vertices[2] = vertices[10] = vertices[18] = vertices[26] = myColor.R;   // Texture color of the coords
    vertices[3] = vertices[11] = vertices[19] = vertices[27] = myColor.G;   // (Default to opaque white)
    vertices[4] = vertices[12] = vertices[20] = vertices[28] = myColor.B;
    vertices[5] = vertices[13] = vertices[21] = vertices[29] = myColor.A;

    vertices[1] = myY;                                                      // Y coordinates
    vertices[9] = vertices[1];
    vertices[17] = vertices[1] + height;
    vertices[25] = vertices[1] + height;

    vertices[0] = myX;                               // X coordinates
    vertices[8] = myX + width;
    vertices[16] = myX;
    vertices[24] = myX + width;

    vertices[6] = vertices[7] = 0.0f;           // Texture coords of top left
    vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right

    glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_DYNAMIC_DRAW);  // Fill the buffer
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);                          // Draw the character

    delete vertices;
}
