/*
 * testGradientMandelbrot.cpp
 *
 *  Created on: May 27, 2015
 *      Author: Chris Dilley
 */

#include "Mandelbrot/GradientMandelbrot.h"

/*!
 * \brief Draws a gradient Mandelbrot set on a CartesianCanvas
 * \details Same as mandelbrotFunction(), but with smoother shading ( see
 * http://linas.org/art-gallery/escape/smooth.html ).
 * \param can, Reference to the CartesianCanvas being drawn to
 * \param numberOfThreads, Reference to the number of threads to use
 */
void gradientMandelbrotFunction(CartesianCanvas& can, unsigned int & numberOfThreads) {
	GradientMandelbrot m1;
	m1.bindings(can);
	m1.draw(can, numberOfThreads);
}

//Takes in command line argument for the number of threads
int main(int argc, char* argv[]) {
	Cart c16(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, BUFFER, "", FRAME / 2);
	unsigned int numberOfThreads = atoi(argv[1]);    //Get the number of threads to use
	c16.setBackgroundColor(GREY);
	c16.start();
	gradientMandelbrotFunction(c16, numberOfThreads);
	c16.close();
}
