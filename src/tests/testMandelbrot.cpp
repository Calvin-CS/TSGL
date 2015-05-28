/*
 * testMandelbrot.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

// Some constants that get used a lot
const int MAX_COLOR = 255;

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

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
    const unsigned int THREADS = 8;  //omp_get_num_procs();
    const unsigned int DEPTH = MAX_COLOR;
    Decimal firstX, firstY, secondX, secondY;
    bool toRedraw = true;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &toRedraw]() {
        can.clear();
        toRedraw = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, &firstX, &firstY]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), firstX, firstY);
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, &firstX, &firstY, &secondX, &secondY, &toRedraw]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), secondX, secondY);
        can.zoom(firstX, firstY, secondX, secondY);
        toRedraw = true;
    });
    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, &toRedraw]() {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        can.zoom(x, y, 1.5);
        toRedraw = true;
    });
    can.bindToScroll([&can, &toRedraw](double dx, double dy) {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        Decimal scale;
        if (dy == 1) scale = .5;
        else scale = 1.5;
        can.zoom(x, y, scale);
        toRedraw = true;
    });

    while (toRedraw) {
        toRedraw = false;
        can.reset(); //Removed the timer and replaced it with an internal timer in the Canvas class
        #pragma omp parallel num_threads(THREADS)
        {
            unsigned int holder = omp_get_num_threads();  //Temp variable
            unsigned int nthreads = 1;   //Actual number of threads
            if (numberOfThreads > holder || numberOfThreads <= 0) {  //Check if the passed number of threads is valid
            	nthreads = holder;  //If not, use the number of threads that we can use with OMP
            } else {
            	nthreads = numberOfThreads;  //Else, use that many threads
            }
            double blocksize = can.getCartHeight() / nthreads;
            double blockheight = can.getWindowHeight() / nthreads;
            for (unsigned int k = 0; k <= blockheight && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
                long double row = blocksize * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
                for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                    complex originalComplex(col, row);
                    complex c(col, row);
                    unsigned iterations = 0;
                    while (std::abs(c) < 2.0 && iterations != DEPTH) {  // Compute it until it escapes or we give up
                        iterations++;
                        c = c * c + originalComplex;
                    }
                    if (iterations == DEPTH) { // If the point never escaped, draw it black
                        can.drawPoint(col, row, BLACK);
                    } else { // Otherwise, draw it with color based on how long it took
                        can.drawPoint(col, row,
                                      ColorInt(iterations % 151,
                                             ((iterations % 131) + omp_get_thread_num() * 128 / nthreads) % 255,
                                             iterations % 255));
                    }
                    if (toRedraw) break;
                }
                if (toRedraw) break;
            }

        }
        std::cout << can.getTime() << std::endl;
        while (can.getIsOpen() && !toRedraw)
            can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
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
