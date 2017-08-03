/*
 * testProgressBar.cpp
 *
 * Usage: ./testProgressBar <width> <height>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a diagonal black-to-white gradient using OMP and takes in a command line argument for the
 *  number of threads to use.
 * \details
 * - Compute and store the necessary information in order to draw the ProgressBar object
 *   (x-coordinate, y-coordinate, width, height, minimum, maximum, and the number of segments).
 * - Create the ProgressBar object with the information calculated in the previous step.
 * - Set the progress of drawing the ProgressBar object and store it in: \b progress.
 * - While the Canvas is open:
 *   - Sleep the Canvas' internal timer until the next draw cycle.
 *   - Increment the progress so far.
 *   - For 0 to the number of segments:
 *     - Update the ProgressBar object.
 *     .
 *   - Draw the ProgressBar onto the Canvas.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void progressBarFunction(Canvas& can) {
    const int X = 100, Y = X, W = can.getWindowWidth()-X*2, H = 20, MIN = 0, MAX = 1000, SEGS = 8;
    can.setBackgroundColor(WHITE);
    ProgressBar pb(X,Y,W,H,MIN,MAX,SEGS);
    int progress = 0;
    can.add(&pb); //TODO: make the ProgressBar work with the new way of doing things.
    while (can.isOpen()) {  // Checks to see if the window has been closed
        can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
        ++progress;
        for (unsigned i = 0; i < SEGS; ++i)
          pb.update(progress+i*(MAX/SEGS),i);
    }
    can.wait();
}

//Takes in the window width and height as command line arguments for the Canvas
//as well as for the number of threads to use
//( see http://www.cplusplus.com/articles/DEN36Up4/ )
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Progress Bar Example");   //Create an explicit Canvas based off of the passed width and height (or the defaults if the width and height were invalid)
    c.setBackgroundColor(WHITE);
    c.run(progressBarFunction);
}
