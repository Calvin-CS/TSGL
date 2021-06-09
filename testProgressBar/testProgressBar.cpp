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
void progressBarFunction(Canvas& can, int numThreads) {
    Background * bg = can.getBackground();
    const int X = 0, Y = X, W = can.getWindowWidth()-200, H = 20, MIN = 0, MAX = 1000, SEGS = numThreads;
    ProgressBar * pb = new ProgressBar(0,0,0,W,H,MIN,MAX,SEGS,0,0,0);
    int progress = 0;
    can.add(pb);
    for (int i = 0; i < SEGS; ++i) {
        float w = pb->getWidth();
        bg->drawText(pb->getCenterX() - w/2 + i*w/SEGS + 10, pb->getCenterY() + pb->getHeight()+8, 0, std::to_wstring(i), "./assets/freefont/FreeSerif.ttf", 36, 0,0,0, BLACK);
    }
    while (can.isOpen()) {  // Checks to see if the window has been closed
        can.sleep();   //Removed the timer and replaced it with an internal timer in the Canvas class
        ++progress;
        for (int i = 0; i < SEGS; ++i)
          pb->update(progress+i*(MAX/SEGS),i);
        // pb->setPitch(can.getFrameNumber());
    }
    delete pb;
}

//Takes in the number of threads to use as a command line argument for the Canvas, defaulting to 8.
//( see http://www.cplusplus.com/articles/DEN36Up4/ )
int main(int argc, char* argv[]) {
    int t = (argc > 1) ? atoi(argv[1]) : 8;
    int w;
    int h;
    if (t <= 0 || 10 < t) //Checked the passed threadNumber if it is valid
      t = 8;              //If not, set numThreads to a default value
    w = (t+2)*100;
    h = 200;
    Canvas c(-1, -1, w, h, "Progress Bar Example");
    c.run(progressBarFunction, t);
}
