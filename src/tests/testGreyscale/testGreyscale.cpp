/*
 * testGreyScale.cpp
 *
 * Usage: ./testGreyScale <width> <height> <numThreads>
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
 *   .
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
 *   - For each column:
 *     - Get the pixel color of a point of the Canvas.
 *     - Set a gray color variable to the average of the RGB components.
 *     - Draw the grayed point over the old point, and increment the index by one pixel.
 *     .
 *   - Break if the Canvas was closed.
 *   - Sleep until the Canvas is ready to render again.
 *   .
 * - Once a thread is finished grayscaling, draw a box around its rendered area using
 *   the predetermined high contrast color.
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use.
 */
void greyScaleFunction(Canvas& can, int numberOfThreads) {
  int threads = numberOfThreads;
  clamp(threads,1,30);
  const unsigned thickness = 3;
  const unsigned WW = can.getWindowWidth(),WH = can.getWindowHeight();
  can.drawImage("../assets/pics/colorful_cars.jpg", 0, 0, WW, WH);
  can.sleepFor(0.25f);
  #pragma omp parallel num_threads(threads)
  {
    int nthreads = omp_get_num_threads();
    unsigned int blocksize = WH / nthreads;
    unsigned int row = blocksize * omp_get_thread_num();
    ColorFloat color = Colors::highContrastColor(omp_get_thread_num());
    for (unsigned int y = row; y < row + blocksize; y++) {
      for (unsigned int x = 0; x < WW; x++) {
		    ColorInt pixelColor = can.getPoint(x, y);
        int gray = (pixelColor.R + pixelColor.G + pixelColor.B) / 3;
        can.drawPoint(x, y, ColorInt(gray, gray, gray));
      }
      if (! can.isOpen()) break;
      can.sleep();
    }
    for (unsigned int i = 0; i < thickness; i++) {
      can.drawRectangle(i, row + i, WW - 1 - i, blocksize - i*2, color, false);
      // can.drawRectangle(column + i, i, column + blocksize - i, WH - 1 - i, color, false);
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
  Canvas c(-1, -1, w, h, "Image Greyscaling");
  int numberOfThreads = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();   //Number of threads
  c.run(greyScaleFunction,numberOfThreads);
}
