/*
 * testGradientWheel.cpp
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
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and TSGL's colored polygons
 * \details Same principle as colorWheelFunction(). Since colored polygons take arrays as parameters
 * to allow for arbitrary-length polygons, there are some key differences:
 * - Colors, x and y coordinates are declared within the #pragma omp block so they can be
 * declared as an array
 * - At the end, drawColoredPolygon() is called on a polygon with 3 vertices, with arrays for the
 * x coordinates, y coordinates, and color.
 * \param can, Reference to the Canvas being drawn to
 */
void gradientWheelFunction(Canvas& can) {
	const int THREADS = 256,                          // Number of threads to compute with
			WINDOW_CW = can.getWindowWidth() / 2,   // Center of the screen
			WINDOW_CH = can.getWindowHeight() / 2;
	const float RADIUS = (WINDOW_CH < WINDOW_CW ? WINDOW_CH : WINDOW_CW) * .95,  // Radius of wheel
			ARCLENGTH = 2 * PI / NUM_COLORS;                                    // Gap between wedges
#pragma omp parallel num_threads(THREADS)
	{
		int nthreads = omp_get_num_threads();
		int tid = omp_get_thread_num();         // Thread ID
		int delta = NUM_COLORS / nthreads;           // Distance between threads to compute
		float shading = 1 - (float) tid / nthreads;  // Shading based on thread ID
		ColorFloat color[3];                         // RGB color to build
		int xx[3], yy[3];                       // Setup the arrays of values for vertices
		int start;
		while (can.getIsOpen()) {
			can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
			start = (NUM_COLORS - can.getReps() % NUM_COLORS + delta*tid) % NUM_COLORS;  // shapes by the location and frame

			color[0] = ColorHSV(start /                         (float) NUM_COLORS * 6, 0.0f, shading, 1.0f);
			color[1] = ColorHSV(start /                         (float) NUM_COLORS * 6, 1.0f, shading, 1.0f);
			color[2] = ColorHSV(( (start+delta) % NUM_COLORS) / (float) NUM_COLORS * 6, 1.0f, shading, 1.0f);

			xx[0] = WINDOW_CW;                                            // Set first vertex to center of screen
			yy[0] = WINDOW_CH;
			xx[1] = WINDOW_CW + RADIUS * sin(ARCLENGTH * start);          // Add the next two vertices to around the circle
			yy[1] = WINDOW_CH + RADIUS * cos(ARCLENGTH * start);
			xx[2] = WINDOW_CW + RADIUS * sin(ARCLENGTH * (start + 1));
			yy[2] = WINDOW_CH + RADIUS * cos(ARCLENGTH * (start + 1));

			can.drawColoredPolygon(3, xx, yy, color);
		}
	}
}

//Takes command line arguments for the width and height of the window
int main(int argc, char* argv[]) {
	int holder1 = atoi(argv[1]);  //Width
	int holder2 = atoi(argv[2]);  //Length
	int width, height = 0;    //Actual width and height to be used
	if (holder1 <= 0 || holder2 <= 0) {  //Check if the passed arguments are valid
		height = width = 960;  //If not, set the width and height to 960
	} else if(holder1 > WINDOW_W || holder2 > WINDOW_H) {
		height = width = 960;
	} else {
		width = holder1;  //Else, set the width and height to the passed command-line arguments
		height = holder2;
	}
	Canvas c13(0, 0, width, height, 1024, "", FRAME);
	c13.setBackgroundColor(BLACK);
	c13.start();
	gradientWheelFunction(c13);
	c13.close();
}
