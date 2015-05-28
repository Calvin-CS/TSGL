/*
 * testShadedVoronoi.cpp
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
 * \brief Draws a randomly generated Voronoi diagram with fancy shading
 * \details Same principle as voronoiFunction(). Key differences:
 * - We keep track of the second best distance to each point in \b nbdist.
 * - We keep track of the kvalues of each 2nd best point in the array \b kvalue2.
 * - In a second post-processing loop through the screen:
 *   - Find the closest and 2nd closest control points to each pixel.
 *   - Find the distance from the pixel to the closest control point and store it in: \b d1.
 *   - Find the distance from the closest to the 2nd closest control point and store it in: \b kd.
 *   - Set \b shading to ( \b d1 / \b kd ).
 *   - Bind \b shading between 0 and 1, and shade the pixel with \b shading.
 *   - Break if the Canvas is closed.
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void shadedVoronoiFunction(Canvas& can) {
	const int WINDOW_W = can.getWindowWidth(),        // Set the screen sizes
			WINDOW_H = can.getWindowHeight(),
			POINTS = 100;                         // Set the number of control points
	srand(time(NULL));                              // Seed the random number generator
	int* x = new int[POINTS]();                     // Initialize an array for POINTS x coords
	int* y = new int[POINTS]();                     // Do the same for y coords
	int* kvalue = new int[WINDOW_W * WINDOW_H]();   // Create a mapping of control point values
	int* kvalue2 = new int[WINDOW_W * WINDOW_H]();  // Create a mapping of more control point values
	ColorFloat color[POINTS];                     // And for an array of colors
	ColorFloat tc, rc, lc, bc, xc, yc;            // Color for the top, right, left, bottom, x-average, and y-average
	float bdist, nbdist, dist, xd, yd;              // Keep track of the closes matches and current distances
	int bestk = -1, nextbestk = -1;                 // Keep track of the current best k-value
	for (int i = 0; i < POINTS; i++) {              // Randomize the control points
		x[i] = rand() % WINDOW_W;
		y[i] = rand() % WINDOW_H;
	}
	srand(time(NULL));
	tc = Colors::randomColor(1.0f);                       // Randomize the axis colors
	rc = Colors::randomColor(1.0f);
	lc = Colors::randomColor(1.0f);
	bc = Colors::randomColor(1.0f);
	for (int i = 0; i < POINTS; i++) {              // For each control point...
		float xx = (float) x[i] / WINDOW_W;         // Calculate an value from 0:1 based on x coord
		float yy = (float) y[i] / WINDOW_H;         // Do the same for y
		xc = Colors::blendedColor(lc, rc, xx);              // Interpolate between the left and right colors
		yc = Colors::blendedColor(tc, bc, yy);              // Do the same for top and bottom
		color[i] = Colors::blendedColor(xc, yc, 0.5f);      // Complete the 4-way interpolation
		//        color[i] = randomColor(rand());           // Or random colors, if you prefer
	}
	for (int i = 0; i < WINDOW_W; i++) {            // For each individual point...
		for (int j = 0; j < WINDOW_H; j++) {
			bdist = nbdist = 9999;                  // Reset the best distance
			for (int k = 0; k < POINTS; k++) {      // Find the closest control point
				xd = i - x[k];                      // Calculate the distance from each control point
				yd = j - y[k];
				dist = sqrt(xd * xd + yd * yd);
				if (dist < bdist) {                 // If it's the closest one
					nbdist = bdist;                 // Update the next best distance and control point
					nextbestk = bestk;
					bdist = dist;                   // Update the best distance and control point
					bestk = k;
				} else if (dist < nbdist) {         // If it's the second closest one
					nbdist = dist;                  // Just update the next best distance / CP
					nextbestk = k;
				}
			}
			kvalue[i * WINDOW_H + j] = bestk;
			kvalue2[i * WINDOW_H + j] = nextbestk;
			can.drawPoint(i, j, color[bestk]);      // Draw the point with the closest control's color
			if (!can.getIsOpen()) break;
		}
	}
	for (int i = 0; i < WINDOW_W; i++) {            // For each individual point...
		for (int j = 0; j < WINDOW_H; j++) {
			int k = kvalue[i * WINDOW_H + j];       // Find its closest control point
			int nk = kvalue2[i * WINDOW_H + j];     // Then find its second closest
			float xd1 = i - x[k];
			float yd1 = j - y[k];
			float d1 = sqrt(xd1 * xd1 + yd1 * yd1); // Find the distance to it closest
			float xkd = x[k] - x[nk];
			float ykd = y[k] - y[nk];
			float kd = sqrt(xkd * xkd + ykd * ykd); // Find the distance between the CPs themselves
			float shading = d1 / kd;
			if (shading > 1)
				shading = 1;
			else if (shading < 0) shading = 0;
			can.drawPoint(i, j, ColorFloat(0.0f, 0.0f, 0.0f, shading));  // Draw the point with the closest control's color

			if (!can.getIsOpen()) break;
		}
	}
	delete x;
	delete y;
	delete kvalue;
	delete kvalue2;
}

//Takes in command line arguments for the width and height of the window
int main(int argc, char* argv[]) {
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
	Canvas c19(0, 0, width, height, BUFFER, "");
	c19.setBackgroundColor(WHITE);
	c19.start();
	shadedVoronoiFunction(c19);
	c19.close();
}

