#include "Image.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Image.
  * \details This is the explicit constructor for the Image class.
  *   \param x The x coordinate of the center of the Image.
  *   \param y The y coordinate of the center of the Image.
  *   \param z The z coordinate of the center of the Image.
  *   \param filename The filename of the image to load, a std::string
  *   \param width The width of the Image.
  *   \param height The height of the Image.
  *   \param yaw The yaw orientation of the Image.
  *   \param pitch The pitch orientation of the Image.
  *   \param roll The roll orientation of the Image. 
  *   \param alpha The alpha of the Image.
  * \return A new Image is drawn with the specified coordinates, dimensions, and transparency.
  * \note <B>IMPORTANT</B>: In CartesianCanvas, *y* specifies the bottom, not the top, of the image.
  */
Image::Image(float x, float y, float z, std::string filename, GLfloat width, GLfloat height, float yaw, float pitch, float roll, float alpha) : Drawable(x,y,z,yaw,pitch,roll) {
    if (width <= 0 || height <= 0) {
        TsglDebug("Cannot have an Image with width or height less than or equal to 0.");
        return;
    }
    shaderType = IMAGE_SHADER_TYPE;
    myWidth = width; myHeight = height;
    myXScale = width; myYScale = height;
    myFile = filename;

	// Load the image.
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename.c_str(), &pixelWidth, &pixelHeight, 0, 4);
    tsglAssert(data, "stbi_load(filename) failed.");
    glEnable(GL_TEXTURE_2D);

    // vertex allocation and assignment
    vertices = new GLfloat[30];

    // positions (x,y,z)    texture coords
    // 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
    // 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
    //-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    // second triangle
    // 0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
    //-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
    //-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    vertices[0]  = vertices[1]  = vertices[5]  = vertices[15] = vertices[16] = vertices[26] = 0.5; // x + y
    vertices[6] = vertices[10] = vertices[11] = vertices[20] = vertices[21] = vertices[25] = -0.5; // x + y
    vertices[2] = vertices[7] = vertices[12] = vertices[17] = vertices[22] = vertices[27] = 0; // z
    vertices[3]  = vertices[4]  = vertices[8] = vertices[18] = vertices[19] = vertices[29] = 1.0; // texture coord x + y
    vertices[9] = vertices[13] = vertices[14] = vertices[23] = vertices[24] = vertices[28] = 0.0; // texture coord x + y
    init = true;
}

 /*!
  * \brief Draw the Image.
  * \details This function actually draws the Image to the Canvas.
  */
void Image::draw(Shader * shader) {
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelWidth, pixelHeight, 0,
	           	 GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteTextures(1, &myTexture);

    glDisable(GL_TEXTURE_2D);
}

/**
 * \brief Mutates the distance from the left side of the Image base to its right side.
 * \param width The Image's new width.
 */
void Image::setWidth(GLfloat width) {
    if (width <= 0) {
        TsglDebug("Cannot have an Image with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth = width;
    myXScale = width;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the left side of the Image base to its right side by the parameter amount.
 * \param delta The amount by which to change the width of the Image.
 */
void Image::changeWidthBy(GLfloat delta) {
    if (myWidth + delta <= 0) {
        TsglDebug("Cannot have an Image with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth += delta;
    myXScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the top side of the Image base to its bottom side.
 * \param height The Image's new height.
 */
void Image::setHeight(GLfloat height) {
    if (height <= 0) {
        TsglDebug("Cannot have an Image with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth = height;
    myYScale = height;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the top side of the Image base to its bottom side by the parameter amount.
 * \param delta The amount by which to change the height of the Image.
 */
void Image::changeHeightBy(GLfloat delta) {
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have an Image with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight += delta;
    myYScale += delta;
    attribMutex.unlock();
}

/*!
 * \brief Alters the file the Image draws.
 * \details Alters the values of the myFile, myWidth, myHeight, and mutates vertices.
 * \param filename New string value for myFile.
 * \param width New width of the Image.
 * \param height New height of the Image.
 */
void Image::changeFile(std::string filename) {
    attribMutex.lock();
    init = false;
    stbi_image_free(data); 
	// Load the image.
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename.c_str(), &pixelWidth, &pixelHeight, 0, 4);
    tsglAssert(data, "stbi_load(filename) failed.");
    init = true;
    attribMutex.unlock();
}

Image::~Image() { 
    glDeleteTextures(1, &myTexture);
    stbi_image_free(data); 
}


}
