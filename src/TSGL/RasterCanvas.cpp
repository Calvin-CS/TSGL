/*
* RasterCanvas.cpp implements the methods from RasterCanvas.h
*/

#include "RasterCanvas.h"

namespace tsgl {

//TODO: drawPoint makes a point cover multiple pixels, which is relevant for getPoint() and makes points look clunkier/less precise than old version. Decreasing pointSize does not change this. (Also issue in CartesianRasterCanvas)
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
