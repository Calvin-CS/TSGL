/*
 * testCosineIntegral.cpp
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

// Shared values between langton functions
enum direction {
	UP = 0,
	RIGHT = 1,
	DOWN = 2,
	LEFT = 3
};

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

const int IPF = 1000;  //For those functions that need it

float randfloat(int divisor = 10000) {
	return (rand() % divisor) / (float) divisor;
}

/*!
 * \brief Draws the area under a predefined function (the integral) using CartesianCanvas
 * and takes a command line argument for the number of threads to use
 * \details
 * - Use a predetermined number of threads, storing it in: \b THREADS.
 * - Bind Q's press event to quit the rendering in case it takes too long.
 * - Set up the internal timer of the Canvas to expire once every \b FRAME / 2 seconds
 * - Draw axes through the origin, with spacing PI/4 between x ticks and 0.5 between y ticks.
 * - Store the width of the canvas's pixel in \b pw to avoid thousands of multiple function calls.
 * - Initialize and draw a CosineFunction using the currently rendered area of the CartesianCanvas.
 * - Set the CartesianCanvas' font from an external font file using setFont().
 * - Draw some labels on the CartesianCanvas to make things look pretty.
 * - Set up a parallel block with OMP using \b THREADS threads.
 * - Check if the argument for the number of threads is valid:
 *   - If it is less than or equal to 0, use the number of threads that we can use with OMP.
 *   - If it is greater than the number of threads that we can use, use only the number of threads that we can use with OMP.
 *   - Else, its valid and use that many threads.
 * - Set \b nthreads to the actual number of threads spawned.
 * - Calculate each thread's share of the work and store it in: \b offset.
 * - Calculate each thread's starting position and store it in: \b start.
 * - Calculate each thread's stopping position and store it in: \b stop.
 * - For each thread, from \b start to \b stop with step size \b pw:
 *   - If the Canvas was closed, break.
 *   - Sleep the internal timer until it's ready to render.
 *   - Draw a line from x,0 to x,f(x) for the current x.
 *   .
 * .
 * \param can Reference to the CartesianCanvas being drawn to
 * \param numberOfThreads, Reference to the number of threads to use
 */
void cosineIntegralFunction(CartesianCanvas& can, int & numberOfThreads) {
    int threads = 0;
	if (numberOfThreads < 0) {
		numberOfThreads *= -1;
	}
	threads = numberOfThreads;
	can.bindToButton(TSGL_Q, TSGL_PRESS, [&can](){  // Quit on press of Q
		can.end();
	});

	can.drawAxes(0, 0, PI/4, .5);
	long double pw = can.getPixelWidth();
	CosineFunction function1;
	can.drawFunction(function1);

	can.setFont("assets/freefont/FreeSerif.ttf");
	can.drawText(L"-1.5π", -1.5 * PI - .1, .25, 20);  // Note the important capital L, used to support Unicode.
	can.drawText(L"1.5\u03C0", 1.5 * PI - .2, .25, 20);
	can.drawText(L"1", .1, 1.05, 20);
	can.drawText(L"-1", .1, -1.1, 20);

#pragma omp parallel num_threads(threads)
	{
		int nthreads = omp_get_num_threads();   //Temp variable
		long double offset = 3*PI / nthreads;
		long double start = -1.5*PI + omp_get_thread_num() * offset;
		long double stop = start + offset;
		for (long double i = start; i < stop; i += pw) {
			if (!can.getIsOpen()) break;
			can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
			can.drawLine(i, 0, i, function1.valueAt(i), Colors::highContrastColor(omp_get_thread_num()));
		}
	}
}

//Takes in command line arguments for the width and height of the window
//as well as the number of threads to use
int main(int argc, char* argv[]) {
	glfwInit();  // Initialize GLFW
	int holder1 = atoi(argv[1]);   //Width
	int holder2 = atoi(argv[2]);   //Height
	int width, height = 0;
	if (holder1 <= 0 || holder2 <= 0) {    //Checked the passed width and height if they are valid
		width = height = 960;  //If not, set the width and height to a default value
	} else if(holder1 > WINDOW_W || holder2 > WINDOW_H) {
		width = height = 960;
	} else {
		width = holder1;  //Else, they are and so use them
		height = holder2;
	}
	Cart c12(0, 0, width, height, -5,-1.5,5,1.5, 16000, "", FRAME / 2);
    int numberOfThreads = atoi(argv[3]);   //Number of threads to use
    c12.setBackgroundColor(WHITE);
	c12.start();
	cosineIntegralFunction(c12, numberOfThreads);   //Pass the argument
	c12.close();
	glfwTerminate();  // Release GLFW
}
