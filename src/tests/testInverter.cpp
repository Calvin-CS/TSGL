///*
// * main.cpp is a driver for an ImageInverter program.
// *
// *  Created on: Nov 14, 2014
// *      Author: adams
// */
//
#include "ImageInverter.h"

int main() {
  glfwInit();

//  Canvas::setDrawBuffer(GL_LEFT);  // Uncomment for ULab computers
  Canvas::setDrawBuffer(GL_FRONT_AND_BACK);  // Uncomment for Patrick's laptop

	ImageInverter ii("pics/Car-colors.jpg", 1022, 1024);
	ii.run();

	glfwTerminate();
}
