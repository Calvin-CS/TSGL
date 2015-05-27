/*
 * testGraydient.cpp
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
 * \brief Draws a diagonal black-to-white gradient using OMP
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in: \b nthreads
 * - Check if the argument for the number of threads is valid:
 *   - If it is less than or equal to 0, use the number of threads that we can use with OMP.
 *   - If it is greater than the number of threads that we can use, use only the number of threads that we can use with OMP.
 *   - Else, its valid and use that many threads.
 * - The outer for loop is set up in a striping pattern, and the inner for loop runs from 0 to the Canvas height
 * - The color is set to a shade of gray based on its distance from the top left of the canvas
 * - The point is drawn to the Canvas
 * .
 * \param can, Reference to the Canvas being drawn to
 * \param numberOfThreads, Reference to the number of threads to use in the function
 */
void graydientFunction(Canvas& can, int & numberOfThreads) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int holder = omp_get_num_threads();   //Temp variable
        int nthreads = 0;   //What we'll actual be using
        if (numberOfThreads <= 0) {   //Check if the argument passed is a valid one, if not
        	nthreads = holder;   //Then assign the number of threads that we can currently get
        } else if(numberOfThreads > holder) {
        	nthreads = holder;   //If greater, use the number of threads that we can with OMP
        } else {
        	nthreads = numberOfThreads;  //Else, it is valid and you can use it
        }
        int color;
        for (int i = omp_get_thread_num(); i < can.getWindowWidth(); i += nthreads) {
            for (int j = 0; j < can.getWindowHeight(); j++) {
                color = i * MAX_COLOR / 2 / can.getWindowWidth() + j * MAX_COLOR / 2 / can.getWindowHeight();
                can.drawPoint(i, j, ColorInt(color, color, color));
            }
        }
    }
}

//http://www.cplusplus.com/articles/DEN36Up4/
int main(int argc, char* argv[]) {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    int holder1 = atoi(argv[1]);    //Width
    int holder2 = atoi(argv[2]);    //Height
    int width = 0;
    int height = 0;
    if(holder1 <= 0 || holder2 <= 0) {
       width = WINDOW_W;
       height = WINDOW_H;
    } else if(holder1 > WINDOW_W || holder2 > WINDOW_H) {
    	width = WINDOW_W;
    	height = WINDOW_H;
    } else {
    	width = holder1;
    	height = holder2;
    }
    Canvas c(0, 0, width, height, BUFFER, "");   //Create an explicit Canvas based off of the passed width and height (or the defaults if the width and height were invalid)
    int numberOfThreads = atoi(argv[3]);   //Convert the char pointer to an int, http://www.cplusplus.com/forum/beginner/58493/
    c.setBackgroundColor(GREY);
    c.start();
    graydientFunction(c, numberOfThreads);  //Now pass the argument for the number of threads to the test function
    c.close();
    glfwTerminate();  // Release GLFW
}
