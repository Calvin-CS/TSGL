/*
 * Image.h extends Drawable and provides a class for drawing an image to a Canvas.
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>

#include "Drawable.h"           // For extending our Drawable object
#include "TextureHandler.h"  // For loading images
#include "TsglAssert.h"      // For unit testing purposes

namespace tsgl {

/*! \class Image
 *  \brief Draw an image to the Canvas.
 *  \details Image is a class which provides a simple interface for loading and drawing images.
 *   The Image class currently supports files in the .png, .bmp, and .jpg formats.
 *  \note For the time being, there is no way to measure the size of an image once it's loaded.
 *   Therefore, the width and height must be specified manually, and stretching may occur if the
 *   input dimensions don't match the images actual dimensions.
 *  \note Additionally, an ImageLoader must be passed as an argument. This ImageLoader is automatically
 *   constructed with the Canvas as the private *loader* variable. At the moment, there is no way to
 *   extend Canvas::drawImage() function due to this privatization.
 *  \warning Aside from an error message output to stderr, Image gives no indication if an image failed to load.
 */
class Image : public Drawable {
 private:
    int myWidth, myHeight;
    float vertices[32];
    std::string myFile;
    GLtexture myTexture;
    TextureHandler* myLoader;
 public:

    /*!
     * \brief Explicitly constructs a new Image.
     * \details This is the explicit constructor for the Image class.
     *   \param filename The filename of the image to load.
     *   \param loader A reference pointer to the TextureHandler with which to load the image.
     *   \param x The x coordinate of the left of the Image.
     *   \param y The y coordinate of the top of the Image.
     *   \param width The width of the Image.
     *   \param height The height of the Image.
     *   \param alpha The alpha of the Image.
     * \return A new Image is drawn with the specified coordinates, dimensions, and transparency.
     * \note <B>IMPORTANT</B>: In CartesianCanvas, *y* specifies the bottom, not the top, of the image.
     */
    Image(std::string filename, TextureHandler &loader, int x, int y, int width, int height, float alpha);

    /*!
     * \brief Draw the Image.
     * \details This function actually draws the Image to the Canvas.
     */
    void draw();

    /*!
     * \brief Accessor for the image's height.
     * \return The height of the Image.
     */
    int getHeight() { return myHeight; }

    /*!
     * \brief Accessor for the image's width.
     * \return The width of the Image.
     */
    int getWidth() { return myWidth; }
};

}

#endif /* IMAGE_H_ */
