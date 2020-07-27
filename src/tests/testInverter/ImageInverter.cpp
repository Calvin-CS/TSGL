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
  // #pragma omp parallel for num_threads(numThreads)
  const unsigned WW = myCanvas1.getWindowWidth(),WH = myCanvas1.getWindowHeight();
  #pragma omp parallel num_threads(numThreads) 
  {
    int nthreads = omp_get_num_threads();
    unsigned int blocksize = WH / nthreads;
    unsigned int row = blocksize * omp_get_thread_num();
    for (unsigned int x = row; x < row + blocksize; x++) {
      for (unsigned int y = 0; y < WW; y++) {
        pixelColor = myCanvas1.getPixel(x, y);
        int invertedR = 255 - pixelColor.R;
        int invertedG = 255 - pixelColor.G;
        int invertedB = 255 - pixelColor.B;
        myCanvas2.drawPixel(x, y, ColorInt(invertedR,invertedG,invertedB) );
      }
      myCanvas1.sleep();
      myCanvas2.sleep();
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
