/*
 * testShadedVoronoi.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include "ShadedVoronoi.cpp"

using namespace tsgl;

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
}

//Takes in command line arguments for the width and height of the window
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 960;
  int h = (argc > 2) ? atoi(argv[2]) : w;
	Canvas c19(0, 0, w, h, "");
	c19.setBackgroundColor(WHITE);
	c19.start();
	shadedVoronoiFunction(c19);
	c19.wait();
}

