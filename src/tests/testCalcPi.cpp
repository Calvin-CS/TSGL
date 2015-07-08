/*
 * testCalcPi.cpp
 *
 *  Created on: Apr 23, 2015
 *  Authors: Joel Adams and Patrick Crain, at Calvin College
 *
 *  Usage: ./testCalcPi [numIntervals] [numThreads]
 */

#include <stdio.h>              // printf(), ...
#include <string>               // stoll(), ...
#include <stdlib.h>             // exit()
#include <omp.h>                // OpenMP functions
#include <tsgl.h>               // IntegralViewer

inline Decimal unitCircleFunction(long double x) {
  return (abs(x) < 1.0L) ? sqrt( 1.0L - (x*x) ) : 0.0L;
}

inline Decimal arbitraryFunction(long double x) {
  Decimal d = cos(16*x);
  return d > 0 ? d : -d;
}

int main(int argc, char** argv) {
  //Handle command line
  if (argc > 3) {
    fprintf(stderr, "\nUsage: calcPI2 [intervals] [numThreads]\n\n");
    exit(1);
  }
  long long numIntervals = (argc > 1) ? std::stoll(argv[1], 0, 10) : 1;
  unsigned numThreads = (argc > 2) ? std::stoll(argv[2], 0, 10) : 1;

  //Setup
  omp_set_num_threads(numThreads);
  functionPointer fPtr = &unitCircleFunction;
//  functionPointer fPtr = &arbitraryFunction;
  Integral i(fPtr, 0.0l, 1.0l, 0.0l, 1.0l, "unit circle");

  //Go!
  printf("Reference pi:  3.141592653589793238462643383279...)\n");
  long double rectanglesPi = i.rectangleEvaluate(numIntervals) * 4.0;
  printf("Rectangles pi: %32.30Lf in %f secs\n", rectanglesPi, i.getRecTime() );
  long double trapezoidsPi = i.trapezoidEvaluate(numIntervals) * 4.0;
  printf("Trapezoids pi: %32.30Lf in %f secs\n", trapezoidsPi, i.getTrapTime() );
}
