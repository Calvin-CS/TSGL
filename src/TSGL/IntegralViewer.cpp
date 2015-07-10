#include "IntegralViewer.h"

using namespace tsgl;

IntegralViewer::IntegralViewer(functionPointer f, int width, int height, Decimal startX, Decimal stopX, Decimal startY, Decimal stopY, std::string fname) {
  myF = f;
  myWidth = width; myHeight = height;
  myStartX = startX; myStopX = stopX;
  myStartY = startY; myStopY = stopY;
  myRecTime = myTrapTime = 0;
  myDelay = FRAME;
  std::stringstream ss;
  ss.str("Integral of " + fname + " from " + to_string(myStartX) + " to " + to_string(myStopX));
  setupCanvas(myTrapCanvas, ss.str() + " using Trapezoids", myDelay); //Canvas for trapezoid method
  setupCanvas(myRecCanvas, ss.str() + " using Rectangles", myDelay);  //Canvas for rectangle method
}

IntegralViewer::~IntegralViewer() {
  if (myRecCanvas->getIsOpen())
    myRecCanvas->stop();
  if (myTrapCanvas->getIsOpen())
    myTrapCanvas->stop();
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  delete myRecCanvas;
  delete myTrapCanvas;
}

void IntegralViewer::drawLabels(CartesianCanvas*& can) {
  const int FSIZE = 32;
  const float cpw = can->getPixelWidth(), cph = can->getPixelHeight();
  const float xoff = cpw*FSIZE, yoff = cph*FSIZE;

  bool blorigin = (myStartX == 0 && myStartY == 0);
  bool tlorigin = (myStartX == 0 && myStopY == 0);
//  bool brorigin = (myStopX == 0 && myStartY == 0);
//  bool trorigin = (myStopX == 0 && myStopY == 0);

  can->drawText(to_string(myStartX), myStartX, myStartY-yoff, FSIZE);
  can->drawText(to_string(myStopX), myStopX, myStartY-yoff, FSIZE);
  if (!blorigin)
    can->drawText(to_string(myStartY), 0, myStartY-1.5f*yoff, FSIZE);
  if (!tlorigin)
    can->drawText(to_string(myStopY), 0, myStopY, FSIZE);
}

void IntegralViewer::setupCanvas(CartesianCanvas*& can, const std::string& label, double delay) {
  const float BORDER = (myStopX-myStartX)/10.0f;
  const float SPACING = BORDER;
  can = new CartesianCanvas(-1, -1, myWidth, myHeight, myStartX - BORDER, myStartY - BORDER,
                            myStopX + BORDER, myStopY + BORDER, label, delay);

  can->setBackgroundColor(ColorFloat(0.9f, 0.9f, 0.9f, 1.0f));

  can->drawRectangle(myStartX,myStartY,myStopX,myStopY,WHITE);        //Area we're drawing to
  can->drawPartialFunction(myF,myStartX,myStopX,0,ColorInt(0,0,255)); //Outline of function
  can->drawAxes(0, 0, SPACING, SPACING);                              //Axes marks
  drawLabels(can);
  can->start();
}

long double IntegralViewer::rectangleEvaluate(long long numRectangles) {
  double startTime = omp_get_wtime();
  long double result = 0.0,
              recWidth = fabs(myStopX - myStartX) / numRectangles,
              halfRecWidth = recWidth / 2.0;
  #pragma omp parallel reduction(+:result)
  {
    int tid = omp_get_thread_num(), nthreads = omp_get_num_threads();
    float bsize = numRectangles*1.0f/nthreads;
    int start = bsize*tid, end = bsize*(tid+1);

    long double xLo = 0.0, xMid = 0.0, y = 0.0;
    ColorFloat tcol = Colors::highContrastColor(tid);
    tcol.A = 0.7f;

    for (long long i = start; i < end; ++i) {
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

long double IntegralViewer::trapezoidEvaluate(long long numTrapezoids) {
  double startTime = omp_get_wtime();
  long double result = 0.0,
              trapWidth = fabs(myStopX - myStartX) / numTrapezoids,
              halfTrapWidth = trapWidth / 2.0;
  #pragma omp parallel reduction(+:result)
  {
    int tid = omp_get_thread_num(), nthreads = omp_get_num_threads();
    float bsize = numTrapezoids*1.0f/nthreads;
    int start = bsize*tid, end = bsize*(tid+1);

    long double leftX = 0.0, rightX = 0.0, leftY = 0.0, rightY = 0.0;
    long double xValues[4] = {0.0}, yValues[4] = {0.0};
    ColorFloat tcol = Colors::highContrastColor(omp_get_thread_num());
    tcol.A = 0.7;
    ColorFloat colorValues[4] = {tcol, tcol, tcol, tcol};

    for (long long i = start; i < end; ++i) {
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

