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
  Canvas c(0,0, 800, 600, 800*600);

  Canvas::setDrawBuffer(GL_RIGHT);  // For Patrick's laptop

	ImageInverter ii("pics/Car-colors.jpg", 1022, 1024);
	ii.run();

	glfwTerminate();
}
