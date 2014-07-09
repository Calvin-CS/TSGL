/*
 * Image.h extends Shape and provides a class for drawing an image to a Canvas
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/9/2014
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <unordered_map>

#include "Shape.h"							// For extending our Shape object
#include "ImageLoader.h"					// For loading images

class Image : public Shape {
private:
	float vertices[32];
	GLuint myTexture;
	std::string myFile;
	static std::unordered_map<std::string,GLuint> loadedTextures;
public:
	/*
	 * Explicit constructor for the Image class
	 * Parameters:
	 * 		f, the filename of the image to load from
	 * 		x, the x coordinate of the Image's left edge
	 *		y, the y coordinate of the Image's top edge
	 * 		w, the width of the Image
	 *		h, the height of the Image
	 * Returns: a new Image with the specified top left corner and dimensions
	 */
	Image(std::string f, int x, int y, int w, int h) {
		isTextured = true;	// Let the Canvas know we're a textured object
		myFile = f;
		vertices[0]  = x;
		vertices[1]  = y;
		vertices[8]  = x+w;
		vertices[9]  = y;
		vertices[16] = x;
		vertices[17] = y+h;
		vertices[24] = x+w;
		vertices[25] = y+h;
		vertices[2]  = vertices[10]  = vertices[18] = vertices[26] = 1.0f;
		vertices[3]  = vertices[11]  = vertices[19] = vertices[27] = 1.0f;
		vertices[4]  = vertices[12] = vertices[20] = vertices[28] = 1.0f;
		vertices[5]  = vertices[13] = vertices[21] = vertices[29] = 1.0f;
		vertices[6] = vertices[7] = 0.0f;
		vertices[14] = 1.0f, vertices[15] = 0.0f;
		vertices[22] = 0.0f, vertices[23] = 1.0f;
		vertices[30] = vertices[31] = 1.0f;
	}

	// draw() actually draws the Image to the canvas
	void draw() {
		int w, h;
		if (Image::loadedTextures.find (myFile) == loadedTextures.end() ) {	// Load the image if we haven't already
			std::string extension = myFile.substr(myFile.find_last_of('.'),4);
			if (extension == ".png")
				loadedTextures[myFile] = ImageLoader::loadTextureFromPNG(myFile.c_str(),w,h,myTexture);
			if (extension == ".bmp")
				loadedTextures[myFile] = ImageLoader::loadTextureFromBMP(myFile.c_str(),w,h,myTexture);
		}
		else
			myTexture = loadedTextures[myFile];
		glBindTexture(GL_TEXTURE_2D, myTexture);
		std::cout << myTexture << std::endl << std::flush;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBufferData(GL_ARRAY_BUFFER, 32*sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
};

#endif /* IMAGE_H_ */
