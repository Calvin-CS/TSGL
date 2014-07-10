/*
 * Text.h extends Shape and provides a class for drawing a string of text to the Canvas
 *
 * Authors: Patrick Crain
 * Last Modified: Mark Vander Stel, 7/10/2014
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Shape.h"											// For extending our Shape object
#include "ImageLoader.h"

class Text : public Shape {
private:
	std::string myString;
	int myX, myY;
	static GLuint fontTexture;
	static bool isInit;
	static void loadFonts();
public:
	/*
	 * Explicit constructor for the Text class (calls the base constructor)
	 * Parameters:
	 * 		s, the string to draw
	 * 		x, the x coordinate
	 *		y, the y coordinate
	 * 		color, a color
	 * Returns: a new Text object at the specified position with the specified string and color
	 */
	Text(std::string s, int x, int y, RGBfloatType color) {
		isTextured = true;	// Let the Canvas know we're a textured object
		myString = s;
		myX = x;
		myY = y;
	}

	// draw actually draws the Text to the canvas
	void draw() {
		if (!Text::isInit)													// Load font if not already loaded
			Text::loadFonts();

		glBindTexture(GL_TEXTURE_2D, fontTexture);							// Set the current texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		float *vertices = new float[32*myString.length()];					// Allocate the vertices

		int fontSize = 9;													// Size in pixels of the rendered characters
		int charSize = 16;													// Width/height in pixels of the texture characters
		float delta = 1.0f/charSize;										// Space between characters in the texture
		float gap = delta*delta;											// Number of pixels to skip over on each side of character
		float texX, texY;													// Texture coordinates

		vertices[2]  = vertices[10]  = vertices[18] = vertices[26] = 1.0f;	// Texture color of the coords
		vertices[3]  = vertices[11]  = vertices[19] = vertices[27] = 1.0f;	// (Default to opaque white)
		vertices[4]  = vertices[12] =  vertices[20] = vertices[28] = 1.0f;
		vertices[5]  = vertices[13] =  vertices[21] = vertices[29] = 1.0f;

		vertices[1]  = myY;													// Y coordinates
		vertices[9]  = vertices[1];
		vertices[17] = vertices[1]+fontSize;
		vertices[25] = vertices[1]+fontSize;

		char c;
		for (int i = 0; i < myString.length(); i++) {
			c = myString[i];														// Get the next character
			texX = (c % charSize) / (float)charSize + gap;							// Computer its x coordinates in the texture
			texY = (c-(c % charSize)) / (float)charSize / (float)charSize + gap;	// Computer its y coordinates in the texture

			vertices[0]  = myX+fontSize*i;											// X coordinates
			vertices[8]  = vertices[0]+fontSize;
			vertices[16] = vertices[0];
			vertices[24] = vertices[0]+fontSize;

			vertices[6] = texX+gap;
			vertices[7] = texY+gap;											// Texture coords of top left
			vertices[14] = texX+delta-gap;
			vertices[15] = texY+gap;										// Texture coords of top right
			vertices[22] = texX+gap;
			vertices[23] = texY+delta-gap;									// Texture coords of bottom left
			vertices[30] = texX+delta-gap;
			vertices[31] = texY+delta-gap;									// Texture coords of bottom right

			glBufferData(GL_ARRAY_BUFFER, 32*sizeof(float), vertices, GL_DYNAMIC_DRAW);	// Fill the buffer
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);											// Draw the character
		}

		delete[] vertices;													// Free up allocated memory
	}
};

#endif /* TEXT_H_ */
