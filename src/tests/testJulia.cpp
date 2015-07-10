/*
 * testJulia.cpp
 *
 *  Created on: May 27, 2015
 *      Author: Patrick Crain, Chris Dilley
 */

#include "Mandelbrot/Julia.h"

using namespace tsgl;

/*!
 * \brief Draws a Julia set.
 * \details It is drawn in this way:
 * - It is drawn in a similar way as the Mandelbrots are.
 * - A class contains all of the necessary information to draw the Julia set.
 * - When the Julia class object is made:
 *    - Set class instance variables for keeping track of the number of threads to use,
 *      the number of iterations, point coordinates for zooming in and out, and a redraw flag
 *      that determines when we should redraw the Julia object.
 *    .
 * - When you bind buttons to the CartesianCanvas:
 *    - Bind the spacebar to clear the screen and redraw the Julia object.
 *    - Bind the left mouse button to zoom in and redraw the Julia object at the
 *      focused point.
 *    - Bind the right mouse button to zoom out and redraw the Julia object.
 *    - Bind the mouse wheel to zoom in and out when scrolled up and down (respectively)
 *      and redraw the Julia object.
 *    .
 * - When you actually draw the Julia object onto the CartesianCanvas:
 *    - Store the window width of the Canvas.
 *    - Create a VisualQueue object that displays how much work each thread did in drawing
 *      the Julia object.
 *    - While its time to redraw:
 *        - Set the redraw flag to false.
 *        - Reset the Canvas' internal timer to 0.
 *        - Reset the VisualQueue object.
 *        - A parallel block is made using: \b myThreads threads.
 *            - Store the thread id number and the number of threads.
 *            - Store the thread color.
 *            - An infinite loop is made where the necessary calculations needed to draw the Julia
 *              object are made and the Julia object is drawn. Once the Julia object has been drawn, the
 *              loop is broken out of. Also, if the Canvas is not open or the redraw flag is set, the loop
 *              is also broken out of. Also, if one of the threads tries to render off of the screen, the loop
 *              is broken out of. The VisualQueue is also updated in this loop, and if you are a Mac user,
 *              I/O events are handled here as well.
 *              .
 *           .
 *       - Output the time it took for each iteration.
 *       - While the Canvas is open and the redraw flag is *NOT* set:
 *          - Sleep the internal timer until the next draw cycle.
 *          .
 *       .
 *    - Close the VisualQueue.
 *  .
 * \param can Reference to the CartesianCanvas to draw to.
 * \param threads Reference to the number of threads to use when drawing the Julia object.
 * \param depth The number of iterations to go to in order to draw the Julia object.
 */
void juliaFunction(CartesianCanvas& can, unsigned &threads, unsigned depth) {
    Julia j(threads,depth);  //Create the Julia object
    j.bindings(can);  //Bind the buttons
    j.draw(can);  //Draw it
}

//Takes command line arguments for the width and height of the screen
//as well as for the number of threads and for the number of iterations to go to
//in order to draw the Julia set.
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
      w = 1.2*Canvas::getDisplayHeight();
      h = 0.75*w;               //If not, set the width and height to a default value
    }
    int x = Canvas::getDisplayWidth()-w- 64;
    Cart c5(x, -1, w, h, -2, -1.125, 1, 1.125, "Julia Set", FRAME / 2);
    unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
    if (t == 0)
      t = omp_get_num_procs();
    unsigned d = (argc > 4) ? atoi(argv[4]) : 1000;
    c5.setBackgroundColor(GRAY);
    c5.start();

    juliaFunction(c5, t, d);   //And pass it as an argument

    c5.wait();
}
