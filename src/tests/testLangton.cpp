/*
 * testLangton.cpp
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
 * \brief Simulates Langton's Ant at speeds faster than the Canvas' framerate
 * \details
 * - The Canvas' width and height are stored
 * - Space is allocated for a flattened 2d array of booleans representing states of the various pixels on the Canvas
 * - The number of iterations per frame is set to a large number
 * - The initial position \b xx, \b yy is set to the center of the Canvas
 * - The initial direction is set to UP
 * - The internal timer of the Canvas is set up to expire once every \b FRAME / \b IPF seconds (see main() )
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn
 *   - If the ant's current cell is filled, turn right and color it; otherwise, turn left and blacken it
 *   - Invert the filled status of the square
 *   - Move forward one square from the current position, wrapping around the screen if on a boundary
 * - Deallocate space for the filled array
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void langtonFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the screen sizes
              WINDOW_H = can.getWindowHeight();
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    int xx = WINDOW_W / 2,  // Start at the center
        yy = WINDOW_H / 2;
    int direction = UP;
    while (can.getIsOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        if (filled[xx + WINDOW_W * yy]) {
            direction = (direction + 1) % 4;                      // Turn right
            can.drawPoint(xx, yy, ColorInt(MAX_COLOR, 0, 0));     // Color it
        } else {
            direction = (direction + 3) % 4;                      // Turn left
            can.drawPoint(xx, yy, ColorFloat(0.0f, 0.0f, 0.0f));  // Don't color it
        }
        filled[xx + WINDOW_W * yy] ^= true;  // Invert the square
        switch (direction) {                 // Check for wrap-around and move
            case UP:
                yy = yy > 0 ? yy - 1 : WINDOW_H - 1; break;
            case RIGHT:
                xx = xx < WINDOW_W - 1 ? xx + 1 : 0; break;
            case DOWN:
                yy = yy < WINDOW_H - 1 ? yy + 1 : 0; break;
            case LEFT:
                xx = xx > 0 ? xx - 1 : WINDOW_W - 1; break;
            default:
                std::cout << "BAD: dir == " << direction << std::endl; break;
        }
    }
    delete filled;
}

//Take command-line arguments for the width and height of the Canvas
int main(int argc, char* argv[]) {
    glfwInit();  // Initialize GLFW
    int holder1 = atoi(argv[1]);  //Width
    int holder2 = atoi(argv[1]);  //Length
    int width, height = 0;    //Actual width and height to be used
    if (holder1 <= 0 || holder2 <= 0) {  //Check if the passed arguments are valid
     	height = width = 960;  //If not, set the width and height to 960
    } else if(holder1 > WINDOW_W || holder2 > WINDOW_H) {
     	height = width = 960;
    } else {
     	width = holder1;  //Else, set the width and height to the passed command-line arguments
     	height = holder2;
    }
    Canvas c6(0, 0, width, height, BUFFER, "", FRAME / IPF);
    c6.setBackgroundColor(GREY);
    c6.start();
    langtonFunction(c6);
    c6.close();
    glfwTerminate();  // Release GLFW
}
