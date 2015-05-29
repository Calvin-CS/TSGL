/*
 * Voronoi.cpp
 *
 *  Created on: May 29, 2015
 *      Author: Chris Dilley
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

class Voronoi {
protected:
static const int MY_POINTS = 100 * 4;
int * myX;
int * myY;
int * myKValue;
ColorFloat myColor[MY_POINTS];                            // And for an array of colors
ColorFloat myTC, myRC, myLC, myBC, myXC, myYC;                   // Color for the top, right, left, bottom, x-average, and y-average

public:
Voronoi(Canvas& can) {
	const int NEW_WINDOW_W = can.getWindowWidth(),      // Set the screen sizes
			  NEW_WINDOW_H = can.getWindowHeight();
	srand(time(NULL));
	myX = new int[MY_POINTS]();
	myY = new int[MY_POINTS]();
	myKValue = new int[NEW_WINDOW_W * NEW_WINDOW_H]();
	for (int i = 0; i < MY_POINTS; i++) {              // Randomize the control points
		myX[i] = rand() % NEW_WINDOW_W;
		myY[i] = rand() % NEW_WINDOW_H;
	}
	srand(time(NULL));
	myTC = Colors::randomColor(1.0f);                            // Randomize the axis colors
	myRC = Colors::randomColor(1.0f);
	myLC = Colors::randomColor(1.0f);
	myBC = Colors::randomColor(1.0f);
	for (int j = 0; j < MY_POINTS; j++) {              // For each control point...
		float xx = (float) myX[j] / NEW_WINDOW_W;         // Calculate an value from 0:1 based on x coord
		float yy = (float) myY[j] / NEW_WINDOW_H;         // Do the same for y
		myXC = Colors::blendedColor(myLC, myRC, xx);              // Interpolate between the left and right colors
		myYC = Colors::blendedColor(myTC, myBC, yy);              // Do the same for top and bottom
		myColor[j] = Colors::blendedColor(myXC, myYC, 0.5f);      // Complete the 4-way interpolation
	}

}

void draw(Canvas& can) {
	int myBestK = 0;                                  // Keep track of the current best k-value
	float myBDist, myDist, myXD, myYD;                      // Keep track of the closes matches and current distances
#pragma omp parallel for private(myBDist, myXD, myYD, myDist, myBestK)
	for (int i = 0; i < can.getWindowWidth(); i++) {            // For each individual point...
		myBestK = 0;
		for (int j = 0; j < can.getWindowHeight(); j++) {
			myBDist = 9999;                           // Reset the best distance
			for (int k = 0; k < MY_POINTS; k++) {      // Find the closest control point
				myXD = i - myX[k];                      // Calculate the distance from each control point
				myYD = j - myY[k];
				myDist = sqrt(myXD * myXD + myYD * myYD);
				if (myDist < myBDist) {                 // If it's the closest one
					myBDist = myDist;                   // Update the best distance and control point
					myBestK = k;
				}
			}
			myKValue[i * can.getWindowHeight() + j] = myBestK;
			can.drawPoint(i, j, myColor[myBestK]);      // Draw the point with the closest control's color
			if (!can.getIsOpen()) break;
		}
	}
}

virtual ~Voronoi() {
	delete [] myX;
	delete [] myY;
	delete [] myKValue;
	myX = myY = myKValue = NULL;
}

};

