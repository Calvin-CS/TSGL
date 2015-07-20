/*
 * testGraydient.cpp
 *
 * Usage: ./testGraydient <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a diagonal black-to-white gradient using OMP and takes in a command-line argument for the
 * number of threads to use.
 * \details
 * - A parallel block is set up with #pragma omp parallel using the number of threads passed as a command-line argument.
 * - The outer for loop is set up in a striping pattern, and the inner for loop runs from 0 to the Canvas height.
 *   - If the Canvas is not open anymore, break out of the function.
 *   - The color ( \b color ) is set to a shade of gray based on its distance from the top left of the canvas.
 *   - The point is drawn to the Canvas.
 *   - Sleep the internal timer of the Canvas until the next draw cycle.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use in the function.
 */
void graydientFunction(Canvas& can, int threads) {
  #pragma omp parallel num_threads(threads)
  {
    for (int i = omp_get_thread_num(); i < can.getWindowWidth(); i += omp_get_num_threads()) {
      if (!can.getIsOpen()) break;
      for (int j = 0; j < can.getWindowHeight(); j++) {
        int color = i * MAX_COLOR / 2 / can.getWindowWidth() + j * MAX_COLOR / 2 / can.getWindowHeight();
        can.drawPoint(i, j, ColorInt(color, color, color));
      }
      can.sleep();
    }
  }
}

//Takes in the window width and height as command-line arguments for the Canvas
//as well as for the number of threads to use
//( see http://www.cplusplus.com/articles/DEN36Up4/ )
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     // Checked the passed width and height if they are valid
      w = h = 960;            // If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Black-white Gradient");
    int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
    c.run(graydientFunction,t);
}
