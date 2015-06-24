#include "CartesianCanvas.h"

namespace tsgl {

CartesianCanvas::CartesianCanvas(double timerLength)
    : Canvas(timerLength) {
    recomputeDimensions(-400, -300, 400, 300);
}

CartesianCanvas::CartesianCanvas(int xx, int yy, int w, int h, Decimal xMin, Decimal yMin, Decimal xMax,
                                 Decimal yMax, std::string t, double timerLength)
    : Canvas(xx, yy, w, h, t, timerLength) {
    recomputeDimensions(xMin, yMin, xMax, yMax);
}

void CartesianCanvas::drawAxes(Decimal x, Decimal y, Decimal dx = 0, Decimal dy = 0) {
    drawLine(maxX, y, minX, y);  // Make the two axes
    drawLine(x, maxY, x, minY);

    if (dx != 0.0) {
        if (dx < 0.0) dx = -dx;

        for (Decimal x_ = x + dx; x_ < maxX; x_ += dx) {
            drawLine(x_, y + 8 * pixelHeight, x_, y - 8 * pixelHeight);
        }
        for (Decimal x_ = x - dx; x_ > minX; x_ -= dx) {
            drawLine(x_, y + 8 * pixelHeight, x_, y - 8 * pixelHeight);
        }
    }
    if (dy != 0.0) {
        if (dy < 0.0) dy = -dy;

        for (Decimal y_ = y + dy; y_ < maxY; y_ += dy) {
            drawLine(x + 8 * pixelWidth, y_, x - 8 * pixelWidth, y_);
        }
        for (Decimal y_ = y - dy; y_ > minY; y_ -= dy) {
            drawLine(x + 8 * pixelWidth, y_, x - 8 * pixelWidth, y_);
        }
    }
}

void CartesianCanvas::drawCircle(Decimal x, Decimal y, Decimal radius, int res, ColorFloat color, bool filled) {
    int actualX, actualY, actualR;
    getScreenCoordinates(x, y, actualX, actualY);
    getScreenCoordinates(x+radius,y,actualR,actualY);
    actualR -= actualX;
    Canvas::drawCircle(actualX, actualY, actualR, res, color, filled);
}

void CartesianCanvas::drawColoredPolygon(int size, Decimal x[], Decimal y[], ColorFloat color[], bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(x[i], y[i], int_x[i], int_y[i]);
    }
    Canvas::drawColoredPolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

void CartesianCanvas::drawConcavePolygon(int size, Decimal x[], Decimal y[], ColorFloat color[], bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(x[i], y[i], int_x[i], int_y[i]);
    }
    Canvas::drawConcavePolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

void CartesianCanvas::drawConvexPolygon(int size, Decimal x[], Decimal y[], ColorFloat color[], bool filled) {
    int* int_x = new int[size];
    int* int_y = new int[size];

    for (int i = 0; i < size; i++) {
        getScreenCoordinates(x[i], y[i], int_x[i], int_y[i]);
    }
    Canvas::drawConvexPolygon(size, int_x, int_y, color, filled);

    delete int_x;
    delete int_y;
}

void CartesianCanvas::drawFunction(const Function &f, ColorFloat color) {
    int screenX = 0, screenY = 0;
    Polyline *p = new Polyline(1 + (maxX - minX) / pixelWidth);
    for (Decimal x = minX; x <= maxX; x += pixelWidth) {
        getScreenCoordinates(x, f.valueAt(x), screenX, screenY);
        p->addNextVertex(screenX, screenY, color);
    }

    drawShape(p);
}

void CartesianCanvas::drawImage(std::string fname, Decimal x, Decimal y, Decimal w, Decimal h, float a) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x, y, actualX1, actualY1);
    getScreenCoordinates(x + w, y - h, actualX2, actualY2);

    Canvas::drawImage(fname, actualX1, actualY1, actualX2 - actualX1, actualY2 - actualY1, a);
}

void CartesianCanvas::drawLine(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);

    Canvas::drawLine(actualX1, actualY1, actualX2, actualY2, color);
}

