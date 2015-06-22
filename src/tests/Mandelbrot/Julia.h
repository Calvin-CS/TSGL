#ifndef JULIA_H_
#define JULIA_H_

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

class Julia {
private:
  Decimal myFirstX, myFirstY, mySecondX, mySecondY;
protected:
  int myThreads;
  unsigned int myDepth;
  bool myRedraw;
public:
  Julia(unsigned threads, unsigned depth);
  virtual ~Julia();
  void bindings(CartesianCanvas& can);
  virtual void draw(CartesianCanvas& can);
  virtual void setRedraw(bool newValue); //mutator
};

#endif /* JULIA_H_ */
