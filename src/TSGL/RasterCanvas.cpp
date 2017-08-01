/*
* RasterCanvas.cpp implements the methods from RasterCanvas.h
*/

#include "RasterCanvas.h"

namespace tsgl {

    void RasterCanvas::drawPoint(float x, float y, ColorInt c, float pointSize){

      // printf("RasterCanvas drawPoint\n");

      rasterPointStruct point;
      point.x = x;
      point.y = y;
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
