/*
 * Image.h extends Shape and provides a class for drawing an image to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>

#include "Shape.h"		    // For extending our Shape object
#include "ImageLoader.h"    // For loading images
class Image : public Shape {
 private:
    float vertices[32];
    std::string myFile;
    ImageLoader myLoader;
 public:
    Image(std::string f, ImageLoader& loader, int x, int y, int w, int h, float a);

    // draw() actually draws the Image to the canvas
    void draw();
};

#endif /* IMAGE_H_ */
