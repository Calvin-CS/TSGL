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

  /*!
   * \brief Default IntegralViewer constructor method.
   * \details This is the default constructor for the IntegralViewer class.
   *   \param f A function to integrate and display. The function must accept exactly one argument of type
   *     Decimal, and return a Decimal.
   *   \param width The width of the window displaying the integration.
   *   \param height The height of the window displaying the integration.
   *   \param startX The minimum x-value whose y-value should be computed.
   *   \param stopX The maximum x-value whose y-value should be computed.
   *   \param startY The minimum y-value that should be displayed on the IntegralViewer's Canvas.
   *   \param stopY The maximum y-value that should be displayed on the IntegralViewer's Canvas.
   *   \param fname A descriptive name for the function you wish to integrate.
   * \return A new IntegralViewer with the specified dimensions, bounds, and function description.
   */
  IntegralViewer(functionPointer f, int width, int height, Decimal startX, Decimal stopX, Decimal startY = 0, Decimal stopY = 1,
           std::string fname  = "function");

  /*!
   * \brief IntegralViewer destructor method.
   * \details This is the destructor for the IntegralViewer class.
   * \details Frees up memory that was allocated to a IntegralViewer instance.
   */
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

  /*!
   * \brief Evaluate an integral using the rectangle method.
   *   \param numRectangles The number of rectangles to use for the integration.
   * \return The area under the curve represented by the IntegralViewer's function, evaluated using
   *   the rectangle method.
   */
  long double rectangleEvaluate(long long numRectangles);

  /*!
   * \brief Evaluate an integral using the trapezoid method.
   *   \param numRectangles The number of trapezoids to use for the integration.
   * \return The area under the curve represented by the IntegralViewer's function, evaluated using
   *   the trapezoid method.
   */
  long double trapezoidEvaluate(long long numTrapezoids);
};

}

#endif /* INTEGRALVIEWER_H_ */

