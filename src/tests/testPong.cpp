/*
 * testPong.cpp
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
 * \brief Plays a nice game of Pong!
 * \details
 * - Store the Canvas' dimensions for ease of use, and declare some useful variables.
 * - Seed the random number generator.
 * - Set the intial position of the paddles and the ball.
 * - Randomize the initial direction of the ball until it's at a reasonable speed.
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
 * - Bind the up arrow's on-press and on-release events to set the right
 * paddle's direction to -1 (up) and 0 (static) respectively.
 * - Bind the down arrow's on-press and on-release events to set the right
 * paddle's direction to 1 (down) and 0 respectively.
 * - Bind the W key's on-press and on-release events to set the left
 * paddle's direction to -1 and 0 respectively.
 * - Bind the S key's on-press and on-release events to set the left
 * paddle's direction to 1 and 0 respectively.
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - Move the ball in its current direction at its current speed.
 *   - If the ball passed by one of the goal areas, increment the appropriate player's score,
 *   reset the ball's position, and send it off in a random direction and speed.
 *   - If the ball hits the top or bottom of the screen, bounce it.
 *   - If the ball hits a paddle, reverse its x-speed and randomly adjust its y-speed.
 *   - Move the paddles according to their current directions.
 *   - Draw the balls and paddles.
 *   - Draw the scores at the top of the screen.
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void pongFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight();
    float leftY, rightY, ballX, ballY;
    float xx, yy, speed, dir;
    int leftdir = 0, rightdir = 0, leftPoints = 0, rightPoints = 0;
    srand(time(NULL));
    // Set initial positions
    leftY = rightY = WINDOW_H / 2 - 32;
    speed = 8;
    ballX = WINDOW_W / 2 - 8;
    ballY = WINDOW_H / 2 - 8;
    // Start the ball off in a random direction
    do {
        dir = randfloat(1000) * 2 * PI;
        xx = speed * cos(dir);
        yy = speed * sin(dir);
    } while (xx > -4 && xx < 4);
    // Set up button bindings
    can.bindToButton(TSGL_UP, TSGL_PRESS, [&rightdir]() {rightdir = -1;});
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&rightdir]() {rightdir = 1;});
    can.bindToButton(TSGL_UP, TSGL_RELEASE, [&rightdir]() {if (rightdir == -1) rightdir = 0;});
    can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [&rightdir]() {if (rightdir == 1) rightdir = 0;});
    can.bindToButton(TSGL_W, TSGL_PRESS, [&leftdir] () {leftdir = -1;});
    can.bindToButton(TSGL_S, TSGL_PRESS, [&leftdir] () {leftdir = 1;});
    can.bindToButton(TSGL_W, TSGL_RELEASE, [&leftdir] () {if (leftdir == -1) leftdir = 0;});
    can.bindToButton(TSGL_S, TSGL_RELEASE, [&leftdir] () {if (leftdir == 1) leftdir = 0;});
    // Check to see if the window has been closed
    while (can.getIsOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        // Move the ball
        ballX += xx;
        ballY += yy;
        // Handle ball boundary collisions
        if (ballX > WINDOW_W + 8) {
            leftPoints++;
            ballX = WINDOW_W / 2 - 8;
            ballY = WINDOW_H / 2 - 8;
            do {
                dir = randfloat(1000) * 2 * 3.14159f;
                xx = speed * cos(dir);
                yy = speed * sin(dir);
            } while (xx > -4 && xx < 4);
        } else if (ballX < -8) {
            rightPoints++;
            ballX = WINDOW_W / 2 - 8;
            ballY = WINDOW_H / 2 - 8;
            do {
                dir = randfloat(1000) * 2 * 3.14159f;
                xx = speed * cos(dir);
                yy = speed * sin(dir);
            } while (xx > -4 && xx < 4);
        } else if (ballY > WINDOW_H - 8 || ballY < 8) yy = -yy;
        // Handle ball paddle collisions
        if (ballX < 32 && ballX > 0 && ballY > leftY - 8 && ballY < leftY + 72) {
            xx = -xx;
            yy += randfloat(1000) * 2 - 1;
        } else if (ballX > WINDOW_W - 32 && ballX < WINDOW_W && ballY > rightY - 8 && ballY < rightY + 72) {
            xx = -xx;
            yy += randfloat(1000) * 2 - 1;
        }
        // Move the paddles if necessary
        rightY += 4 * rightdir;
        leftY += 4 * leftdir;
        can.clear();
        // Draw paddles and balls
        can.drawRectangle(8, leftY, 24, 64, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
        can.drawRectangle(WINDOW_W - 24 - 8, rightY, 24, 64, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
        can.drawRectangle(ballX - 8, ballY - 8, 16, 16, WHITE);
        // Draw Scores
        for (int i = 0; i < leftPoints; i++) {
            can.drawRectangle(WINDOW_W / 2 - 64 - 4 * i, 16, 2, 8, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
        }
        for (int i = 0; i < rightPoints; i++) {
            can.drawRectangle(WINDOW_W / 2 + 64 + 4 * i, 16, 2, 8, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
        }
    }
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas c25(0,0,1600,600,1000, "", FRAME);
    c25.setBackgroundColor(BLACK);
    c25.start();
    pongFunction(c25);
    c25.close();
    glfwTerminate();  // Release GLFW
}
