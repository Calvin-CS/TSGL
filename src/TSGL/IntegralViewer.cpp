#include "IntegralViewer.h"

namespace tsgl {

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

 /*!
  * \brief IntegralViewer destructor method.
  * \details This is the destructor for the IntegralViewer class.
  * \details Frees up memory that was allocated to a IntegralViewer instance.
  */
IntegralViewer::~IntegralViewer() {
    if (myRecCanvas->isOpen())
        myRecCanvas->stop();
    if (myTrapCanvas->isOpen())
        myTrapCanvas->stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    delete myRecCanvas;
    delete myTrapCanvas;
}

void IntegralViewer::drawLabels(CartesianCanvas*& can) {
    CartesianBackground * bg = can->getBackground();
    const float /* cpw = can->getPixelWidth(), */ cph = can->getPixelHeight();
    const float FSIZE = cph * 30;
    const float /* xoff = cpw*FSIZE, */ yoff = cph * 20;

    bool blorigin = (myStartX == 0 && myStartY == 0);
    bool tlorigin = (myStopX == 0 && myStopY == 0);

    std::string startXString = to_string(myStartX);
    std::string startYString = to_string(myStartY);
    std::string stopXString = to_string(myStopX);
    std::string stopYString = to_string(myStopY);

    bg->drawText(cph * 10 + myStartX + cph * 2 * startXString.size(), myStartY-yoff, 0, startXString, "./assets/freefont/FreeSerif.ttf", FSIZE, 0,0,0, BLACK);
    bg->drawText(-cph * 10 + myStopX + cph * 2 * stopXString.size(), myStartY-yoff, 0, stopXString, "./assets/freefont/FreeSerif.ttf", FSIZE, 0,0,0, BLACK);
    if (!blorigin)
        bg->drawText(cph * 10 + cph * 2 * startYString.size(), myStartY-yoff, 0, startYString, "./assets/freefont/FreeSerif.ttf", FSIZE, 0,0,0, BLACK);
    if (!tlorigin)
        bg->drawText(cph * 10 + cph * 2 * stopYString.size(), myStopY+yoff, 0, stopYString, "./assets/freefont/FreeSerif.ttf", FSIZE, 0,0,0, BLACK);
}

void IntegralViewer::setupCanvas(CartesianCanvas*& can, const std::string& label, double delay) {
    const float BORDER = (myStopX-myStartX)/10.0f;
    const float SPACING = BORDER;
    can = new CartesianCanvas(-1, -1, myWidth, myHeight, myStartX - BORDER, myStartY - BORDER,
                                myStopX + BORDER, myStopY + BORDER, label, GRAY, delay);

    // can->setBackgroundColor(ColorFloat(0.95f, 0.95f, 0.95f, 1.0f));
    CartesianBackground * bg = can->getBackground();

    can->start();

    bg->drawRectangle((myStartX + myStopX)/2,(myStartY+myStopY)/2,0, myStopX - myStartX, myStopY - myStartY, 0,0,0,WHITE);        //Area we're drawing to
    bg->drawPartialFunction(myF,myStartX,myStopX,ColorInt(0,0,255)); //Outline of function
    bg->drawAxes(0, 0, SPACING, SPACING);                              //Axes marks
    drawLabels(can);
}

 /*!
  * \brief Evaluate an integral using the rectangle method.
  *   \param numRectangles The number of rectangles to use for the integration.
  * \return The area under the curve represented by the IntegralViewer's function, evaluated using
  *   the rectangle method.
  */
long double IntegralViewer::rectangleEvaluate(long long numRectangles) {
    CartesianBackground * bg = myRecCanvas->getBackground();
    double startTime = omp_get_wtime();
    long double result = 0.0,
                recWidth = fabs(myStopX - myStartX) / numRectangles,
                halfRecWidth = recWidth / 2.0;
    #pragma omp parallel reduction(+:result)
    {
        Decimal xLo = 0.0, xMid = 0.0, y = 0.0;
        ColorFloat tcol = Colors::highContrastColor(omp_get_thread_num());
        tcol.A = 0.7f;

        #pragma omp for
        for (long long i = 0; i < numRectangles; ++i) {
            if (!myRecCanvas->isOpen()) continue;
            myRecCanvas->sleep();
            xLo = myStartX + i  * recWidth;
            xMid = xLo + halfRecWidth;
            y = (*myF)(xMid);
            result += y;
            bg->drawRectangle(xMid, y/2,0, recWidth, abs(y), 0,0,0, tcol);
        }
        result *= recWidth;
    }
    myRecTime = omp_get_wtime() - startTime;
    myRecCanvas->wait();
    return result;
}

 /*!
  * \brief Evaluate an integral using the trapezoid method.
  *   \param numRectangles The number of trapezoids to use for the integration.
  * \return The area under the curve represented by the IntegralViewer's function, evaluated using
  *   the trapezoid method.
  */
long double IntegralViewer::trapezoidEvaluate(long long numTrapezoids) {
    CartesianBackground * cart = myTrapCanvas->getBackground();
    double startTime = omp_get_wtime();
    long double result = 0.0,
                trapWidth = fabs(myStopX - myStartX) / numTrapezoids,
                halfTrapWidth = trapWidth / 2.0;
    #pragma omp parallel reduction(+:result)
    {
        Decimal leftX = 0.0, rightX = 0.0, leftY = 0.0, rightY = 0.0;
        float xValues[4] = {0.0}, yValues[4] = {0.0};
        Decimal centerX, centerY;
        ColorFloat tcol = Colors::highContrastColor(omp_get_thread_num());
        tcol.A = 0.7;
        ColorFloat colorValues[4] = {tcol, tcol, tcol, tcol};

        #pragma omp for
        for (long long i = 0; i < numTrapezoids; ++i) {
            if (!myTrapCanvas->isOpen()) continue;
            leftX = myStartX + i * trapWidth;
            rightX = leftX + trapWidth;
            leftY = (*myF)(leftX);
            rightY = (*myF)(rightX);
            result += ((leftY + rightY) * halfTrapWidth);

            xValues[0] = xValues[1] = leftX;
            xValues[2] = xValues[3] = rightX;
            yValues[1] = leftY; yValues[2] = rightY;
            centerX = (leftX + rightX) / 2;
            centerY = (leftY + rightY) / 2;

            cart->drawConvexPolygon(centerX, centerY, 0, 4, xValues, yValues, 0,0,0, tcol);
            myTrapCanvas->sleep();
        }
    }
    myTrapTime = omp_get_wtime() - startTime;
    myTrapCanvas->wait();
    return result;
}

}
