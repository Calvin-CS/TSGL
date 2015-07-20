/*
 * testHighData.cpp
 *
 * Usage: ./testHighData <width> <height> <numThreads>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a large number of pixels on a Canvas at a high framerate.
 * \details Very basic stress test for the Canvas' drawPoint() function.
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
 * - Set Local variables to track the internal timer's repetitions, and the Canvas' dimensions.
 * - While the Canvas is open:
 *   - Set \b reps to the timer's current number of repetitions.
 *   - Compute the blue component of the current color based on reps.
 *   - Attempt to draw every pixel with the current 1.0,1.0,blue.
 *   - Sleep the timer until the Canvas is ready to draw again.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void highData(Canvas& can, unsigned threads) {
  const float HVAL = 6.0f/255.0f;  // For converting integer hues to floating point values
  const unsigned int width = can.getWindowWidth(), height = can.getWindowHeight();
  #pragma omp parallel num_threads(threads)
  {
    float tid = omp_get_thread_num(), nthreads = omp_get_num_threads();
    int offset = (MAX_COLOR*tid)/nthreads;
    unsigned bstart = tid*(width/nthreads);
    unsigned bend = (tid==nthreads) ? width-1 : bstart + width/nthreads;
    ColorHSV tcol= Colors::highContrastColor(tid);
    while (can.getIsOpen()) {
      tcol.H = HVAL * ((can.getReps() + offset) % MAX_COLOR);
      for (unsigned i = bstart; i <= bend; i++)
        for (unsigned int j = 0; j < height; j++)
          can.drawPoint(i, j, tcol);
      can.handleIO();
    }
  }
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = 1.2*Canvas::getDisplayHeight(), h = 0.75*w; //If not, set the width and height to a default value
    int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
    Canvas c(-1, -1, w, h, "Pixel Drawing Load Test", FRAME);
    c.run(highData,t);
}
