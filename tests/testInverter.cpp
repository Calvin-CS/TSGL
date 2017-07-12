/*
 * testInverter.cpp
 *
 * Usage: ./testInverter <numThreads>
 */

#include "ImageInverter/ImageInverter.h"

int main(int argc, char* argv[]) {
  int t = (argc > 1) ? atoi(argv[1]) : omp_get_num_procs();
  ImageInverter ii("../assets/pics/Car-colors.jpg", 1022, 1024);
  ii.run(t);
}
