/*
 * testColorWheel.cpp
 *
 * Usage: ./testColorWheel <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>
#include "Util.h"  //Constants

using namespace tsgl;

/*!
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and private per-thread variables.
 * \details
 * - \b THREADS is set to a number greater than the number of physical processors.
 * - The center of the canvas is computed and stored.
 * - The radius of the wheel is computed (using the minimum of the Canvas width / height) and stored.
 * - The size of the \b GRADIENT is computed and stored.
 * - Variables for the second and third vertices of a triangle and the current shading are initialized.
 * - The internal timer of the Canvas is set up to expire every \b FRAME seconds.
 * - The predetermined number of parallel threads is forked off using OMP, pulling along the coordinate
 *   and shading variables.
 * - The actual number of threads is stored in: \b nthreads.
 * - A color \b delta is computed.
 * - Each thread's thread id ( \b tid ) is stored.
 * - Each thread's shading is computed using it's id and \b nthreads.
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn.
 *   - Each thread's \b start is computed using the timer's lifetime and it's current id.
 *   - The second and third coordinates of a triangle approximating an arc of a circle are
 *     computed using the \b GRADIENT and the thread's \b start position.
 *   - A triangle is drawn on the Canvas for each thread, with the first vertex in the center, and
 *     the second and third vertices as computed above, with a hue based on the precomputed \b start,
 *     full saturation, and a value of \b shading.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param threads Number of threads to use.
 */
void colorWheelFunction(Canvas& can, int threads) {
    Background * bg = can.getBackground();
    const int CW = can.getWindowWidth() / 2,          // Half the window's width
              CH = can.getWindowHeight() / 2;         // Half the window's height
    const float RADIUS = (CH < CW ? CH : CW) * .95,   // Radius of wheel
                GRADIENT = 2 * PI / NUM_COLORS;       // Gap between wedges
    #pragma omp parallel num_threads(threads)
    {
        float x2, x3, y2, y3, shading;
        int tid = omp_get_thread_num();
        int delta = tid * NUM_COLORS / threads;           // Distance between threads to compute
        shading = 1 - (float) tid / threads;
        while (can.isOpen()) {
            can.sleep();
            int start = (NUM_COLORS - (can.getReps() % NUM_COLORS) + delta) % NUM_COLORS;
            x2 = RADIUS * sin(GRADIENT * start);
            y2 = RADIUS * cos(GRADIENT * start);
            x3 = RADIUS * sin(GRADIENT * (start + 1));
            y3 = RADIUS * cos(GRADIENT * (start + 1));
            bg->drawTriangle(0,0,0, x2,y2,0, x3,y3,0, 0,0,0,
                             ColorHSV(start * 6.0f / NUM_COLORS, 1.0f, shading), false);
        }
    }
}


//Takes command line arguments for the height and width of the window
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
        w = h = 960;          //If not, set the width and height to a default value
    int t = (argc > 3) ? atoi(argv[3]) : 64;
    Canvas c(-1, -1, w, h, "Color Wheel");
    c.run(colorWheelFunction,t);
}
