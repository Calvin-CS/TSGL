/*
 * Integrator.h
 *
 *  Created on: Apr 23, 2015
 *      Author: adams
 */

#ifndef INTEGRAL_H_
#define INTEGRAL_H_

#include <cmath>   // abs()
#include <string>
#include <omp.h>
#include <tsgl.h>
using namespace tsgl;

typedef long double (*functionPointer)(long double x);

class UnitCircleFunction : public Function {
public:
  UnitCircleFunction() : Function() {}
  Decimal valueAt(Decimal x) const { return sqrt(1.0 - (x * x)); }
};


class Integral {
public:
  Integral(functionPointer f, long double startX, long double stopX);
  ~Integral();
  double getRecTime() const  { return myRecTime; }
  double getTrapTime() const { return myTrapTime; }

  long double evaluateUsingRectangleMethod(long long numRectangles);
  long double evaluateUsingTrapezoidMethod(long long numTrapezoids);

protected:
  void setupCanvas(CartesianCanvas*& canvas, const std::string& label = "", double delay = 0.0);

private:
  functionPointer myF;
  long double myX1;
  long double myX2;

  double myRecTime;
  double myTrapTime;
  double myDelay;

  CartesianCanvas *myRecCanvas;
  CartesianCanvas *myTrapCanvas;
};

inline
Integral::Integral(functionPointer f, long double startX, long double stopX) {
  myF = f;
  myX1 = startX;
  myX2 = stopX;

  myRecTime = myTrapTime = 0.0;
  myDelay = 0.2;

  // canvas for trapezoid method
  setupCanvas(myTrapCanvas, "Integral of Unit Circle from 0 to 1 using Trapezoids", myDelay);
  // canvas for rectangle method
  setupCanvas(myRecCanvas, "Integral of Unit Circle from 0 to 1 using Rectangles", myDelay);
}

inline
void Integral::setupCanvas(CartesianCanvas*& canvasPtr, const std::string& label, double delay) {
  canvasPtr = new CartesianCanvas(0, 0, 800, 800,
                                  myX1 - 0.1, -0.1,
                                  myX2 + 0.1, 1.1,
                                  label,
                                  delay);

  canvasPtr->start();
  canvasPtr->setBackgroundColor(WHITE);
  canvasPtr->drawAxes(0, 0, 0.1, 0.1);
  canvasPtr->drawCircle(0, 0, 1.0, 1024, ColorInt(0,0,255), false);

  canvasPtr->drawText(L"+1", 1.0, -0.05, 32);
  canvasPtr->drawText(L"+1", -0.05, 1.0, 32);

  // UnitCircleFunction unitCircle;
  // myCanvas1->drawFunction(unitCircle);
  // ToDo: Add color parameter to drawFunction
}


inline
Integral::~Integral() {
//  myRecCanvas->stop();
//  myTrapCanvas->stop();
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  delete myRecCanvas;
  delete myTrapCanvas;
}



inline
long double Integral::evaluateUsingRectangleMethod(long long numRectangles) {
  double startTime = omp_get_wtime();
  long double result = 0.0;
  long double recWidth = fabs(myX2 - myX1) / numRectangles;
  long double halfRecWidth = recWidth / 2.0;

  #pragma omp parallel reduction(+:result)
  {
    long double xLo = 0.0;
    long double xMid = 0.0;
    long double y = 0.0;
    ColorFloat threadColor =
                 Colors::highContrastColor( omp_get_thread_num() );
    threadColor.A = 0.7;

    #pragma omp for
    for (long long i = 0; i < numRectangles; i++) {
      xLo = myX1 + i * recWidth;
      xMid = xLo + halfRecWidth;
      y = (*myF)(xMid);
      result += y;

      myRecCanvas->drawRectangle(xLo, 0, xLo+recWidth, y, threadColor);
      myRecCanvas->sleepFor(0.1f);
    }
    result *= recWidth;
  }
  myRecTime = omp_get_wtime() - startTime;

  myRecCanvas->wait();

  return result;
}

inline
long double Integral::evaluateUsingTrapezoidMethod(long long numTrapezoids) {
  double startTime = omp_get_wtime();
  long double result = 0.0;
  long double trapWidth = fabs(myX2 - myX1) / numTrapezoids;
  long double halfTrapWidth = trapWidth / 2.0;

  #pragma omp parallel
  {
    long double leftX = 0.0, rightX = 0.0;
    long double leftY = 0.0, rightY = 0.0;

    long double xValues[4] = {0.0}, yValues[4] = {0.0};
    ColorFloat threadColor = Colors::highContrastColor(
                                      omp_get_thread_num() );
    threadColor.A = 0.7;
    ColorFloat colorValues[4] = {threadColor, threadColor,
                                 threadColor, threadColor};

    #pragma omp for reduction(+:result)
    for (long long i = 0; i < numTrapezoids; i++) {
      leftX = myX1 + i * trapWidth;
      rightX = leftX + trapWidth;
      leftY = (*myF)(leftX);
      rightY = (*myF)(rightX);
      result += ((leftY + rightY) * halfTrapWidth);

      xValues[0] = leftX;  yValues[0] = 0;
      xValues[1] = leftX;  yValues[1] = leftY;
      xValues[2] = rightX; yValues[2] = rightY;
      xValues[3] = rightX; yValues[3] = 0;

      myTrapCanvas->drawConvexPolygon(4, xValues, yValues,
                                             colorValues);
      myTrapCanvas->sleepFor(0.1f);
    }
  }

  myTrapTime = omp_get_wtime() - startTime;

  myTrapCanvas->wait();

  return result;
}

/* process the command-line arguments
 * Parameters: argc, an int
 *             argv a char**
 *             intervals, an int&
 *             threads, an int&
 * Postcondition:
 *  -  intervals (number of rectangles or trapezoids)
         and threads have been defined,
 *       according to the values the user specified when
 *       the program was invoked.
 */
inline
void processCommandLine(int argc, char ** argv, long long& intervals, unsigned& threads) {
   if ( argc == 3 ) {
      intervals = std::stoll(argv[1], 0, 10);
      threads = std::stoul(argv[2], 0, 10);
   } else if ( argc == 2 ) {
      intervals = std::stoll(argv[1], 0, 10);
      threads = 1;
   } else if ( argc == 1 ) {
      intervals = 1;
      threads = 1;
   } else {
      fprintf(stderr, "\nUsage: calcPI2 [intervals] [numThreads]\n\n");
      exit(1);
   }
}

#endif /* INTEGRAL_H_ */

