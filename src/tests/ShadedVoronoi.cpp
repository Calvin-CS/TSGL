/*
 * ShadedVoronoi.cpp
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#include "Voronoi.cpp"

class ShadedVoronoi : public Voronoi {
private:
int * myKValue2;
static const int MY_NEW_POINTS = Voronoi::MY_POINTS / 4;
ColorFloat myColor[MY_NEW_POINTS];
int myBestK, myNextBestK;                                   // Keep track of the current best k-value
float myBDist, myNBDist, myDist, myXD, myYD;              // Keep track of the closes matches and current distances
public:

ShadedVoronoi(Canvas& can) : Voronoi(can) {
	const int NEW_WINDOW_W = can.getWindowWidth(),        // Set the screen sizes
			NEW_WINDOW_H = can.getWindowHeight();
	srand(time(NULL));                              // Seed the random number generator
	Voronoi::myX = new int[MY_NEW_POINTS]();                     // Initialize an array for POINTS x coords
	Voronoi::myY = new int[MY_NEW_POINTS]();                     // Do the same for y coords
	Voronoi::myKValue = new int[NEW_WINDOW_W * NEW_WINDOW_H]();   // Create a mapping of control point values
	myKValue2 = new int[NEW_WINDOW_W * NEW_WINDOW_H]();  // Create a mapping of more control point values
	myBestK = myNextBestK = -1;                 // Keep track of the current best k-value
	myBDist = myNBDist = myDist = myXD = myYD = 0;
	for (int i = 0; i < MY_NEW_POINTS; i++) {              // Randomize the control points
		Voronoi::myX[i] = rand() % NEW_WINDOW_W;
		Voronoi::myY[i] = rand() % NEW_WINDOW_H;
	}
	srand(time(NULL));
	Voronoi::myTC = Colors::randomColor(1.0f);                       // Randomize the axis colors
	Voronoi::myRC = Colors::randomColor(1.0f);
	Voronoi::myLC = Colors::randomColor(1.0f);
	Voronoi::myBC = Colors::randomColor(1.0f);
	for (int i = 0; i < MY_NEW_POINTS; i++) {              // For each control point...
		float xx = (float) Voronoi::myX[i] / NEW_WINDOW_W;         // Calculate an value from 0:1 based on x coord
		float yy = (float) Voronoi::myY[i] / NEW_WINDOW_H;         // Do the same for y
		Voronoi::myXC = Colors::blendedColor(Voronoi::myLC, Voronoi::myRC, xx);              // Interpolate between the left and right colors
		Voronoi::myYC = Colors::blendedColor(Voronoi::myTC, Voronoi::myBC, yy);              // Do the same for top and bottom
		myColor[i] = Colors::blendedColor(Voronoi::myXC, Voronoi::myYC, 0.5f);      // Complete the 4-way interpolation
	}
}

void draw(Canvas& can) {
	const int NEW_WINDOW_W = can.getWindowWidth(),        // Set the screen sizes
			  NEW_WINDOW_H = can.getWindowHeight(),
		      MY_NEW_POINTS = Voronoi::MY_POINTS / 4;		// Set the number of control points
	for (int i = 0; i < NEW_WINDOW_W; i++) {            // For each individual point...
		for (int j = 0; j < NEW_WINDOW_H; j++) {
			myBDist = myNBDist = 9999;                  // Reset the best distance
			for (int k = 0; k < MY_NEW_POINTS; k++) {      // Find the closest control point
				myXD = i - Voronoi::myX[k];                      // Calculate the distance from each control point
				myYD = j - Voronoi::myY[k];
				myDist = sqrt(myXD * myXD + myYD * myYD);
				if (myDist < myBDist) {                 // If it's the closest one
					myNBDist = myBDist;                 // Update the next best distance and control point
					myNextBestK = myBestK;
					myBDist = myDist;                   // Update the best distance and control point
					myBestK = k;
				} else if (myDist < myNBDist) {         // If it's the second closest one
					myNBDist = myDist;                  // Just update the next best distance / CP
					myNextBestK = k;
				}
			}
			Voronoi::myKValue[i * NEW_WINDOW_H + j] = myBestK;
			myKValue2[i * NEW_WINDOW_H + j] = myNextBestK;
			can.drawPoint(i, j, myColor[myBestK]);      // Draw the point with the closest control's color
			if (!can.getIsOpen()) break;
		}
	}
	for (int i = 0; i < NEW_WINDOW_W; i++) {            // For each individual point...
		for (int j = 0; j < NEW_WINDOW_H; j++) {
			int k = Voronoi::myKValue[i * NEW_WINDOW_H + j];       // Find its closest control point
			int nk = myKValue2[i * NEW_WINDOW_H + j];     // Then find its second closest
			float xd1 = i - Voronoi::myX[k];
			float yd1 = j - Voronoi::myY[k];
			float d1 = sqrt(xd1 * xd1 + yd1 * yd1); // Find the distance to it closest
			float xkd = Voronoi::myX[k] - Voronoi::myX[nk];
			float ykd = Voronoi::myY[k] - Voronoi::myY[nk];
			float kd = sqrt(xkd * xkd + ykd * ykd); // Find the distance between the CPs themselves
			float shading = d1 / kd;
			if (shading > 1)
				shading = 1;
			else if (shading < 0) shading = 0;
			can.drawPoint(i, j, ColorFloat(0.0f, 0.0f, 0.0f, shading));  // Draw the point with the closest control's color

			if (!can.getIsOpen()) break;
		}
	}

}

virtual ~ShadedVoronoi() {
	delete [] myKValue2;
	myKValue2 = NULL;
}
};