void CartesianCanvas::drawPoint(Decimal x, Decimal y, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawPoint(actualX, actualY, color);
}

void CartesianCanvas::drawRectangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, ColorFloat color, bool filled) {
    int actualX1, actualY1, actualX2, actualY2;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);  //Change y - h to y + h
    Canvas::drawRectangle(actualX1, actualY1, actualX2, actualY2, color, filled);
}

void CartesianCanvas::drawText(std::string s, Decimal x, Decimal y, unsigned size, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawText(s, actualX, actualY, size, color);
}

void CartesianCanvas::drawText(std::wstring s, Decimal x, Decimal y, unsigned size, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawText(s, actualX, actualY, size, color);
}

void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color, bool filled) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, color, filled);
}

void CartesianCanvas::getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY) {
    cartX = (screenX * cartWidth) / getWindowWidth() + minX;
    cartY = minY - (screenY - getWindowHeight()) * cartHeight / getWindowHeight();
}

Decimal CartesianCanvas::getCartHeight() {
    return cartHeight;
}

Decimal CartesianCanvas::getCartWidth() {
    return cartWidth;
}

Decimal CartesianCanvas::getPixelWidth() {
    return pixelWidth;
}

Decimal CartesianCanvas::getPixelHeight() {
    return pixelHeight;
}

Decimal CartesianCanvas::getMaxX() {
    return maxX;
}

Decimal CartesianCanvas::getMaxY() {
    return maxY;
}

Decimal CartesianCanvas::getMinX() {
    return minX;
}

Decimal CartesianCanvas::getMinY() {
    return minY;
}

void CartesianCanvas::getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY) {
    screenX = ceil((cartX - minX) / cartWidth * getWindowWidth());
    screenY = ceil(getWindowHeight() - (cartY - minY) / cartHeight * getWindowHeight());
}

void CartesianCanvas::recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax) {
    minX = xMin;
    minY = yMin;
    maxX = xMax;
    maxY = yMax;
    cartWidth = maxX - minX;
    cartHeight = maxY - minY;
    Decimal xError = cartWidth / getWindowWidth();
    Decimal yError = cartHeight / getWindowHeight();
    pixelWidth = (cartWidth - xError) / (getWindowWidth() + xError);
    pixelHeight = (cartHeight - yError) / (getWindowHeight() + yError);
}

void CartesianCanvas::reset() {
    Canvas::reset();
}

void CartesianCanvas::sleep() {
    Canvas::sleep();
}

void CartesianCanvas::zoom(Decimal x, Decimal y, Decimal scale) {
    Decimal newWidth = cartWidth * scale;
    Decimal newHeight = cartHeight * scale;
    recomputeDimensions(x - .5 * newWidth, y - .5 * newHeight, x + .5 * newWidth, y + .5 * newHeight);
}

void CartesianCanvas::zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2) {
    Decimal scale = (std::abs(x2 - x1) / cartWidth + std::abs(y2 - y1) / cartHeight) / 2.0;
    zoom((x2 + x1) / 2, (y2 + y1) / 2, scale);
}

//-----------------------Unit testing-------------------------------------------------
void CartesianCanvas::runTests() {
  TsglDebug("Testing CartesianCanvas class...");
  CartesianCanvas c1(0.0f);
  c1.setBackgroundColor(WHITE);
  c1.start();

  std::this_thread::sleep_for(std::chrono::seconds(1));
  tsglAssert(testZoom(c1), "Unit test for zoom() functions failed!");
  tsglAssert(testRecomputeDimensions(c1), "Unit test for recomputing dimensions failed!");
  c1.stop();
  TsglDebug("Unit tests for CartesianCanvas complete.");
}

