/*
 * Image.h extends Shape and provides a class for drawing an image to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/8/2014
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "Shape.h"							// For extending our Shape object
#include "ImageLoader.h"					// For loading images

typedef std::unordered_map<std::string,GLuint>	textureMap;

class Image : public Shape {
private:
	float vertices[32];
	std::string myFile;
	static textureMap loadedTextures;
public:
	Image(std::string f, int x, int y, int w, int h);

	// draw() actually draws the Image to the canvas
	void draw();
};

#endif /* IMAGE_H_ */
