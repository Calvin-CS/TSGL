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

const int MAX_COLOR = 255;

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

class Mandelbrot {
private:
	Decimal myFirstX, myFirstY, mySecondX, mySecondY;
protected:
	int myThreads;
	unsigned int myDepth;
	bool myRedraw;
public:
  Mandelbrot();
  virtual ~Mandelbrot() {};
  void bindings(CartesianCanvas& can);
  virtual void draw(CartesianCanvas& can, unsigned int & numberOfThreads);
  virtual void setRedraw(bool newValue); //mutator
};

#endif /* MANDELBROT_H_ */