//bool CartesianCanvas::testAxes(CartesianCanvas& can) {
//   int passed = 0;
//   int failed = 0;
//   can.drawAxes(0, 0, 10, 10);
// //  can.sleep();
//   ColorInt black(0, 0, 0);
//   if(can.getPixel(0, 0) == black && can.getPixel(0, 10) == black) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, distance between axes for testAxes() failed!");
//   }
//
////   while(can.getIsOpen()) {
////     std::cout << can.getMouseX() << " " << can.getMouseY() << std::endl;
////   }
////   for(unsigned i = 0; i < can.getWindowWidth(); i++) {
////     std::cout << can.getPixel(460, i).AsString() << std::endl;
////   }
//
//   if(passed == 1 && failed == 0) {
//     TsglDebug("Unit test for drawing axes passed!");
//     return true;
//   } else {
//     TsglErr("This many tests passed for testAxes(): ");
//     std::cout << " " << passed << std::endl;
//     TsglErr("This many tests failed for testAxes(): ");
//     std::cout << " " << failed << std::endl;
//     return false;
//   }
//}

bool CartesianCanvas::testZoom(CartesianCanvas& can) {
    int passed = 0;
    int failed = 0;
    //Test 1: Zooming out
    //Had to use round() because there was a floating-point error that
    //propagated to the rest of the tests after the first two
    can.zoom(0, 0, 1.5);
    if(round(can.getCartWidth()) == 1200 && round(can.getCartHeight()) == 900) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, Zooming out for testZoom() failed!");
    }

    //Test 2: Zooming in
    can.zoom(0, 0, .5);
    if(round(can.getCartWidth()) == 600 && round(can.getCartHeight()) == 450) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Zooming in for testZoom() failed!");
    }

    //Test 3: Zooming out/in on a different point

    //Zooming out....
    can.zoom(10, 10, 1.2);
    if(round(can.getCartWidth()) == 720 && round(can.getCartHeight()) == 540) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Zooming out on a different point for testZoom() failed!");
    }

    //Zooming in....
    can.zoom(15, 20, .9);
    if(round(can.getCartWidth()) == 648 && round(can.getCartHeight()) == 486) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Zooming in on a different point for testZoom() failed!");
    }

    //Results:
    if(passed == 4 && failed == 0) {
      TsglDebug("Unit test for zooming in & out passed!");
      return true;
    } else {
      TsglErr("This many passed for testZoom(): ");
      std::cout << " " << passed << std::endl;
      TsglErr("This many failed for testZoom(): ");
      std::cout << " " << failed << std::endl;
      return false;
    }
}

bool CartesianCanvas::testRecomputeDimensions(CartesianCanvas& can) {
   int passed = 0;
   int failed = 0;
   Decimal xMin, xMax;
   Decimal yMin, yMax;
   //Test 1: Positive values only (with 0.0)
   xMin = 0.0;
   xMax = 500.0;
   yMin = 0.0;
   yMax = 500.0;
   can.recomputeDimensions(xMin, yMin, xMax, yMax);
   if(can.getCartWidth() == 500.0 && can.getCartHeight() == 500.0) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 1, Positive values only for testRecomputeDimensions() failed!");
   }

   //Test 2: Negative values included
   xMin = xMax = yMin = yMax = 0.0;
   xMin = -300.0;
   xMax = 900.0;
   yMin = -500.0;
   yMax = 1000.0;
   can.recomputeDimensions(xMin, yMin, xMax, yMax);

   if(can.getCartWidth() == 1200.0 && can.getCartHeight() == 1500.0) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 2, Negative values for testRecomputeDimensions() failed!");
   }

   //Test 3: Same as Test 2, but negative values are max
   xMin = xMax = yMin = yMax = 0.0;
   xMin = -900.0;
   xMax = -100.0;
   yMin = -800.0;
   yMax = -50.0;
   can.recomputeDimensions(xMin, yMin, xMax, yMax);

   if(can.getCartWidth() == 800.0 && can.getCartHeight() == 750.0) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 3, Max negative values for testRecomputeDimensions() failed!");
   }

   if(passed == 3 && failed == 0) {
     TsglDebug("Unit test for recomputing dimensions passed!");
     return true;
   } else {
     TsglErr("This many tests passed for testRecomputeDimensions(): ");
     std::cout << " " << passed << std::endl;
     TsglErr("This many tests failed for testRecomputeDimensions(): ");
     std::cout << " " << failed << std::endl;
     return false;
   }
}
//-----------------End Unit testing----------------------------------------------------
}
