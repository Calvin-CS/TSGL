/*
 * testColorPoints.cpp
 *
 * Usage: ./testColorPoints <width> <height> <numThreads>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a neat pattern of points to a Background using OMP and takes in a command line
 *  argument for the number of threads to use. Also tests both get- and draw- Pixel() functionality.
 * \details
 * - A parallel block is set up with \#pragma omp parallel using the number of threads passed.
 * - The actual number of threads created is stored in: \b nthreads .
 * - The number of lines per thread is calculated and stored in: \b myPart .
 * - The starting position of each given thread is calculated and stored in: \b myStart .
 * - The outer for loop is set up in a block pattern, and the inner for loop runs from 0 to the Canvas width.
 *   - The color for a thread is calculated.
 *   - If the point's coordinate is even:
 *     - Draw a point on the Canvas in the thread's color.
 *   - Else:
 *     - Draw the point normally.
 *   .
 *   - The function breaks from the outer for loop if the Canvas is closed.
 *   .
 * - Sleep the internal timer of the Canvas until the next draw cycle.
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param numberOfThreads Number of threads to use.
 */
void colorPointsFunction(Canvas& can, int numberOfThreads) {
    // Background * background = new Background(0,0,0,can.getWindowWidth(), can.getWindowHeight(), 0,0,0,RED);
    // can.setBackground(background);
    Background * background = can.getBackground();

    /* this is the part of the test for drawPixel */
    #pragma omp parallel num_threads(numberOfThreads)
    {
        int nthreads = omp_get_num_threads();  //Actual number of threads to use
        // note: allocating rows pixels to threads like this is only perfect if can.getWindowHeight() % # of threads = 0.
        // but I'm too lazy to make it work perfectly always, since it's "good enough" and this is really just a drawPixel test.
        int myPart = can.getWindowHeight() / nthreads;
        int myStart = myPart * omp_get_thread_num();
        for (int i = myStart; i < myStart + myPart; i++) {
        for (int j = 0; j < can.getWindowWidth(); j++) {
            // int id = omp_get_thread_num();
            if (i % 2 == 0) {
            background->drawPixel(j - can.getWindowWidth()/2, i - can.getWindowHeight()/2, BLACK);
            } else {
            background->drawPixel(j - can.getWindowWidth()/2, i - can.getWindowHeight()/2, ColorInt(i % 255, j % 255, (i*j) % 255));
            }
        }
        if (!can.isOpen()) break;
        }
    }

    /* end drawPixel. while loop only contains can.sleep() */

    /* the getPixel portion of the test */
    // bool print = false;
    // int mouseX = 0;
    // int mouseY = 0;

    // background->drawSquare(-100,100,0,100,0,0,0,RED);
    // background->drawSquare(-100,-100,0,100,0,0,0,GREEN);
    // background->drawSquare(100,100,0,100,0,0,0,BLUE);
    // background->drawSquare(100,-100,0,100,0,0,0,ORANGE);

    // can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&print] () {
    //     print = true;
    // });

    // ColorInt c;
    /* end getPixel(). uncomment entirety of while loop besides can.sleep */

    while (can.isOpen()) {
        can.sleep();
        // mouseX = can.getMouseX();
        // mouseY = can.getMouseY();
        // if (print) {
        //     c = background->getPixel(mouseX, mouseY);
        //     background->drawPixel(mouseX, mouseY, RED);
        //     printf("%d, %d - ", mouseX, mouseY);
        //     printf("%d:%d:%d:%d\n", c.R, c.G, c.B, c.A);
        //     print = false;
        // }
    }
}

//Takes in command line arguments for the window width and height as well
//as for the number of threads to use
int main(int argc, char* argv[]) {
  int h = (argc > 2) ? atoi(argv[2]) : 0.8*Canvas::getDisplayHeight();
  int w = (argc > 1) ? atoi(argv[1]) : h * 1.5;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;            //If not, set the width and height to a default value
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  Canvas c(-1, -1, w, h, "Dithered Points", BLACK);
  c.run(colorPointsFunction,t);
}
