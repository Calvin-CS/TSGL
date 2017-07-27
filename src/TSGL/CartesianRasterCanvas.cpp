/*
* CartesianRasterCanvas.cpp implements the methods from CartesianRasterCanvas.h
*/

#include "CartesianRasterCanvas.h"

namespace tsgl {

void CartesianRasterCanvas::drawPoint(float x, float y, ColorInt c, float pointSize){
  int actualX, actualY;
  getScreenCoordinates(x,y,actualX,actualY);

  rasterPointStruct point;
  point.x = actualX;
  point.y = actualY;
  point.size = pointSize;
  point.R = c.R;
  point.G = c.G;
  point.B = c.B;
  point.A = c.A;

  rasterPointMutex.lock();
    rasPointVec.push_back(point);
  rasterPointMutex.unlock();

}

}
