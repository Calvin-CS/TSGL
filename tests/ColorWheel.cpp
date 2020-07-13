/*
 * ColorWheel.cpp
 *
 * Usage: ./ColorWheel <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>
#include <vector>
#include "Util.h"  //Constants

using namespace tsgl;

/*!
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and private per-thread variables.
 * \details
 * - \b threads is set to a number greater than the number of physical processors.
 * - The center of the canvas is computed and stored.
 * - The radius of the wheel is computed (using the minimum of the Canvas width / height) and stored.
 * - The size of the \b GRADIENT is computed and stored.
 * - Variables for the second and third vertices of a triangle are initialized.
 * - Each of the Triangles in the wheel are created and added to the Canvas.
 * - The predetermined number of parallel threads is forked off using OMP.
 * - A color \b delta is computed.
 * - Each thread's thread id ( \b tid ) is stored.
 * - Each thread's shading is computed using it's id and \b nthreads.
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn.
 *   - Each thread's \b start is computed using the counter's lifetime and its current id.
 *   - A triangle's color is changed to a hue based on the precomputed \b start,
 *     full saturation, and a value of \b shading.
 *   .
 * - After the Canvas is closed, all the Triangle pointers are deleted.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void colorWheelFunction(Canvas& can, int threads) {
    const int CW = can.getWindowWidth() / 2,          // Half the window's width
              CH = can.getWindowHeight() / 2;         // Half the window's height
    const float RADIUS = (CH < CW ? CH : CW) * .95,   // Radius of wheel
                GRADIENT = 2 * PI / NUM_COLORS;       // Gap between wedges

    //Create all Triangles in wheel and add to Canvas
    std::vector<Triangle*> segs(NUM_COLORS);
    float x2, x3, y2, y3;
    for(int i = 0; i < NUM_COLORS; i++) {
        int s = (NUM_COLORS - i) % NUM_COLORS;
        x2 = CW + RADIUS * sin(GRADIENT * s);
        y2 = CH + RADIUS * cos(GRADIENT * s);
        x3 = CW + RADIUS * sin(GRADIENT * (s + 1));
        y3 = CH + RADIUS * cos(GRADIENT * (s + 1));
        Triangle* t = new Triangle(CW, CH, x2, y2, x3, y3);
        t->setHasOutline(false);
        segs[i] = t;
        can.add(t);
    }

    #pragma omp parallel num_threads(threads)
    {
        int tid = omp_get_thread_num();
        int delta = tid * NUM_COLORS / threads;           // Distance between threads to compute
        float shading = 1 - (float) tid / threads;
        int counter = 0;
        while (can.isOpen()) {
            can.sleep();
            #pragma omp barrier               //Barrier for synchronization
            int start = (NUM_COLORS - (counter % NUM_COLORS) + delta) % NUM_COLORS;
            //Change the color of the next segment
            segs[start]->setColor(ColorHSV(start * 6.0f / NUM_COLORS, 1.0f, shading));
            counter++;
        }
    }

    //Free memory from triangle pointers
    for(int i = 0; i < NUM_COLORS; i++) {
      delete segs[i];
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
