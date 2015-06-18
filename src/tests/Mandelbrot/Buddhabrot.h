/*
 * Buddhabrot.h
 *
 *  Created on: June 18, 2015
 *      Author: Patrick Crain
 */

#ifndef BUDDHABROT_H_
#define BUDDHABROT_H_

#include <stdlib.h>

#include "Mandelbrot.h"

using namespace tsgl;

class Buddhabrot : public Mandelbrot {
private:
  int **counter;
  int cww, cwh;
public:
  Buddhabrot(unsigned threads, unsigned depth);
  ~Buddhabrot();
  void draw(CartesianCanvas& can, unsigned int & numberOfThreads);
};

#endif /* BUDDHABROT_H_ */

