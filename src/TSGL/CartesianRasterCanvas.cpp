/*
* CartesianRasterCanvas.cpp implements the methods from CartesianRasterCanvas.h
*/

#include "CartesianRasterCanvas.h"

namespace tsgl {

    ColorInt CartesianRasterCanvas::getPixel(int x, int y) {
      int actualX, actualY;
      getScreenCoordinates(x,y,actualX,actualY);
      return RasterCanvas::getPixel(actualX,actualY);
    }

    void CartesianRasterCanvas::drawPoint(Decimal x, Decimal y, ColorInt c, float pointSize){
      int actualX, actualY;
      getScreenCoordinates(x,y,actualX,actualY);
      RasterCanvas::drawPoint(actualX,actualY,c,pointSize);
    }

}
