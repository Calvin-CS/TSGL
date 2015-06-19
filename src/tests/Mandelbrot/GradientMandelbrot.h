/*
 * GradientMandelbrot.cpp
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
 */

#ifndef GRADIENTMANDELBROT_H_
#define GRADIENTMANDELBROT_H_

#include "Mandelbrot.h"

using namespace tsgl;

class GradientMandelbrot : public Mandelbrot {
public:
  /*!
   *
   */
  GradientMandelbrot(unsigned threads, unsigned depth);
  /*!
   *
   */
  void draw(CartesianCanvas& can);
  /*!
   *
   */
  void setRedraw(bool newValue);
};

#endif /* GRADIENTMANDELBROT_H_ */

