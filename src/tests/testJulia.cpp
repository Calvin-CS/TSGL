/*
 * testMandelbrot.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include "Mandelbrot/Julia.h"

using namespace tsgl;

void juliaFunction(CartesianCanvas& can, unsigned &threads, unsigned depth) {
    Julia j(threads,depth);
    j.bindings(can);
    j.draw(can);
}

//Takes command line arguments for the number of threads
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
      w = 1.2*Canvas::getDisplayHeight();
      h = 0.75*w;               //If not, set the width and height to a default value
    }
    int x = Canvas::getDisplayWidth()-w- 64;
    Cart c5(x, -1, w, h, -2, -1.125, 1, 1.125, "", FRAME / 2);
    unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
    if (t == 0)
      t = omp_get_num_procs();
    unsigned d = (argc > 4) ? atoi(argv[4]) : 1000;
    c5.setBackgroundColor(GREY);
    c5.start();

    juliaFunction(c5, t, d);   //And pass it as an argument

    c5.wait();
}
