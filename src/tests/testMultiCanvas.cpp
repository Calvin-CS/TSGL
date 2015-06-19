/*
 * testGraydient.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

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
        new Canvas(0,   0,   w,h,""),
        new Canvas(mw-w,0,   w,h,""),
        new Canvas(0,   mh-h,w,h,""),
        new Canvas(mw-w,mh-h,w,h,"")
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
