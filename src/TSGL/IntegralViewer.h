/*
 * IntegralViewer.h
 */

#ifndef INTEGRALVIEWER_H_
#define INTEGRALVIEWER_H_

#include <string>
#include <omp.h>
#include <tsgl.h>
#include <sstream>
#include "Util.h"  //Decimal typedef

namespace tsgl {

/*! \class IntegralViewer
 *  \brief Provides a a tool for computing and visualizing integrals of functions
 *  \details IntegralViewer provides a simple interface for integrating functions and outputting the results of the
 *    integration, both numerically and visually. IntegralViewer can evaluate an arbitrary function of the type
 *    <code>Decimal myFunction(Decimal x)</code>, where x is the input value of the function and the return value is
 *    the y value of the function. IntegralViewer can compute integrals using one or both of the rectangle method
 *    and the trapezoid method, and provides functions for displaying both on a custom Canvas. Furthermore, these
 *    computations and visualizations are thread-safe; the number of threads to use can be set with
 *    <code>omp_set_num_threads()</code>.
 */
class IntegralViewer {
private:
  functionPointer myF;
  int myWidth, myHeight;
  long double myStartX, myStopX, myStartY, myStopY;
  double myRecTime, myTrapTime, myDelay;
  CartesianCanvas *myRecCanvas, *myTrapCanvas;

  void drawLabels(CartesianCanvas*& can);
  void setupCanvas(CartesianCanvas*& can, const std::string& label = "", double delay = 0.0);

public:

  IntegralViewer(functionPointer f, int width, int height, Decimal startX, Decimal stopX, Decimal startY = 0, Decimal stopY = 1,
           std::string fname  = "function");

  ~IntegralViewer();

  /*!
   * \brief Accessor for the time the canvas spent integrating using the rectangle method.
   * \return The elapsed time in seconds for the integration using rectangles.
   */
  double getRecTime() const  { return myRecTime; }

  /*!
   * \brief Accessor for the time the canvas spent integrating using the trapezoid method.
   * \return The elapsed time in seconds for the integration using trapezoids.
   */
  double getTrapTime() const { return myTrapTime; }

  long double rectangleEvaluate(long long numRectangles);

  long double trapezoidEvaluate(long long numTrapezoids);
};

}

#endif /* INTEGRALVIEWER_H_ */

