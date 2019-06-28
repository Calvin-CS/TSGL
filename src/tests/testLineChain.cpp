/*
 * testLineChain.cpp
 *
 * Usage: ./testLineChain <width> <height> <numThreads>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a spirograph on a given Canvas using the specified number of threads.
 * \details
 * - The number of iterations, center of the spirograph, distance between threads, and spin are stored in constants.
 * - A parallel block is made and the process is forked using the passed number of threads: \b t.
 * - The actual number of threads spawned is stored in the constant: \b NTHREADS.
 * - The rate at which the spirograph fades is calculated and stored in: \b FADERATE.
 * - The current thread's id is stored in: \b TID.
 * - \b xOld and \b yOld represent the old x and y-coordinates of the line,
 *   \b xNew and \b yNew represent the new ones.
 * - \b xNew is set to the width of the Canvas, \b yNew is set to the middle of the canvas.
 * - Set a variable, \b next, which will be used to determine the next \b xNew and \b yNew and set \b s to \b next.
 * - Get a color based off of the thread's id number.
 * - The internal timer of the Canvas is set up to go off every \b FRAME seconds ( \b FRAME == 1 / \b FPS ).
 * - While the canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn.
 *   - For 0 to the number of iterations per frame:
 *      - \b next is incremented by the spacing in between the threads, and \b s is incremented by the spin factor.
 *      - \b xOld and \b yOld are set to \b xNew and \b yNew.
 *      - The size of the line is determined by \b s.
 *      - \b xNew and \b yNew are calculated and set.
 *      - The line is drawn to the Canvas.
 *      .
 *   - If we are working with the main thread, draw a rectangle that is the size of the Canvas and has an alpha transparency
 *     of: \b FADERATE.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param t The number of threads to use in the function.
 */
void lineChainFunction(Canvas& can, int t) {
  const int IPF = 3;
  const int CWW = can.getWindowWidth() / 2, CWH = can.getWindowHeight() / 2;
  const float ARC = 2.3f, SPIN = 0.01f;
  #pragma omp parallel num_threads(t)
  {
    const float NTHREADS = omp_get_num_threads();
    const float FADERATE = (NTHREADS < 200) ? 1.0f*NTHREADS/200 : 1;
    const int TID = omp_get_thread_num();
    int xOld, yOld, xNew = CWW*2, yNew = CWH;
    float next = (ARC*TID)/NTHREADS, s = next;
    ColorFloat c = Colors::highContrastColor(TID);
    while (can.isOpen()) {  // Checks to see if the window has been closed
      can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
      for (int i = 0; i < IPF; ++i) {
        next += ARC; s += SPIN;
        xOld = xNew; yOld = yNew;
        float size = cos(s);
        xNew = CWW + CWW*size*cos(next);
        yNew = CWH + CWH*size*sin(next);
        can.drawLine(xOld, yOld, xNew, yNew, c);
      }
      if (TID == 0)
        can.drawRectangle(0,0,CWW*2,CWH*2,ColorFloat(0,0,0,FADERATE));
      #pragma omp barrier
    }
  }
}

//Takes command line arguments for the window width and height as well as for the number of threads
//to use in the function
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 900;                  //If not, set the width and height to a default value
    unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
    if (t == 0)
      t = omp_get_num_procs();
    Canvas c(-1, -1, w, h, "Spirograph");
    c.setBackgroundColor(BLACK);
    c.run(lineChainFunction,t);
}
