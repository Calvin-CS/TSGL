/*
 * Image.h extends Drawable and provides a class for drawing an image to a Canvas.
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>

#include "Drawable.h"           // For extending our Drawable object
// #include "getBMP.h"
#include "TsglAssert.h"      // For unit testing purposes

namespace tsgl {

/*! \class Image
 *  \brief Draw an image to the Canvas.
 *  \details Image is a class which provides a simple interface for loading and drawing images.
 *   The Image class currently supports files in the .png, .bmp, and .jpg formats.
 *  \note For the time being, there is no way to measure the size of an image once it's loaded.
 *   Therefore, the width and height must be specified manually, and stretching may occur if the
 *   input dimensions don't match the images actual dimensions.
 *  \warning Aside from an error message output to stderr, Image gives no indication if an image failed to load.
 */
class Image : public Drawable {
 private:
    // imageFile * image;
    unsigned char * data = 0;
    GLfloat myWidth, myHeight;
    GLint pixelWidth, pixelHeight;
    std::string myFile;
    GLuint myTexture;
    GLfloat texcoords[8] = 
    {
        0, 0,   0, 1,   1, 1,   1, 0
    };
 public:
    Image(float x, float y, float z, std::string filename, GLfloat width, GLfloat height, float yaw, float pitch, float roll/* , float alpha = 1.0f */);

    virtual void draw();

    /*!
     * \brief Accessor for the image's height.
     * \return The height of the Image.
     */
    GLfloat getHeight() { return myHeight; }

    /*!
     * \brief Accessor for the image's width.
     * \return The width of the Image.
     */
    GLfloat getWidth() { return myWidth; }

    void setWidth(GLfloat width);

    void setHeight(GLfloat height);

    void changeWidthBy(GLfloat delta);

    void changeHeightBy(GLfloat delta);

    GLint getPixelHeight() { return pixelHeight; }

    GLint getPixelWidth() { return pixelWidth; }

    void setColor(ColorFloat c[]);

    ~Image();
};

}

#endif /* IMAGE_H_ */