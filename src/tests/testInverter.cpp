/*
 * testInverter.cpp
 *
 * Usage: ./testInverter <numThreads>
 */

#include <tsgl.h>
#include "ImageInverter/ImageInverter.h"

using namespace tsgl;

int main(int argc, char* argv[]) {
  int numThreads = (argc > 1) ? atoi(argv[1]) : omp_get_num_procs();
  ImageInverter ii("../assets/pics/Car-colors.jpg", 1022, 1024);
  ii.run(numThreads);
//   std::string fileName = "./assets/pics/Car-colors.jpg";
//   int width = 1022;
//   int height = 1024;
//   Canvas myCanvas1(0, 0, width, height, fileName);
//   Canvas myCanvas2(-1, -1, width, height, fileName);
//   int myWidth = width;
//   int myHeight = height; 
//   std::string myFileName = fileName; 

//   myCanvas1.start();
//   myCanvas1.drawImage(fileName, 0, 0, width, height, 1.0f, 0);
//   sleep(1);
//   myCanvas2.start();

//   ColorInt pixelColor;
//   #pragma omp parallel for num_threads(numThreads)
//   for (int row = 0; row < myHeight; row++) {
//     myCanvas1.sleep();
// /*
//     if (! (myCanvas1.isOpen() && myCanvas2.isOpen()) ) {
//       #ifndef _WIN32
//       row = myHeight; //Exit out (not allowed on Windows)
//       #endif
//       continue;
//     }
// */
//     for (int col = 0; col < myWidth; col++) {
//       pixelColor = myCanvas1.getPixel(row, col);
//       int invertedR = 255 - pixelColor.R;
//       int invertedG = 255 - pixelColor.G;
//       int invertedB = 255 - pixelColor.B;
//       myCanvas2.drawPixel(row, col, ColorInt(invertedR,invertedG,invertedB) );
//     }
//   }

//   myCanvas2.wait();
//   myCanvas1.wait();
}
