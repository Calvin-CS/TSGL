/*
 * Image.h extends Shape and provides a class for drawing an image to a Canvas.
 *
 * Authors: Patrick Crain, Mark Vander Stel, Chris Dilley.
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>

#include "Shape.h"           // For extending our Shape object
#include "TextureHandler.h"  // For loading images
#include "TsglAssert.h"      // For unit testing purposes

#ifdef _WIN32
namespace tsgl {
#endif

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
class Image : public Shape {
 private:
    float vertices[32];
    std::string myFile;
    TextureHandler* myLoader;
 public:
    /*!
     * \brief Explicitly constructs a new Image.
     * \details This is the explicit constructor for the Image class.
     *      \param f The filename of the image to load.
     *      \param loader A pointer to the TextureHandler with which to load the image.
     *      \param x The x coordinate of the left of the Image.
     *      \param y The y coordinate of the top of the Image.
     *      \param w The width of the Image.
     *      \param h The height of the Image.
     *      \param a The alpha of the Image.
     * \return A new Image drawn with the specified coordinates, dimensions, and transparency.
     * \note <B>IMPORTANT</B>: In CartesianCanvas, *y* specifies the bottom, not the top, of the image.
     */
    Image(std::string f, TextureHandler &loader, int x, int y, int w, int h, float a);

    /*!
     * \brief Draw the Image.
     * \details This function actually draws the Image to the Canvas.
     */
    void draw();

    /*!
     *
     */
    static void runTests();
};

#ifdef _WIN32
}
#endif

#endif /* IMAGE_H_ */
