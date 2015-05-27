/*
 * testLangtonRainbow.cpp
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
 * \brief Simulates 4 Langton's Ants at speeds faster than the Canvas' framerate, with nicer colors!
 * \details Same as langtonColonyFunction(), but with dynamically-colored ants.
 * \param can Reference to the Canvas being drawn to
 */
void langtonRainbowFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),                          // Set the window sizes
              WINDOW_H = can.getWindowHeight(),
              RADIUS = WINDOW_H / 6;                                    // How far apart to space the ants
    bool* filled = new bool[WINDOW_W * WINDOW_H]();                     // Create an empty bitmap for the window
    int xx[4], yy[4], dir[4];
    xx[0] = WINDOW_W / 2 - RADIUS;
    yy[0] = WINDOW_H / 2;
    xx[1] = WINDOW_W / 2;
    yy[1] = WINDOW_H / 2 - RADIUS;
    xx[2] = WINDOW_W / 2 + RADIUS;
    yy[2] = WINDOW_H / 2;
    xx[3] = WINDOW_W / 2;
    yy[3] = WINDOW_H / 2 + RADIUS;

    for (int i = 0; i < 4; i++) {
        dir[i] = i;
    }

    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int j = 0; j < 4; j++) {
            if (filled[xx[j] + WINDOW_W * yy[j]]) {
                dir[j] = (dir[j] + 1) % 4;
                can.drawPoint(xx[j], yy[j],
                              ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 1.0f, .25f));
            } else {
                dir[j] = (dir[j] + 3) % 4;
                can.drawPoint(xx[j], yy[j],
                              ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 0.5f, .25f));
            }
            switch (dir[j]) {
                case UP:
                    yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H - 1;
                    break;
                case RIGHT:
                    xx[j] = (xx[j] < WINDOW_H - 1) ? xx[j] + 1 : 0;
                    break;
                case DOWN:
                    yy[j] = (yy[j] < WINDOW_H - 1) ? yy[j] + 1 : 0;
                    break;
                case LEFT:
                    xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_H - 1;
                    break;
                default:
                    break;
            }
        }
        for (int j = 0; j < 4; j++)
            filled[xx[j] + WINDOW_W * yy[j]] ^= true;        //Invert the squares the ants are on
    }
    delete filled;
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_FRONT_AND_BACK);	// For Patrick's laptop
    Canvas c8(0, 0, WINDOW_H, WINDOW_H, BUFFER, "", FRAME / IPF);
    c8.setBackgroundColor(BLACK);
    c8.start();
    langtonRainbowFunction(c8);
    c8.close();
    glfwTerminate();  // Release GLFW
}

