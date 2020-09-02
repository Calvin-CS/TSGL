/*
 * testLineFan.cpp
 *
 * Usage: ./testLineFan <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>
#include "Util.h"

using namespace tsgl;

/*!
 * \brief Draws a fan of randomly colored lines at the target framerate and a dynamic number of threads using OMP.
 * \details
 * - The internal timer of the Canvas is set up to go off every \b FRAME seconds ( \b FRAME == 1 / \b FPS ).
 * - The spacing in between the arcs of the fan is stored in the constant: \b ARC .
 * - While the canvas is open:
 *   - The number of threads to use is recalculated, and the process is forked based off of the passed number of threads: \b t.
 *   - The internal timer sleeps on each thread until the next frame is ready to be drawn.
 *   - An offset is calculated based on the thread's ID and a predefined arc-length.
 *   - An angle is then calculated using the offset and the Canvas' current lifespan ( as calculated by \b can.getReps() ).
 *   - The vertices of the lines to be drawn are chosen using the sines and cosines of the predetermined angle.
 *   - A random color is chosen.
 *   - The line is draw to the Canvas.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param t The number of threads to use in the function.
 */
void lineFanFunction(Canvas& can, int t) {
    Background * bg = can.getBackground();
    const double ARC = 7.11;  //(Arbitrary) spacing between arcs of the fan
    while (can.isOpen()) {
        #pragma omp parallel num_threads(t)
        {
            can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
            int a, b, red, green, blue;
            double angle, offset = omp_get_thread_num() * ARC * 180 / PI;
            angle = offset + can.getReps();
            a = can.getWindowWidth() / 2 * (1 + sin(angle));
            b = can.getWindowHeight() / 2 * (1 + cos(angle));
            red = (a + can.getReps()) % NUM_COLORS;
            green = (b + can.getReps()) % NUM_COLORS;
            blue = (a * b + can.getReps()) % NUM_COLORS;
            bg->drawLine(0, 0, 0, can.getWindowHeight() * 0.9, angle, 0,0, ColorInt(red, green, blue));
        }
    }
}

//Takes command-line arguments for the width and height of the screen as well as for the
//number of threads to use in the function
int main(int argc, char** argv) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
      w = 1200;
      h = 900;                  //If not, set the width and height to a default value
    }
    unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
    if (t == 0)
      t = omp_get_num_procs();
    Canvas c(-1,-1,w,h,"Line Fan");
    c.run(lineFanFunction,t);
}
