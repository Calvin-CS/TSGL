/*
 * testLineChain.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a spirograph on a given Canvas using the specified number of threads.
 * \details
 * - \b xNew and \b yNew are set to the middle of the canvas.
 * - The internal timer of the Canvas is set up to go off every \b FRAME seconds ( \b FRAME == 1 / \b FPS ).
 * - While the canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn.
 *   - \b xOld and \b yOld are set to \b xNew and \b yNew, while \b xNew and \b yNew are set to random positions.
 *   - A random color is chosen.
 *   - The line is drawn to the Canvas.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
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
    while (can.getIsOpen()) {  // Checks to see if the window has been closed
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

//Takes command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 900;                  //If not, set the width and height to a default value
    unsigned t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();    //Get the number of threads to use
    if (t == 0)
      t = omp_get_num_procs();
    Canvas c2(-1, -1, w, h, "Spirograph", FRAME);
    c2.setBackgroundColor(BLACK);
    c2.start();
    lineChainFunction(c2,t);
    c2.wait();
}
