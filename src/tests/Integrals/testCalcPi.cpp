/*
 * testCalcPi.cpp
 *
 *  Created on: Apr 23, 2015
 *  Author: Joel Adams, at Calvin College
 *
 *  Usage: ./testCalcPi [numIntervals] [numThreads]
 */

#include "Integral.h"    // Integrator class
#include <stdio.h>       // printf(), ...
#include <string>        // stoll(), ...
#include <stdlib.h>      // exit()
#include <omp.h>           // OpenMP functions

long double f(long double x) {
    if (-1.0L < x && x < 1.0L) {
        return sqrt( 1.0L - (x*x) );
    } else {
        return 0.0L;
    }
}

int main(int argc, char** argv) {
  long long numIntervals = 0;
  unsigned numThreads = 0;
  processCommandLine(argc, argv, numIntervals, numThreads);
  functionPointer fPtr = &f;
  omp_set_num_threads(numThreads);

  printf("\nReference pi:  3.141592653589793238462643383279...)\n");

  Integral i(fPtr, 0.0l, 1.0l);

  long double rectanglesPi = i.evaluateUsingRectangleMethod(numIntervals) * 4.0;
  printf("Rectangles pi: %32.30Lf in %f secs\n",
                 rectanglesPi, i.getRecTime() );

  long double trapezoidsPi = i.evaluateUsingTrapezoidMethod(numIntervals) * 4.0;
  printf("Trapezoids pi: %32.30Lf in %f secs\n\n",
                 trapezoidsPi, i.getTrapTime() );
}

