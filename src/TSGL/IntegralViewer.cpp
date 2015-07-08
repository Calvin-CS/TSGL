#include "IntegralViewer.h"

using namespace tsgl;

Integral::Integral(functionPointer f, Decimal startX, Decimal stopX, Decimal startY, Decimal stopY, std::string fname) {
  myF = f;
  myStartX = startX; myStopX = stopX;
  myStartY = startY; myStopY = stopY;
  myRecTime = myTrapTime = 0;
  myDelay = FRAME;
  std::stringstream ss;
  ss.str("Integral of " + fname + " from " + to_string(myStartX) + " to " + to_string(myStopX));
  setupCanvas(myTrapCanvas, ss.str() + " using Trapezoids", myDelay); //Canvas for trapezoid method
  setupCanvas(myRecCanvas, ss.str() + " using Rectangles", myDelay);  //Canvas for rectangle method
}

Integral::~Integral() {
  if (myRecCanvas->getIsOpen())
    myRecCanvas->stop();
  if (myTrapCanvas->getIsOpen())
    myTrapCanvas->stop();
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  delete myRecCanvas;
  delete myTrapCanvas;
}

void Integral::setupCanvas(CartesianCanvas*& can, const std::string& label, double delay) {
  const float BORDER = (myStopX-myStartX)/10.0f;
  const float SPACING = BORDER;
  can = new CartesianCanvas(-1, -1, 800, 800, myStartX - BORDER, myStartY - BORDER,
                            myStopX + BORDER, myStopY + BORDER, label, delay);

  can->drawRectangle(myStartX,myStartY,myStopX,myStopY,WHITE);        //Area we're drawing to
  can->drawPartialFunction(myF,myStartX,myStopX,0,ColorInt(0,0,255)); //Outline of function
  can->drawAxes(0, 0, SPACING, SPACING);                              //Axes marks
//  can->drawText("+1", myStopX, myStopY, 32);
//  can->drawText("+1", myStartX, myStopY, 32);
  can->start();
}

long double Integral::rectangleEvaluate(long long numRectangles) {
  double startTime = omp_get_wtime();
  long double result = 0.0,
              recWidth = fabs(myStopX - myStartX) / numRectangles,
              halfRecWidth = recWidth / 2.0;
  #pragma omp parallel reduction(+:result)
  {
    long double xLo = 0.0, xMid = 0.0, y = 0.0;
    ColorFloat tcol = Colors::highContrastColor(omp_get_thread_num());
    tcol.A = 0.7;

    #pragma omp for
    for (long long i = 0; i < numRectangles; i++) {
      if (!myRecCanvas->getIsOpen()) continue;
      xLo = myStartX + i * recWidth;
      xMid = xLo + halfRecWidth;
      y = (*myF)(xMid);
      result += y;
      myRecCanvas->drawRectangle(xLo, 0, xLo+recWidth, y, tcol);
      myRecCanvas->sleep();
    }
    result *= recWidth;
  }
  myRecTime = omp_get_wtime() - startTime;
  myRecCanvas->wait();
  return result;
}

long double Integral::trapezoidEvaluate(long long numTrapezoids) {
  double startTime = omp_get_wtime();
  long double result = 0.0,
              trapWidth = fabs(myStopX - myStartX) / numTrapezoids,
              halfTrapWidth = trapWidth / 2.0;
  #pragma omp parallel
  {
    long double leftX = 0.0, rightX = 0.0, leftY = 0.0, rightY = 0.0;
    long double xValues[4] = {0.0}, yValues[4] = {0.0};
    ColorFloat tcol = Colors::highContrastColor(omp_get_thread_num());
    tcol.A = 0.7;
    ColorFloat colorValues[4] = {tcol, tcol, tcol, tcol};

    #pragma omp for reduction(+:result)
    for (long long i = 0; i < numTrapezoids; i++) {
      if (!myTrapCanvas->getIsOpen()) continue;
      leftX = myStartX + i * trapWidth;
      rightX = leftX + trapWidth;
      leftY = (*myF)(leftX);
      rightY = (*myF)(rightX);
      result += ((leftY + rightY) * halfTrapWidth);

      xValues[0] = xValues[1] = leftX;
      xValues[2] = xValues[3] = rightX;
      yValues[1] = leftY; yValues[2] = rightY;

      myTrapCanvas->drawConvexPolygon(4, xValues, yValues, colorValues);
      myTrapCanvas->sleep();
    }
  }
  myTrapTime = omp_get_wtime() - startTime;
  myTrapCanvas->wait();
  return result;
}

