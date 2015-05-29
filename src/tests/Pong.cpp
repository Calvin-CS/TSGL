/*
 * Pong.cpp
 *
 *  Created on: May 29, 2015
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

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

class Pong {
private:
float myLeftY, myRightY, myBallX, myBallY;
float myXX, myYY, mySpeed, myDir;
int myLeftDir, myRightDir, myLeftPoints, myRightPoints;

public:
Pong(Canvas& can) {
const int NEW_WINDOW_W = can.getWindowWidth(), NEW_WINDOW_H = can.getWindowHeight();
myLeftDir = myRightDir = myLeftPoints = myRightPoints = 0;
srand(time(NULL));
myLeftY = myRightY = NEW_WINDOW_H / 2 - 32;
mySpeed = 8;
myBallX = NEW_WINDOW_W / 2 - 8;
myBallY = NEW_WINDOW_H / 2 - 8;
do {
	myDir = randfloat(1000) * 2 * PI;
	myXX = mySpeed * cos(myDir);
	myYY = mySpeed * sin(myDir);
} while(myXX > -4 && myXX < 4);
}

void bindings(Canvas& can) {
    can.bindToButton(TSGL_UP, TSGL_PRESS, [this]() {this->myRightDir = -1;});
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [this]() {this->myRightDir = 1;});
    can.bindToButton(TSGL_UP, TSGL_RELEASE, [this]() {if (this->myRightDir == -1) this->myRightDir = 0;});
    can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [this]() {if (this->myRightDir == 1) this->myRightDir = 0;});
    can.bindToButton(TSGL_W, TSGL_PRESS, [this] () {this->myLeftDir = -1;});
    can.bindToButton(TSGL_S, TSGL_PRESS, [this] () {this->myLeftDir = 1;});
    can.bindToButton(TSGL_W, TSGL_RELEASE, [this] () {if (this->myLeftDir == -1) this->myLeftDir = 0;});
    can.bindToButton(TSGL_S, TSGL_RELEASE, [this] () {if (this->myLeftDir == 1) this->myLeftDir = 0;});
}

void draw(Canvas& can) {
    // Check to see if the window has been closed
    while (can.getIsOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        // Move the ball
        myBallX += myXX;
        myBallY += myYY;
        // Handle ball boundary collisions
        if (myBallX > can.getWindowWidth() + 8) {
            myLeftPoints++;
            myBallX = can.getWindowWidth() / 2 - 8;
            myBallY = can.getWindowHeight() / 2 - 8;
            do {
                myDir = randfloat(1000) * 2 * 3.14159f;
                myXX = mySpeed * cos(myDir);
                myYY = mySpeed * sin(myDir);
            } while (myXX > -4 && myXX < 4);
        } else if (myBallX < -8) {
            myRightPoints++;
            myBallX = can.getWindowWidth() / 2 - 8;
            myBallY = can.getWindowHeight() / 2 - 8;
            do {
                myDir = randfloat(1000) * 2 * 3.14159f;
                myXX = mySpeed * cos(myDir);
                myYY = mySpeed * sin(myDir);
            } while (myXX > -4 && myXX < 4);
        } else if (myBallY > can.getWindowHeight() - 8 || myBallY < 8) myYY = -myYY;
        // Handle ball paddle collisions
        if (myBallX < 32 && myBallX > 0 && myBallY > myLeftY - 8 && myBallY < myLeftY + 72) {
            myXX = -myXX;
            myYY += randfloat(1000) * 2 - 1;
        } else if (myBallX > can.getWindowWidth() - 32 && myBallX < can.getWindowWidth() && myBallY > myRightY - 8 &&  myBallY < myRightY + 72) {
            myXX = -myXX;
            myYY += randfloat(1000) * 2 - 1;
        }
        // Move the paddles if necessary
        myRightY += 4 * myRightDir;
        myLeftY += 4 * myLeftDir;
        can.clear();
        // Draw paddles and balls
        can.drawRectangle(8, myLeftY, 24, 64, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
        can.drawRectangle(can.getWindowWidth() - 24 - 8, myRightY, 24, 64, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
        can.drawRectangle(myBallX - 8, myBallY - 8, 16, 16, WHITE);
        // Draw Scores
        for (int i = 0; i < myLeftPoints; i++) {
            can.drawRectangle(can.getWindowWidth() / 2 - 64 - 4 * i, 16, 2, 8, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
        }
        for (int i = 0; i < myRightPoints; i++) {
            can.drawRectangle(can.getWindowWidth() / 2 + 64 + 4 * i, 16, 2, 8, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
        }
    }
}

};


