/*
 * ImageInverter.cpp
 */

#include "ImageInverter.h"

ImageInverter::ImageInverter(const std::string& fileName, unsigned width, unsigned height)
 : myCanvas1(0, 0, width, height, fileName),
   myCanvas2(-1, -1, width, height, fileName),
   myWidth(width), myHeight(height), myFileName(fileName)
{
  myCanvas1.start();
  myCanvas1.drawImage(fileName, 0, 0, width, height);
  // myCanvas1.drawRectangle(1,1,width-2,height-2,BLACK,false);
  sleep(1);
  myCanvas2.start();
}

void ImageInverter::run(unsigned numThreads) {
  invertImage(numThreads);
  stop();
}

void ImageInverter::invertImage(unsigned numThreads) {
  ColorInt pixelColor;
  #pragma omp parallel for num_threads(numThreads)
  for (int row = 0; row < myHeight; row++) {
    myCanvas1.sleep();
/*
    if (! (myCanvas1.isOpen() && myCanvas2.isOpen()) ) {
      #ifndef _WIN32
      row = myHeight; //Exit out (not allowed on Windows)
      #endif
      continue;
    }
*/
    for (int col = 0; col < myWidth; col++) {
      pixelColor = myCanvas1.getPixel(row, col);
      int invertedR = 255 - pixelColor.R;
      int invertedG = 255 - pixelColor.G;
      int invertedB = 255 - pixelColor.B;
      myCanvas2.drawPixel(row, col, ColorInt(invertedR,invertedG,invertedB) );
    }
  }
}

void ImageInverter::stop() {
  myCanvas2.wait();
  myCanvas1.wait();
}

ImageInverter::~ImageInverter() {
//  delete myCanvas1;
//  delete myCanvas2;
//  std::cout << "ImageInverter terminated normally.\n" << std::endl;
}
