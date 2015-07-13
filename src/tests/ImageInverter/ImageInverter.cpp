#include "ImageInverter.h"

using namespace tsgl;

ImageInverter::ImageInverter(const std::string& fileName, unsigned width, unsigned height)
 : myCanvas1(0, 0, width, height, fileName),
   myCanvas2(-1, -1, width, height, fileName),
   myWidth(width), myHeight(height), myFileName(fileName)
{
  myCanvas1.start();
  myCanvas1.drawImage(fileName, 0, 0, width, height);
  sleep(1);
  myCanvas2.start();
}

void ImageInverter::run(unsigned numThreads) {
  // sleep(1);
  invertImage(numThreads);
  stop();
}

void ImageInverter::invertImage(unsigned numThreads) {
  int inverse = 0;
  ColorInt pixelColor;
  #pragma omp parallel for num_threads(numThreads)
  for (int row = 0; row < myHeight; row++) {
    if (! (myCanvas1.getIsOpen() && myCanvas2.getIsOpen()) ) {
      #ifndef _WIN32
      row = myHeight; //Exit out (not allowed on Windows)
	  #endif
      continue;
    }
    for (unsigned col = 0; col < myWidth; col++) {
      pixelColor = myCanvas1.getPixel(row, col);
      inverse = (pixelColor.R + pixelColor.G + pixelColor.B) / 3;
      myCanvas2.drawPixel(row, col, ColorInt(inverse,inverse,inverse) );
    }
	myCanvas1.sleepFor(FRAME);
  }
}

void ImageInverter::stop() {
  myCanvas1.wait();
  myCanvas2.wait();
}


ImageInverter::~ImageInverter() {
//  delete myCanvas1;
//  delete myCanvas2;
//  std::cout << "ImageInverter terminated normally.\n" << std::endl;
}
