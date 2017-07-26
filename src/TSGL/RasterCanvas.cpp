/*
* RasterCanvas.cpp implements the methods from RasterCanvas.h
*/

#include "RasterCanvas.h"

namespace tsgl {

    ColorInt RasterCanvas::getPixel(int x, int y) {
      char buffer[4] = {0};
      ColorInt retVal;

      glReadBuffer(GL_BACK);
      glReadPixels(	x, y, 1, 1,
        GL_RGBA, GL_BYTE, &buffer);

      // printf("GL ERROR %d\n", glGetError());

      retVal.R = (int)buffer[0];
      retVal.G = (int)buffer[1];
      retVal.B = (int)buffer[2];
      retVal.A = (int)buffer[3];

      // printf("%f", (int)buffer[3]);

      return(retVal);
    }

    void RasterCanvas::drawPoint(float x, float y, ColorInt c, float pointSize){

      drawPoint(x, y, &c, pointSize){

    }

    void RasterCanvas::drawPoint(float x, float y, ColorInt &c, float pointSize){

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
