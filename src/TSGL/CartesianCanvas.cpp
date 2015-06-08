#include "CartesianCanvas.h"

#ifdef _WIN32
namespace tsgl {
#endif

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
    Canvas::drawConcavePolygon(size, int_x, int_y, color, filled);

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

void CartesianCanvas::drawText(std::wstring s, Decimal x, Decimal y, unsigned int size, ColorFloat color) {
    int actualX, actualY;
    getScreenCoordinates(x, y, actualX, actualY);

    Canvas::drawText(s, actualX, actualY, size, color);
}

void CartesianCanvas::drawTriangle(Decimal x1, Decimal y1, Decimal x2, Decimal y2, Decimal x3, Decimal y3, ColorFloat color, bool filled) {
    int actualX1, actualY1, actualX2, actualY2, actualX3, actualY3;
    getScreenCoordinates(x1, y1, actualX1, actualY1);
    getScreenCoordinates(x2, y2, actualX2, actualY2);
    getScreenCoordinates(x3, y3, actualX3, actualY3);
    Canvas::drawTriangle(actualX1, actualY1, actualX2, actualY2, actualX3, actualY3, BLACK, false);
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
  CartesianCanvas c1(0, 0, 400, 400, -400, 400, -400, 400, "", 0.1f);
  c1.start();
  tsglAssert(testAxes(c1), "Unit test for drawing axes failed");
  tsglAssert(testZoom(c1), "Unit test for zoom() functions failed");
  tsglAssert(testRecomputeDimensions(c1), "Unit test for recomputing dimensions failed");
  tsglAssert(testDraw(c1), "Unit test for drawing failed");
  std::cout << "All unit tests passed!" << std::endl;
  c1.wait();
}

bool CartesianCanvas::testAxes(CartesianCanvas& can) {
  return true;
}

bool CartesianCanvas::testZoom(CartesianCanvas& can) {
  return true;
}

bool CartesianCanvas::testRecomputeDimensions(CartesianCanvas& can) {
  return true;
}

bool CartesianCanvas::testDraw(CartesianCanvas& can) {
  return true;
}

//-----------------End Unit testing----------------------------------------------------

#ifdef _WIN32
}
#endif
