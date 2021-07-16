/*
 * testGetPixels.cpp
 *
 * Usage: ./testGetPixels <numThreads>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Grabs the pixels from an image on the Canvas and plays with them.
 * \details
 * - Set a predetermined number of threads and store it in: \b THREADS.
 * - Store the Canvas' dimensions for easy use.
 * - Draw an image on the initially blank Canvas, stretched to fill it.
 * - Set up a parallel OMP block with \b THREADS threads.
 * - Set up the internal timer of the Canvas to expire every 1/100th of a second.
 * - Determine a block size for each thread based on the Canvas' height and the number
 *   of spawned threads.
 * - Determine a starting row for each thread based on \b blocksize and the thread's id.
 * - While the Canvas is open:
 *   - For each row:
 *     - For each column:
 *       - Over each old pixel, draw a new pixel with each of the RGB components incremented
 *         and wrapped.
 *       .
 *     .
 *   - Sleep until the Canvas is ready to draw again.
 *   - Print the time between sleeps of the Canvas' internal timer.
 *   .
 * .
 *
 * \param can Reference to the Canvas being drawn to.
 * \param threads Number of threads to use.
 */
void getPixelsFunction(Canvas& can, int threads) {
  Background * bg = can.getBackground();
  int width = can.getWindowWidth(), height = can.getWindowHeight();
  bg->drawImage(0,0,0,"pics/test.png",width, height, 0,0,0);
  can.sleepFor(0.5f);
  #pragma omp parallel num_threads(threads)
  {
    int blocksize = (double)height / omp_get_num_threads();
    int row = blocksize * omp_get_thread_num() - can.getWindowHeight()/2;
    while (can.isOpen()) {
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
      for (int y = row; y < row + blocksize; y++) {
        for (int x = -(width/2); x < width/2; x++) {
          ColorInt c = bg->getPixel(x,y);
          bg->drawPixel(x, y, ColorInt((1+c.R) % NUM_COLORS, (1+c.G) % NUM_COLORS, (1+c.B) % NUM_COLORS));
        }
      }
    }
  }
}

int main(int argc, char* argv[]) {
  int t = (argc > 1) ? atoi(argv[1]) : omp_get_num_procs();
  Canvas c(-1, -1, 800, 600, "Pixel Shifter", .01);
  c.run(getPixelsFunction,t);
}
