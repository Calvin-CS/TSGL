#include "Image.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Image.
  * \details This is the explicit constructor for the Image class.
  *   \param filename The filename of the image to load.
  *   \param loader A reference pointer to the TextureHandler with which to load the image.
  *   \param x The x coordinate of the left of the Image.
  *   \param y The y coordinate of the top of the Image.
  *   \param width The width of the Image.
  *   \param height The height of the Image.
  *   \param alhpa The alpha of the Image.
  * \return A new Image is drawn with the specified coordinates, dimensions, and transparency.
  * \note <B>IMPORTANT</B>: In CartesianCanvas, *y* specifies the bottom, not the top, of the image.
  */
Image::Image(std::string filename, TextureHandler &loader, int x, int y, int width, int height, float alpha) {
    isTextured = true;  // Let the Canvas know we're a textured object
    myTexture = 0;      // Fix no texture initialization warning
    myWidth = width; myHeight = height;
    if (myWidth <= 0 || myHeight <= 0) {
      TextureHandler::getDimensions(filename,myWidth,myHeight);
    }
    myFile = filename;
    myLoader = &loader;
    vertices[0] = x;
    vertices[1] = y;
    vertices[8] = x + myWidth;
    vertices[9] = y;
    vertices[16] = x;
    vertices[17] = y + myHeight;
    vertices[24] = x + myWidth;
    vertices[25] = y + myHeight;
    vertices[2] = vertices[10] = vertices[18] = vertices[26] = 1.0f;  // Texture color of the coords
    vertices[3] = vertices[11] = vertices[19] = vertices[27] = 1.0f;
    vertices[4] = vertices[12] = vertices[20] = vertices[28] = 1.0f;
    vertices[5] = vertices[13] = vertices[21] = vertices[29] = alpha;
    vertices[6] = vertices[7] = 0.0f;           // Texture coords of top left
    vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right
}

 /*!
  * \brief Draw the Image.
  * \details This function actually draws the Image to the Canvas.
  */
void Image::draw() {
    unsigned int w, h;
    myLoader->loadPicture(myFile, w, h, myTexture);

    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

}
