/*
 * testVoronoi.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include "Voronoi.cpp"

using namespace tsgl;

/*!
 * \brief Draws a randomly generated Voronoi diagram, using OMP and private variables
 * ( see http://en.wikipedia.org/wiki/Voronoi_diagram ).
 * \details
 * - The Canvas's dimensions are stored in a local variable.
 * - The number of control points are predetermined and stored in: \b POINTS.
 * - We seed the random number generator with the time.
 * - We allocate arrays for x and y coordinates for each of the points.
 * - We allocate an array of k-values for each pixel on the Canvas
 * - We declare an array of colors for each of the points
 * - We initialize variables for the top, right, left, bottom, x-center, and y-center colors
 * - The best k-value is initially set to 0.
 * - We initialize variables keeping track of the best and current distances.
 * - We randomize the locations of the control points.
 * - We randomize the colors of key areas of the diagram.
 * - For each control point:
 *   - We get its x coordinate and y coordinate
 *   - We determine its x-color based on a linear interpolation on the x-axis.
 *   - We determine its y-color based on a linear interpolation on the y-axis.
 *   - We determine its color based on an even mixture of its x-color and y-color.
 *   .
 * - We start a parallel OMP block, allowing the system to determine the best means of parallelization
 * - For each column, we set the best k to 0. Then:
 *   - For each row:
 *     - For each point:
 *       - We calculate the distance from row,col to the control point
 *       - If this distance is less than the best distance, we update the best distance and best-k
 *       (best point).
 *       - We set the k-value for row,col to bestk.
 *       - We draw a pixel at row,col with the point bestk's color.
 *       - If we closed the Canvas, break.
 *       .
 *     .
 *   .
 *   Deallocate the arrays for x, y, and kvalue.
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void voronoiFunction(Canvas& can) {
	Voronoi v1(can);   //Make the Voronoi object
	v1.draw(can);      //Draw it on the Canvas
}

//Takes command line arguments for the width and height of the window
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;              //If not, set the width and height to a default value
	Canvas c18(-1, -1, w, h, "");
	c18.setBackgroundColor(WHITE);
	c18.start();
	voronoiFunction(c18);
	c18.wait();
}

