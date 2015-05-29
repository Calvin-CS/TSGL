/*
 * Mandelbrot.cpp
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
 *(NEEDS DOCUMENTATION)
 */

//Imports, constants....
#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

const int MAX_COLOR = 255;

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

class Mandelbrot {
private:
	int myThreads;
	unsigned int myDepth;
	Decimal myFirstX, myFirstY, mySecondX, mySecondY;
	bool myRedraw;

public:
Mandelbrot() {
		myThreads = 8;
		myDepth = MAX_COLOR;
		myFirstX = myFirstY = mySecondX = mySecondY = 0.0;
		myRedraw = true;
}

void bindings(CartesianCanvas& can) {
		can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &myRedraw]() {
			can.clear();
			myRedraw = true;
		});
		can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, &myFirstX, &myFirstY]() {
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), myFirstX, myFirstY);
		});
		can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, &myFirstX, &myFirstY, &mySecondX, &mySecondY, &myRedraw]() {
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), mySecondX, mySecondY);
			can.zoom(myFirstX, myFirstY, mySecondX, mySecondY);
			myRedraw = true;
		});
		can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, &myRedraw]() {
			Decimal x, y;
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
			can.zoom(x, y, 1.5);
			myRedraw = true;
		});
		can.bindToScroll([&can, &myRedraw](double dx, double dy) {
			Decimal x, y;
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
			Decimal scale;
			if (dy == 1) scale = .5;
			else scale = 1.5;
			can.zoom(x, y, scale);
			myRedraw = true;
		});
	}

void draw(CartesianCanvas& can, unsigned int & numberOfThreads) {
		while(myRedraw) {
			setRedraw(false);
			can.reset();
#pragma omp parallel num_threads(myThreads)
{
	unsigned int holder = omp_get_num_threads();  //Temp variable
	unsigned int nthreads = 1;   //Actual number of threads
	if (numberOfThreads > holder || numberOfThreads == 0) {  //Check if the passed number of threads is valid
		nthreads = holder;  //If not, use the number of threads that we can use with OMP
	} else {
		nthreads = numberOfThreads;  //Else, use that many threads
	}
	double blocksize = can.getCartHeight() / nthreads;
	double blockheight = can.getWindowHeight() / nthreads;
	for(unsigned int k = 0; k <= blockheight && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
		long double row = blocksize * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
		for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
			complex originalComplex(col, row);
			complex c(col, row);
			unsigned iterations = 0;
			while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
				iterations++;
				c = c * c + originalComplex;
			}
			if(iterations == myDepth) { // If the point never escaped, draw it black
				can.drawPoint(col, row, BLACK);
			} else { // Otherwise, draw it with color based on how long it took
				can.drawPoint(col, row,
						ColorInt(iterations % 151,
								((iterations % 131) + omp_get_thread_num() * 128 / nthreads) % 255,
								iterations % 255));
			}
			if (myRedraw) break;
		}
		if (myRedraw) break;
	}
}
std::cout << can.getTime() << std::endl;
while (can.getIsOpen() && !myRedraw)
	can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
		}

	}

	//mutator
void setRedraw(bool newValue) {
		myRedraw = newValue;
	}

};
