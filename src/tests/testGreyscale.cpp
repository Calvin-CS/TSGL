/*
 * testGreyscale.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Grabs the pixels from an image on the Canvas and converts them to grayscale.
 * \details
 * - Predetermine the number of threads and line thickness and store them in variables.
 * - Check if the passed parameter for the number of threads is valid:
 *    - If its a negative value, change the sign to be positive.
 *    - If its greater than 30, assign 30 to the number of threads to use.
 *    - Else, assign the passed parameter to the number of threads to use.
 * - Set up the internal timer of the Canvas to expire every ( \b FRAME * 2 ) seconds.
 * - Store the Canvas' dimensions for ease of use.
 * - Stretch a fancy image over the Canvas.
 * - Tell the internal timer to manually sleep for a quarter of a second (to assure the draw buffer is filled).
 * - Initialize a pointer to the Canvas' screen buffer.
 * - Set up a parallel OMP block with \b threads threads.
 * - Get the actual number of spawned threads and store it in: \b nthreads.
 * - Compute the \b blocksize based on the Canvas height and \b nthreads.
 * - Compute the current thread's row based on \b blocksize and the thread's id.
 * - Generate a nice color based on the thread's id.
 * - Set a grayscale color variable to 0.
 * - For each row:
 *   - Set a buffer index variable based on the row and column ( times 3 colors ).
 *   - For each column:
 *     - Set the gray color variable to the average of the RGB components.
 *     - Draw the grayed point over the old point, and increment the index by one pixel.
 *     .
 *   - Break if the Canvas was closed.
 *   - Sleep until the Canvas is ready to render again.
 *   .
 * - Once a thread is finished grayscaling, draw a box around its rendered area using
 * the predetermined high contrast color.
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use.
 */
void greyScaleFunction(Canvas& can, int & numberOfThreads) {
  int threads = 0;
  if (numberOfThreads < 0) {
    numberOfThreads *= -1;
    threads = numberOfThreads;
  } else if(numberOfThreads > 30) {
    threads = 30;
  } else {
    threads = numberOfThreads;
  }
  const unsigned int thickness = 3;
  const unsigned ww = can.getWindowWidth(),
                 wh = can.getWindowHeight();
  can.drawImage("../assets/pics/colorful_cars.jpg", 0, 0, ww, wh);
  Timer::threadSleepFor(.25);
  uint8_t* buffer = can.getScreenBuffer();

#pragma omp parallel num_threads(threads)
  {
    int nthreads = omp_get_num_threads();
    unsigned int blocksize = wh / nthreads;
    unsigned int row = blocksize * omp_get_thread_num();
    ColorFloat color = Colors::highContrastColor(omp_get_thread_num());
    int gray = 0;

    for (unsigned int y = row; y < row + blocksize; y++) {
      int index = y * ww * 3;
      for (unsigned int x = 0; x < ww; x++) {
        gray = (buffer[index] + buffer[index + 1] + buffer[index + 2]) / 3;
        can.drawPoint(x, wh - y, ColorInt(gray, gray, gray));
        index += 3;
      }
      if (! can.getIsOpen()) break;
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
    }
    for (unsigned int i = 0; i < thickness; i++) {
      can.drawRectangle(i, row + i, ww - 1 - i, row + blocksize - i, color, false);
    }
  }
}

//Takes command line arguments for the width and height of the window
//as well as for the number of threads to use
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;              //If not, set the width and height to a default value
  Canvas c31(-1, -1, w, h, "Image Greyscaling", FRAME * 2);
  int numberOfThreads = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Number of threads
  c31.setBackgroundColor(GREY);
  c31.start();
  greyScaleFunction(c31, numberOfThreads);  //Pass it as an argument
  c31.wait();
}
