// /*
//  * Image.h extends Shape and provides a class for drawing an image to a Canvas.
//  */

// #ifndef IMAGE_H_
// #define IMAGE_H_

// #include <string>

// #include "Drawable.h"           // For extending our Shape object
// #include "TextureHandler.h"  // For loading images
// #include "TsglAssert.h"      // For unit testing purposes

// namespace tsgl {

// /*! \class Image
//  *  \brief Draw an image to the Canvas.
//  *  \details Image is a class which provides a simple interface for loading and drawing images.
//  *   The Image class currently supports files in the .png, .bmp, and .jpg formats.
//  *  \note For the time being, there is no way to measure the size of an image once it's loaded.
//  *   Therefore, the width and height must be specified manually, and stretching may occur if the
//  *   input dimensions don't match the images actual dimensions.
//  *  \note Additionally, an ImageLoader must be passed as an argument. This ImageLoader is automatically
//  *   constructed with the Canvas as the private *loader* variable. At the moment, there is no way to
//  *   extend Canvas::drawImage() function due to this privatization.
//  *  \warning Aside from an error message output to stderr, Image gives no indication if an image failed to load.
//  */
// class Image : public Drawable {
//  private:
//     int myWidth, myHeight;
//     float currentRotation;
//     float *vertices;
//     std::string myFile;
//     GLtexture myTexture;
//     TextureHandler* myLoader;
//  public:
//     Image(std::string filename, TextureHandler &loader, int x, int y, int width, int height, float alpha = 1.0f);

//     virtual void draw();

//     /*!
//      * \brief Accessor for the image's height.
//      * \return The height of the Image.
//      */
//     int getHeight() { return myHeight; }

//     /*!
//      * \brief Accessor for the image's width.
//      * \return The width of the Image.
//      */
//     int getWidth() { return myWidth; }

//     void setCenter(float x, float y);

//     void moveImageBy(float deltaX, float deltaY);

//     virtual void setRotation(float radians);

//     void changeFileName(std::string filename, int width = 0, int height = 0);

//     ~Image() { delete[] vertices; }

// };

// }

// #endif /* IMAGE_H_ */