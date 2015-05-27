/*
 * testVoronoi.cpp
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
 * \brief Draws a randomly generated Voronoi diagram, using OMP and private variables
 * ( see http://en.wikipedia.org/wiki/Voronoi_diagram ).
 * \details
 * - The Canvas's dimensions are stored in a local variable.
 * - The number of control points are predetermined and stored in \b POINTS.
 * - We seed the random number generator with the time.
 * - We allocate arrays for x and y coordinates for each of the points.
 * - We allocate an array of k-values for each pixel on the Canvas
 * - We declare an array of colors for each of the points
 * - We initialize variables for the top, right, left, bottom, x-center, and y-center colors
 * - The best k-value is initially set to 0.
 * - We initialize variables keeping track of the best and current distances.
 * - We randomize the locations of the control points.
 * - We randomize the colors of key areas of the diagram.
 * - For each control point:
 *   - We get its x coordinate and y coordinate
 *   - We determine its x-color based on a linear interpolation on the x-axis.
 *   - We determine its y-color based on a linear interpolation on the y-axis.
 *   - We determine its color based on an even mixture of its x-color and y-color.
 *   .
 * - We start a parallel OMP block, allowing the system to determine the best means of parallelization
 * - For each column, we set the best k to 0. Then:
 *   - For each row:
 *     - For each point:
 *       - We calculate the distance from row,col to the control point
 *       - If this distance is less than the best distance, we update the best distance and best-k
 *       (best point).
 *       - We set the k-value for row,col to bestk.
 *       - We draw a pixel at row,col with the point bestk's color.
 *       - If we closed the Canvas, break.
 *       .
 *     .
 *   .
 *   Deallocate the arrays for x, y, and kvalue.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void voronoiFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),      // Set the screen sizes
              WINDOW_H = can.getWindowHeight(),
              POINTS = 100*4;                       // Set the number of control points
    srand(time(NULL));                              // Seed the random number generator
    int* x = new int[POINTS]();                     // Initialize an array for POINTS x coords
    int* y = new int[POINTS]();                     // Do the same for y coords
    int* kvalue = new int[WINDOW_W * WINDOW_H]();   // Create a mapping of control point values
    ColorFloat color[POINTS];                            // And for an array of colors
    ColorFloat tc, rc, lc, bc, xc, yc;                   // Color for the top, right, left, bottom, x-average, and y-average
    int bestk = 0;                                  // Keep track of the current best k-value
    float bdist, dist, xd, yd;                      // Keep track of the closes matches and current distances
    for (int i = 0; i < POINTS; i++) {              // Randomize the control points
        x[i] = rand() % WINDOW_W;
        y[i] = rand() % WINDOW_H;
    }
    srand(time(NULL));
    tc = Colors::randomColor(1.0f);                            // Randomize the axis colors
    rc = Colors::randomColor(1.0f);
    lc = Colors::randomColor(1.0f);
    bc = Colors::randomColor(1.0f);
    for (int i = 0; i < POINTS; i++) {              // For each control point...
        float xx = (float) x[i] / WINDOW_W;         // Calculate an value from 0:1 based on x coord
        float yy = (float) y[i] / WINDOW_H;         // Do the same for y
        xc = Colors::blendedColor(lc, rc, xx);              // Interpolate between the left and right colors
        yc = Colors::blendedColor(tc, bc, yy);              // Do the same for top and bottom
        color[i] = Colors::blendedColor(xc, yc, 0.5f);      // Complete the 4-way interpolation
    }
    #pragma omp parallel for private(bdist, xd, yd, dist, bestk)
    for (int i = 0; i < WINDOW_W; i++) {            // For each individual point...
        bestk = 0;
        for (int j = 0; j < WINDOW_H; j++) {
            bdist = 9999;                           // Reset the best distance
            for (int k = 0; k < POINTS; k++) {      // Find the closest control point
                xd = i - x[k];                      // Calculate the distance from each control point
                yd = j - y[k];
                dist = sqrt(xd * xd + yd * yd);
                if (dist < bdist) {                 // If it's the closest one
                    bdist = dist;                   // Update the best distance and control point
                    bestk = k;
                }
            }
            kvalue[i * WINDOW_H + j] = bestk;
            can.drawPoint(i, j, color[bestk]);      // Draw the point with the closest control's color
            if (!can.getIsOpen()) break;
        }
    }
    delete x;
    delete y;
    delete kvalue;
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c18(0, 0, 1600, 1200, BUFFER, "");
    c18.setBackgroundColor(WHITE);
    c18.start();
    voronoiFunction(c18);
    c18.close();
    glfwTerminate();  // Release GLFW
}

