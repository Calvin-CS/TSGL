/*
 * testShadedVoronoi.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include "ShadedVoronoi.cpp"

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
	ShadedVoronoi s1(can);
	s1.draw(can);
	s1.~ShadedVoronoi();
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

