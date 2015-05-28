/*
 * testColorPoints.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

// Some constants that get used a lot
const int NUM_COLORS = 256, MAX_COLOR = 255;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

float randfloat(int divisor = 10000) {
	return (rand() % divisor) / (float) divisor;
}

/*!
 * \brief Draws a neat pattern of points to a canvas using OMP and takes in a command line
 * argument for the number of threads to use
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in: \b nthreads
 * - Check if the passed argument for the number of threads is valid:
 *   - If the argument is less than or equal to 0, use the number of threads that we can use with OMP.
 *   - If the argument greater than the number of threads that we can use, then just use the number of threads that we can use with OMP.
 *   - Else, the argument is valid and we can use that number of threads in the function.
 * - The number of lines per thread is calculated and stored in: \b myPart
 * - The starting position of each given thread is calculated and stored in: \b myStart
 * - The outer for loop is set up in a block pattern, and the inner for loop runs from 100 to the Canvas width - 100
 * - Some dark voodoo magic is used to calculate the color for a given point (we aren't really sure yet how the color for a given point is calculated)
 * - The point is drawn to the Canvas
 * - The function breaks from the outer for loop if the Canvas is closed
 * .
 * \param can, Reference to the Canvas being drawn to
 * \param numberOfThreads, Reference to the number of threads to use
 */
void colorPointsFunction(Canvas& can, int & numberOfThreads) {
#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int holder = omp_get_num_threads();  //Temp variable
		int nthreads = 0;  //Actual number of threads to use
		if (numberOfThreads <= 0) {   //If the argument is <= 0, its not valid
			nthreads = holder;   //Use the number of threads that we can use with OMP
		} else if(numberOfThreads > holder) {  //Same if the argument is > than the number of threads we can use with OMP
			nthreads = holder;
		} else {
			nthreads = numberOfThreads;  //Else, it's valid so we can use that many threads in the function
		}
		int myPart = can.getWindowHeight() / nthreads;
		int myStart = myPart * omp_get_thread_num();
		for (int j = myStart; j < myStart + myPart; j++) {
			for (int i = 100; i < can.getWindowWidth() - 100; i++) {
				if (i % 2 == 0)
					can.drawPoint(i, j, ColorInt(j % NUM_COLORS, i % NUM_COLORS, (i * j) % 113));
				else
					can.drawPoint(i, j, ColorInt(i % NUM_COLORS, j % NUM_COLORS, (i * j) % NUM_COLORS));
			}
			if (!can.getIsOpen()) break;
		}
	}
}

//Takes in command line arguments for the window width and height as well
//as for the number of threads to use
int main(int argc, char* argv[]) {
	glfwInit();  // Initialize GLFW
	int holder1 = atoi(argv[1]);  //Width
	int holder2 = atoi(argv[2]);  //Height
	int width = 0;
	int height = 0;
	if (holder1 <= 0 || holder2 <= 0) {   //Check the passed width and height and see if they are valid
		width = WINDOW_W;  //If not, use the default width and height
		height = WINDOW_H;
	} else if(holder1 > WINDOW_W || holder2 > WINDOW_H || holder1 <= 200) {    //Caught a weird situation where if the width was less than or equal to 200 then the pattern would not show up.
		width = WINDOW_W;
		height = WINDOW_H;
	} else {
		width = holder1;  //Else they are valid, use them
		height = holder2;
	}
	Canvas c1(0, 0, width, height, BUFFER, "");
	int numberOfThreads = atoi(argv[3]);    //Convert the char pointer to an int, http://www.cplusplus.com/forum/beginner/58493/
	c1.setBackgroundColor(GREY);
	c1.start();
	colorPointsFunction(c1, numberOfThreads);   //Now pass the argument for the number of threads to the test function
	c1.close();
	glfwTerminate();  // Release GLFW
}
