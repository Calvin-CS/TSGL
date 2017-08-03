/*
 * LineChain.cpp
 *
 * Usage: ./LineChain <width> <height> <numThreads>
 */

#include <tsgl.h>
#include <queue>

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
 *     .
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param t The number of threads to use in the function.
 */
void lineChainFunction(Canvas& can, int t) {
  const int IPF = 3;
  const int CWW = can.getWindowWidth() / 2, CWH = can.getWindowHeight() / 2;
  const float ARC = 2.3f, SPIN = 0.01f;

  bool paused = false; // Flag that determines whether the animation is paused
  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&paused]() { // toggle pause when spacebar is pressed
		paused = !paused;
	});

  #pragma omp parallel num_threads(t)
  {
    const float NTHREADS = omp_get_num_threads(); //Number of actual threads
    const float FADERATE = (NTHREADS < 100) ? 1.0f*NTHREADS/100 : 1;
    const int TID = omp_get_thread_num();
    int xOld, yOld, xNew = CWW*2, yNew = CWH, layer = TID;
    float next = (ARC*TID)/NTHREADS, s = next;
    ColorFloat c = Colors::highContrastColor(TID); //High contrast color for this thread

    //Create and add Rectangle for fading lines
    Rectangle* myRect = new Rectangle(0, 0, can.getWindowWidth(), can.getWindowHeight(), ColorFloat(0, 0, 0, FADERATE));
    can.add(myRect);

    std::queue<Polyline*> myLines; //Queue for storing line pointers
    while (can.isOpen()) {  // Checks to see if the window has been closed
      can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
      while(paused && can.isOpen()) {}

      //Make the next line
      next += ARC; s += SPIN;
      xOld = xNew; yOld = yNew;
      float size = cos(s);
      xNew = CWW + CWW*size*cos(next);
      yNew = CWH + CWH*size*sin(next);
      Line* newLine = new Line(xOld, yOld, xNew, yNew, c);
      newLine->setLayer(layer);
      layer++;
      //Add the line to our Queue and Canvas
      can.add(newLine);
      myLines.push(newLine);

      myRect->setLayer(layer-2*t); //Creates illusion of lines fading


      //Delete oldest line
      if(myLines.size() > 60) {
        Polyline* oldLine = myLines.front();
        myLines.pop();
        can.remove(oldLine);
        delete oldLine;
      }
      #pragma omp barrier //Synchronizes all threads
    }

    //After Canvas is closed...
    while(myLines.size() > 0) { //Delete pointers to lines in Queue
      delete myLines.front();
      myLines.pop();
    }
    delete myRect; //Delete thread's Rectangle
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
    std::cout << "Done!" << std::endl;
}
