/*
 * testMandelbrot.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include "Mandelbrot.cpp"

/*!
 * \brief Draws the Mandelbrot set on a CartesianCanvas with custom controls, a specified target update rate,
 *  and a dynamic number of threads and uses command-line arguments to specify the number of threads to use
 * \details
 * - The number of threads to use is predetermined and stored in: \b THREADS.
 * - The number of iterations to check is predetermined and stored in: \b DEPTH.
 * - The internal timer of the Canvas is set up to go off every ( \b FRAME / 2 ) seconds.
 * - A flag telling us to redraw is set to true
 * - The spacebar on-press event is set to tell the Canvas to clear and re-render.
 * - The left mouse on-press event is set to grab the mouse's current coordinates
 * - The left mouse on-release event is set to grab the mouse's current coordinates, and tell the Canvas to zoom into the
 * bounding rectangle between the current coordinates and those from the left mouse's on press event.
 * - The right mouse on-press event is set to grab the mouse's current coordinates, and tell the Canvas to zoom out
 * from that area.
 * - The mouse's scroll wheel is set to tell the Canvas to zoom in / out by a predetermined amount at the mouse's
 * current coordinates.
 * While the toRedraw flag is set:
 *   - Set the toRender flag to false
 *   - Reset the internal timer to 0.
 *   - Fork off the predetermined number of parallel threads using OMP
 *   - Store the actual number of threads spawned in: \b nthreads
 *   - Figure the cartesian size of the area each thread is to calculate and store it in: \b blocksize
 *   - Figure out the actual number of rows each thread is to calculate and store it in: \b blockheight
 *   - Run an outer loop from 0 to blockheight:
 *     - Calculate the cartesian coordinates of the thread's \b row as
 *     \b blocksize * (our ID) + (top of our CartesianCanvas) + (cartesian height of our physical pixels) * k
 *     - Run an inner loop from the minimum to maximum x values of the Cartesian Canvas, stepping by pixel width:
 *       - (Basic Mandelbrot calculations; see http://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings )
 *       - Break if the Canvas is to redraw
 *       .
 *     - Break if the Canvas is to redraw
 *     .
 *   Output the time it took to compute the screen
 *   Sleep the thread for one frame until the Canvas is closed by the user or told to redraw
 *   .
 * .
 * \param can, Reference to the CartesianCanvas being drawn to
 * \param numberOfThreads, Reference to the number of threads passed via command-line arguments.
 */
void mandelbrotFunction(CartesianCanvas& can, unsigned int & numberOfThreads) {
	Mandelbrot m1;
	m1.bindings(can);
	m1.draw(can, numberOfThreads);
}

//Takes command line arguments for the number of threads
int main(int argc, char* argv[]) {
    Cart c5(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, BUFFER, "", FRAME / 2);
    unsigned int numberOfThreads = atoi(argv[1]);    //Get the number of threads to use
    c5.setBackgroundColor(GREY);
    c5.start();
    mandelbrotFunction(c5, numberOfThreads);   //And pass it as an argument
    c5.close();
}
