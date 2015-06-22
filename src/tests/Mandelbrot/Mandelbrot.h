/*
 * Mandelbrot.cpp
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
 *(NEEDS DOCUMENTATION)
 */

#ifndef MANDELBROT_H_
#define MANDELBROT_H_

//Imports, constants....
#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

using namespace tsgl;

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

class Mandelbrot {
private:
  Decimal myFirstX, myFirstY, mySecondX, mySecondY;
protected:
  int myThreads;
  unsigned int myDepth;
  bool myRedraw;
public:
  /*!
   *
   */
  Mandelbrot(unsigned threads, unsigned depth);

  /*!
   *
   */
  virtual ~Mandelbrot() {};

  /*!
   *
   */
  void bindings(CartesianCanvas& can);

  /*!
   *
   */
  virtual void draw(CartesianCanvas& can);

  /*!
   *
   */
  virtual void setRedraw(bool newValue); //mutator
};

#endif /* MANDELBROT_H_ */
