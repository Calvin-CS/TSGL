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

/*!
 * \brief Compute a Unit circle.
 * \details
 */
inline Decimal unitCircleFunction(long double x) {
  return (abs(x) < 1.0L) ? sqrt( 1.0L - (x*x) ) : 0.0L;
}

/*!
 *
 */
inline Decimal sineFunction(long double x) {
  return cos(8*x);
  Decimal d = cos(8*x);
  return d > 0 ? d : -d;
}

//Main method
int main(int argc, char** argv) {
  //Handle command line
  if (argc > 3) {
    fprintf(stderr, "\nUsage: calcPI2 [intervals] [numThreads]\n\n");
    exit(1);
  }

  //Check if the number of intervals and the number of threads was passed
  //If one was not, set it to a default value
  long long numIntervals = (argc > 1) ? std::stoll(argv[1], 0, 10) : 1;
  unsigned numThreads = (argc > 2) ? std::stoll(argv[2], 0, 10) : 1;

  //Setup
  omp_set_num_threads(numThreads);
  IntegralViewer i1(&unitCircleFunction, 800, 800, 0.0l, 1.0l, 0.0l, 1.0l, "unit circle");

  //Go!
  printf("Reference pi:  3.141592653589793238462643383279...)\n");
  long double rectanglesPi = i1.rectangleEvaluate(numIntervals) * 4.0;
  printf("Rectangles pi: %32.30Lf in %f secs\n", rectanglesPi, i1.getRecTime() );
  long double trapezoidsPi = i1.trapezoidEvaluate(numIntervals) * 4.0;
  printf("Trapezoids pi: %32.30Lf in %f secs\n", trapezoidsPi, i1.getTrapTime() );

  //Bonus!
  IntegralViewer i2(&sineFunction, 1200, 800, -1.1l, 1.2l, -1.3l, 1.4l, "cosine");
  i2.rectangleEvaluate(numIntervals);
  i2.trapezoidEvaluate(numIntervals);
}
