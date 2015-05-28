/*
 * testDumbSort.cpp
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
 * \brief Provides a visualization for a basic (and slow) shaker sort.
 * \details
 * - The \b SIZE of the list of items and the number of iterations per frame ( \b IPF) are set.
 * - An integer array of size \b SIZE is allocated.
 * - A flag \b goingUp is set.
 * - Our integer array is filled with random integers under the Canvas' height.
 * - The background color is set to gray for visibility
 * - The internal timer of the Canvas is set up to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn
 *   - If the minimum sorted element equals the maximum, we're done
 *   - At a rate of \b IPF times a second:
 *     - If we're going up and the element above us is less than us, swap.
 *     - If we're going down and the element below us is less than us, swap.
 *     - Move in the current direction, inverting our direction if we've reached the minimum / maximum.
 *     .
 *   - Clear the Canvas
 *   - From 0 to \b SIZE:
 *     - Get the height of each element in the integer array
 *     - Draw it as a yellow rectangle if it's the currently-computed member; draw it red otherwise.
 *     .
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 * \param
 */
void dumbSortFunction(Canvas& can) {
	const int SIZE = 550,   // Size of the data pool (set to 550 by default)
	          IPF = 50;           // Iterations per frame
    int numbers[SIZE];      // Array to store the data
    int pos = 0, temp, min = 1, max = SIZE - 1, lastSwap = 0;
    bool goingUp = true;
    for (int i = 0; i < SIZE; i++)
        numbers[i] = rand() % (can.getWindowHeight() - 40);
    can.setBackgroundColor(GREY);
    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        if (min >= max) return;  // We are done sorting

        for (int i = 0; i < IPF; i++) {
            if (goingUp) {
                if (numbers[pos] > numbers[pos + 1]) {
                    temp = numbers[pos];
                    numbers[pos] = numbers[pos + 1];
                    numbers[pos + 1] = temp;
                    lastSwap = pos;
                }
                if (pos >= max) {
                    pos = max;
                    max = (lastSwap < max) ? lastSwap : max - 1;
                    goingUp = !goingUp;
                } else
                    pos++;
            } else {
                if (numbers[pos] < numbers[pos - 1]) {
                    temp = numbers[pos];
                    numbers[pos] = numbers[pos - 1];
                    numbers[pos - 1] = temp;
                    lastSwap = pos;
                }
                if (pos <= min) {
                    pos = min;
                    min = (lastSwap > min) ? lastSwap : min + 1;
                    goingUp = !goingUp;
                } else
                    pos--;
            }
        }

        can.clear();
        int start = 50, width = 1, height;
        ColorFloat color;
        for (int i = 0; i < SIZE; i++, start += width * 2) {
            height = numbers[i];
            if (i == pos)
                color = ColorInt(MAX_COLOR, MAX_COLOR, 0);
            else
                color = ColorInt(MAX_COLOR, 0, 0);
            can.drawRectangle(start, can.getWindowHeight() - 20 - height, width, height, color);
        }
    }
}

//Takes in command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int holder1 = atoi(argv[1]);   //Width
    int holder2 = atoi(argv[2]);   //Height
    int width, height = 0;
    if (holder1 <= 0 || holder2 <= 0) {   //Check the passed width and height if they are valid
    	width = WINDOW_W;   //If not, use the default width and height
    	height = WINDOW_H;
    } else if(holder1 > WINDOW_W || holder2 > WINDOW_H) {
     	width = WINDOW_W;
        height = WINDOW_H;
    } else {
    	width = holder1; //Else, use the passed width and height
    	height = holder2;
    }
    Canvas c9(0, 0, width, height, 1000, "", FRAME);
    c9.setBackgroundColor(GREY);
    c9.start();
    dumbSortFunction(c9);   //Pass it as an argument
    c9.close();
}
