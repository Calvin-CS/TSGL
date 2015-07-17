///*
// * main.cpp is a driver for an ImageInverter program.
// *
// *  Created on: Nov 14, 2014
// *      Author: adams
// */
//
#include "ImageInverter/ImageInverter.h"

int main(int argc, char* argv[]) {
  int t = (argc > 1) ? atoi(argv[1]) : omp_get_num_procs();
  ImageInverter ii("../assets/pics/Car-colors.jpg", 1022, 1024);
  ii.run(t);
}
