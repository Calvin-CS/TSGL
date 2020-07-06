#include "Background.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Background.
  * \details Explicit constructor for a Background object.
  *   \param x The x coordinate of the center of the Background.
  *   \param y The y coordinate of the center of the Background.
  *   \param z The z coordinate of the center of the Background.
  *   \param width Background's width in pixels.
  *   \param yaw The Background's yaw, in degrees.
  *   \param pitch The Background's pitch, in degrees.
  *   \param roll The Background's roll, in degrees.
  *   \param c A ColorFloat for the Background's original color.
  * \warning An invariant is held where if width or height isn't positive then an error message is given.
  * \return A new Background with a buffer for storing the specified numbered of vertices.
  */
Background::Background(float x, float y, float z, GLint width, GLint height, float yaw, float pitch, float roll, const ColorFloat &c)  
: Drawable(x, y, z, yaw, pitch, roll)  {
    if (width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Background with non-positive width or height.");
    }
    attribMutex.lock();
    shaderType = TEXTURE_SHADER_TYPE;
    myXScale = myWidth = width;
    myYScale = myHeight = height;
    vertices = new GLfloat[30];
    buffer = new uint8_t[myWidth * myHeight * 4];
    vertices[0]  = vertices[1] = vertices[6] = vertices[10] = vertices[16]  = vertices[20] = -0.5; // x + y
    vertices[5] = vertices[11] = vertices[15] = vertices[21] = vertices[25] = vertices[26] = 0.5; // x + y
    vertices[2] = vertices[7] = vertices[12] = vertices[17] = vertices[22] = vertices[27] = 0; // z
    vertices[3]  = vertices[13]  = vertices[14] = vertices[23] = vertices[24] = vertices[29] = 0.0; // texture coord x + y
    vertices[4] = vertices[8] = vertices[9] = vertices[18] = vertices[19] = vertices[28] = 1.0; // texture coord x + y

    // make original background monocolor
    for (int j = 0; j < myHeight; j++) 
        for (int i = 0; i < myWidth; i++) {
            buffer[i * myHeight * 4 + j * 4] = (int) (c.R * 255);
            buffer[i * myHeight * 4 + j * 4 + 1] = (int) (c.G * 255);
            buffer[i * myHeight * 4 + j * 4 + 2] = (int) (c.B * 255);
            buffer[i * myHeight * 4 + j * 4 + 3] = (int) (c.A * 255);
        }

    init = true;  
    attribMutex.unlock();
}

 /*!
  * \brief Draw the Background.
  * \details This function actually draws the Background to the Canvas.
  */
void Background::draw(Shader * shader) {
    if (!init) {
        TsglDebug("Vertex buffer is not full.");
        return;
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(myRotationPointX, myRotationPointY, myRotationPointZ));
    model = glm::rotate(model, glm::radians(myCurrentYaw), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(myCurrentPitch), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(myCurrentRoll), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ));
    model = glm::scale(model, glm::vec3(myXScale, myYScale, myZScale));

    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int alphaLoc = glGetUniformLocation(shader->ID, "alpha");
    glUniform1f(alphaLoc, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &myTexture);
    // enable textures and bind the texture id
    glBindTexture(GL_TEXTURE_2D, myTexture);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // actually generate the texture + mipmaps
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myWidth, myHeight, 0,
	           	 GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteTextures(1, &myTexture);

    glDisable(GL_TEXTURE_2D);
}

 /*!
  * \brief Gets the color of the pixel drawn on the current Background at the given row and column.
  * \note (0,0) signifies the <b>top-left</b> of the Background's texture.
  *      \param row The row (y-position) of the pixel to grab.
  *      \param col The column (x-position) of the pixel to grab.
  * \return A ColorInt containing the color of the pixel at (col,row).
  */
ColorInt Background::getPixel(int row, int col) {
    ColorInt c;
    attribMutex.lock();
    c = ColorInt(buffer[(row * myWidth + col) * 4],
                    buffer[(row * myWidth + col) * 4 + 1],
                    buffer[(row * myWidth + col) * 4 + 2],
                    buffer[(row * myWidth + col) * 4 + 3]);
    attribMutex.unlock();
    return c;
}

 /*!
  * \brief Draws a single pixel, specified in row,column format.
  * \details This function alters the value at the specified row, column offset within the Background's buffer variable.
  * \note (0,0) signifies the <b>top-left</b> of the Background.
  *   \param row The row (y-position) of the pixel.
  *   \param col The column (x-position) of the pixel.
  *   \param color The color of the point.
  */
void Background::drawPixel(int row, int col, ColorInt c) {
    attribMutex.lock();
    buffer[(row * myWidth + col) * 4] = c.R;
    buffer[(row * myWidth + col) * 4 + 1] = c.G;
    buffer[(row * myWidth + col) * 4 + 2] = c.B;
    buffer[(row * myWidth + col) * 4 + 3] = c.A;
    attribMutex.unlock();
}

/*!
* \brief Destructor for the Background.
*/
Background::~Background() {
    delete [] buffer;
}

}