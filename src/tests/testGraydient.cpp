/*
 * testGraydient.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

/*!
 * \brief Draws a diagonal black-to-white gradient using OMP and takes in a command line argument for the
 * number of threads to use
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in: \b nthreads
 * - Check if the argument for the number of threads is valid:
 *   - If it is less than or equal to 0, use the number of threads that we can use with OMP.
 *   - If it is greater than the number of threads that we can use, use only the number of threads that we can use with OMP.
 *   - Else, its valid and use that many threads.
 * - The outer for loop is set up in a striping pattern, and the inner for loop runs from 0 to the Canvas height
 * - The color is set to a shade of gray based on its distance from the top left of the canvas
 * - The point is drawn to the Canvas
 * .
 * \param can, Reference to the Canvas being drawn to
 * \param numberOfThreads, Reference to the number of threads to use in the function
 */
void graydientFunction(Canvas& can, int & numberOfThreads) {
    #pragma omp parallel num_threads(numberOfThreads)
    {
        int nthreads = omp_get_num_threads();   //Temp variable
        int color;
        for (int i = omp_get_thread_num(); i < can.getWindowWidth(); i += nthreads) {
            for (int j = 0; j < can.getWindowHeight(); j++) {
                color = i * MAX_COLOR / 2 / can.getWindowWidth() + j * MAX_COLOR / 2 / can.getWindowHeight();
                can.drawPoint(i, j, ColorInt(color, color, color));
            }
        }
    }
}

//Takes in the window width and height as command line arguments for the Canvas
//as well as for the number of threads to use
//( see http://www.cplusplus.com/articles/DEN36Up4/ )
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 960;
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c(0, 0, w, h, "");   //Create an explicit Canvas based off of the passed width and height (or the defaults if the width and height were invalid)
    int numberOfThreads = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Convert the char pointer to an int ( see http://www.cplusplus.com/forum/beginner/58493/ )
    c.start();
    graydientFunction(c, numberOfThreads);  //Now pass the argument for the number of threads to the test function
    c.wait();
}
