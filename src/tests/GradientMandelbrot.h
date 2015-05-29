/*
 * GradientMandelbrot.cpp
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
 */

#ifndef GRADIENTMANDELBROT_H_
#define GRADIENTMANDELBROT_H_

#include "Mandelbrot.h"

class GradientMandelbrot : public Mandelbrot {
public:
  GradientMandelbrot();
  void draw(CartesianCanvas& can, unsigned int & numberOfThreads);
  void setRedraw(bool newValue);
};

#endif /* GRADIENTMANDELBROT_H_ */

