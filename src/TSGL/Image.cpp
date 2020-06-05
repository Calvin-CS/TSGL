// From stb_image.h:
// Do this:
//   #define STB_IMAGE_IMPLEMENTATION
// before you include this file in *one* C or C++ file to create the implementation.
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
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
Image::Image(float x, float y, float z, std::string filename, GLfloat width, GLfloat height, float yaw, float pitch, float roll/* , float alpha */) : Drawable(x,y,z,yaw,pitch,roll) {
    if (width <= 0 || height <= 0) {
        TsglDebug("Cannot have an Image with width or height less than or equal to 0.");
        return;
    }
    myWidth = width; myHeight = height;
    myXScale = width; myYScale = height;
    numberOfVertices = numberOfOutlineVertices = 4;
    myFile = filename;

	// Load the image.
	// image = getBMP(filename);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filename.c_str(), &pixelWidth, &pixelHeight, 0, 4);
    assert(data);
    if (!data)
    {
        TsglDebug("Failed to load texture");
    }
    glEnable(GL_TEXTURE_2D);

    // create the Image's texture id
    glGenTextures(1, &myTexture);

    // vertex allocation and assignment
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];

    addVertex(-0.5,-0.5,0,RED);
    addVertex(-0.5,0.5,0,BLUE);
    addVertex(0.5,0.5,0,CYAN);
    addVertex(0.5,-0.5,0,GREEN);
}

 /*!
  * \brief Draw the Image.
  * \details This function actually draws the Image to the Canvas.
  */
void Image::draw() {
    if (!init) {
        TsglDebug("Vertex buffer is not full.");
        return;
    }
    // enable textures and bind the texture id
    glBindTexture(GL_TEXTURE_2D, myTexture);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // actually generate the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pixelWidth, pixelHeight, 0,
	           	 GL_RGBA, GL_UNSIGNED_BYTE, data);

    // transformation matrix
    glPushMatrix();
    glTranslatef(myRotationPointX, myRotationPointY, myRotationPointZ);
    glRotatef(myCurrentYaw, 0, 0, 1);
    glRotatef(myCurrentPitch, 0, 1, 0);
    glRotatef(myCurrentRoll, 1, 0, 0); 
    glTranslatef(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ);
    glScalef(myXScale, myYScale, myZScale);

    // enable necessary states (vertex, color, and texture)
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // link arrays to their respective opengl pointers
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    // draw the image
    glDrawArrays(GL_QUADS, 0, numberOfVertices);

    // pop transformation matrix
    glPopMatrix();

    /* Cleanup states */
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Image::setColor(ColorFloat c[]) {
    for (int i = 0; i < numberOfVertices; i++) {
        colors[i*4] = c[i].R;
        colors[i*4+1] = c[i].G;
        colors[i*4+2] = c[i].B;
        colors[i*4+3] = c[i].A;
    }
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

// /*!
//  * \brief Alters the file the Image draws.
//  * \details Alters the values of the myFile, myWidth, myHeight, and mutates vertices.
//  * \param filename New string value for myFile.
//  * \param width New width of the Image.
//  * \param height New height of the Image.
//  */
// void Image::changeFileName(std::string filename, int width, int height) {
//   attribMutex.lock();
//   myFile = filename;
//   myWidth = width; myHeight = height;
//   if (myWidth <= 0 || myHeight <= 0) {
//     TextureHandler::getDimensions(filename,myWidth,myHeight);
//   }
//   vertices[0] = myCenterX - myWidth/2;
//   vertices[1] = myCenterY - myHeight/2;
//   vertices[8] = myCenterX + myWidth/2;
//   vertices[9] = myCenterY - myHeight/2;
//   vertices[16] = myCenterX - myWidth/2;
//   vertices[17] = myCenterY + myHeight/2;
//   vertices[24] = myCenterX + myWidth/2;
//   vertices[25] = myCenterY + myHeight/2;
//   attribMutex.unlock();
// }

Image::~Image() { 
    glDeleteTextures(1, &myTexture);
    //  delete image; 
    stbi_image_free(data); 
}


}
