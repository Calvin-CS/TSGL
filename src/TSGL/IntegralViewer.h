/*
 * IntegralViewer.h
 *
 *  Created on: Apr 23, 2015
 */

#ifndef INTEGRAL_H_
#define INTEGRAL_H_

#include <cmath>   // abs()
#include <string>
#include <omp.h>
#include <tsgl.h>
#include <sstream>
using namespace tsgl;

typedef Decimal (*functionPointer)(Decimal x);

class IntegralViewer {
private:
  functionPointer myF;
  int myWidth, myHeight;
  long double myStartX, myStopX, myStartY, myStopY;
  double myRecTime, myTrapTime, myDelay;
  CartesianCanvas *myRecCanvas, *myTrapCanvas;

  void drawLabels(CartesianCanvas*& can);
protected:
  void setupCanvas(CartesianCanvas*& can, const std::string& label = "", double delay = 0.0);
public:
  IntegralViewer(functionPointer f, int width, int height, Decimal startX, Decimal stopX, Decimal startY = 0, Decimal stopY = 1,
           std::string fname  = "function");
  ~IntegralViewer();
  double getRecTime() const  { return myRecTime; }
  double getTrapTime() const { return myTrapTime; }
  long double rectangleEvaluate(long long numRectangles);
  long double trapezoidEvaluate(long long numTrapezoids);
};

#endif /* INTEGRAL_H_ */

