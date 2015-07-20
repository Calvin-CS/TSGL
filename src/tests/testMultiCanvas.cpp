/*
 * testMultiCanvas.cpp
 *
 * Usage: ./testMultiCanvas
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief More windows, more fun!
 * \details Displays multiple Canvas windows at the same time.
 * - A parallel block is created and the process is forked.
 * - The actual number of threads spawned is stored.
 * - The color for each point on the Canvases is stored.
 * - The outer for loop is set up in a striping pattern, the inner for loop is set from 0 to the current Canvas' height.
 *   - Get the color for each point.
 *   - Draw the point on the current being worked on Canvas.
 *   .
 * .
 * \param can Reference to the Canvas to draw to.
 * \param t The number of threads to use in the process.
 * \see testGraydient ( as this is a copy of testGraydient but on multiple Canvases ).
 */
static void multiCanvasFunction(Canvas* can, int t) {
    #pragma omp parallel num_threads(t)
    {
        int nthreads = omp_get_num_threads();   //Temp variable
        int color;
        for (int i = omp_get_thread_num(); i < can->getWindowWidth(); i += nthreads) {
            for (int j = 0; j < can->getWindowHeight(); j++) {
                color = i * MAX_COLOR / 2 / can->getWindowWidth() + j * MAX_COLOR / 2 / can->getWindowHeight();
                can->drawPoint(i, j, ColorInt(color, color, color));
            }
        }
    }
}

int main(int argc, char* argv[]) {
    const int NCAN = 4;
    int mw = Canvas::getDisplayWidth(), mh = Canvas::getDisplayHeight(), w = 0.5*mw, h = 0.5*mh;
    Canvas* c[NCAN] {
        new Canvas(0,   0,   w,h,"Canvas 1"),
        new Canvas(mw-w,0,   w,h,"Canvas 2"),
        new Canvas(0,   mh-h,w,h,"Canvas 3"),
        new Canvas(mw-w,mh-h,w,h,"Canvas 4")
    };
    std::thread t[NCAN];
    for (unsigned i = 0; i < NCAN; ++i) {
      c[i]->start();
      t[i] = std::thread(multiCanvasFunction,std::ref(c[i]), omp_get_num_procs()/NCAN);
    }
    for (unsigned i = 0; i < NCAN; ++i)
      t[i].join();
    for (unsigned i = 0; i < NCAN; ++i)
      c[i]->wait();
}
