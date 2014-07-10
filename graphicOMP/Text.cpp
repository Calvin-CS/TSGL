/*
 * Text.cpp
 *
 *  Created on: Jul 10, 2014
 *      Author: pretzel
 */

#include "Text.h"

bool Text::isInit = false;
GLuint Text::fontTexture;
void Text::loadFonts() {
	isInit = true;
	int x,y;
	ImageLoader::loadTextureFromPNG("data/font1.png",x,y,fontTexture);
}
