/*
 * testVoronoi.cpp
 *
 * Usage: ./testVoronoi <width> <height>
 */

/* testVoronoi.cpp contains multiple functions that display a Voronoi diagram in similar fashions. */

#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE //To avoid issues with fopen, http://stackoverflow.com/questions/14386/fopen-deprecated-warning
#endif

#include "Voronoi/Voronoi.h"
#include "Voronoi/ShadedVoronoi.h"

using namespace tsgl;

/*!
 * \brief Draws a randomly generated Voronoi diagram, using OMP and private variables
 * ( see http://en.wikipedia.org/wiki/Voronoi_diagram ).
 * \details
 * - The data and methods for drawing are stored in a class.
 * - When you create an instance of the class:
 *    - The Canvas's dimensions are stored in local constants.
 *    - The number of control points are predetermined and stored a protected class instance variable.
 *    - We seed the random number generator with the time.
 *    - We allocate arrays for x and y coordinates for each of the points.
 *    - We allocate an array of k-values for each pixel on the Canvas.
 *    - For each control point:
 *      - Fill the x-coordinate and y-coordinate arrays with random values (thereby randomizing the locations
 *        of the control points).
 *      .
 *    - Seed the random number generator again with the time.
 *    - We initialize variables for the top, right, left, and bottom corner colors.
 *    - For each control point:
 *      - We get its x coordinate and y coordinate.
 *      - We determine its x-color based on a linear interpolation on the x-axis.
 *      - We determine its y-color based on a linear interpolation on the y-axis.
 *      - We determine its color based on an even mixture of its x-color and y-color.
 *      .
 *   .
 * - When you draw:
 *    - The best k-value is initially set to 0.
 *    - We initialize variables keeping track of the best and current distances.
 *    - We start a parallel OMP block, allowing the system to determine the best means of parallelization.
 *    - For each column, we set the best k to 0. Then:
 *      - For each row:
 *        - Reset the best distance to a large value.
 *        - For each control point:
 *          - We calculate the distance from row,col to the control point.
 *          - If this distance is less than the best distance, we update the best distance and best-k
 *            (best point).
 *          - We set the k-value for row,col to the best-k.
 *          - We draw a pixel at row,col with the best-k's point color.
 *          - If we closed the Canvas, break.
 *          .
 *        .
 *      .
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void voronoiFunction(Canvas& can) {
  Voronoi v1(can);   //Make the Voronoi object
  v1.draw(can);      //Draw it on the Canvas
}

/*!
 * \brief Draws a randomly generated Voronoi diagram with fancy shading.
 * \details Same principle as voronoiFunction(). Also has a class.
 * - Key differences:
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
 * \param can Reference to the Canvas being drawn to.
 */
void shadedVoronoiFunction(Canvas& can) {
  ShadedVoronoi s1(can);
  s1.draw(can);
}

//Takes command line arguments for the width and height of the window
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;            //If not, set the width and height to a default value
  //Normal Voronoi
  std::cout << "Regular Voronoi" << std::endl;
  Canvas c1(-1, -1, w, h, "Voronoi");
  c1.run(voronoiFunction);
  std::cout << "Special Voronoi" << std::endl;

  //Shaded Voronoi
  Canvas c2(-1, -1, w, h, "Shaded Voronoi");
  c2.run(shadedVoronoiFunction);
}
