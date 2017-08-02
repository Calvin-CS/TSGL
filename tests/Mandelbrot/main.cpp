/*
 * testMandelbrot.cpp
 *
 * Usage: ./testMandelbrot <width> <height> <numThreads> <maxIterations>
 */

/*testMandelbrot.cpp contains multiple functions that display a Mandelbrot set in similar fashions (and one that displays a Julia set). */

#include "Buddhabrot.h"
#include "Mandelbrot.h"
#include "GradientMandelbrot.h"
#include "ThreadMandelbrot.h"
#include "Julia.h"
#include "GeneralizedMandelbrot.h"
#include "Nova.h"

using namespace tsgl;

/*!
 * \brief Draws the Mandelbrot set on a CartesianCanvas with custom controls, a specified target update rate,
 *  and a dynamic number of threads and uses command-line arguments to specify the number of threads to use.
 *  There is also a ProgressBar that shows the progress made by each thread as the Mandelbrot set is drawn onto the CartesianCanvas.
 * \details
 * - A class containing all of the data and methods to draw a Mandelbrot set has been made.
 * - When you create a Mandelbrot object:
 *    - The number of threads to use is predetermined (passed as the \b threads parameter) and stored in: \b myThreads.
 *    - The number of iterations to check is predetermined (passed as the \b depth parameter) and stored in: \b myDepth.
 *    - The internal timer of the Canvas is set up to go off every ( \b FRAME / 2 ) seconds.
 *    - A flag telling us to redraw is set to true.
 *    .
 * - When you bind the buttons:
 *    - The spacebar on-press event is set to tell the Canvas to clear and re-render.
 *    - The left mouse on-press event is set to grab the mouse's current coordinates.
 *    - The left mouse on-release event is set to grab the mouse's current coordinates, and tell the Canvas to zoom into the
 *      bounding rectangle between the current coordinates and those from the left mouse's on press event and to redraw the Mandelbrot
 *      at that point.
 *    - The right mouse on-press event is set to grab the mouse's current coordinates, and tell the Canvas to zoom out
 *      from that area and redraw the Mandlebrot at that point.
 *    - The mouse's scroll wheel is set to tell the Canvas to zoom in / out by a predetermined amount at the mouse's
 *      current coordinates and redraw the Mandelbrot at that point.
 *    .
 * - When you actually draw the Mandelbrot object onto the CartesianCanvas:
 *   - Store the height of the Canvas.
 *   - While the redraw flag is set:
 *      - Set the redraw flag to false.
 *      - Reset the internal timer to 0.
 *      - Fork off the predetermined number of parallel threads using OMP.
 *      - Store the actual number of threads spawned in: \b nthreads.
 *      - Assign a color to each thread.
 *      - Figure the cartesian size of the area each thread is to calculate and store it in: \b blocksize.
 *      - Figure out the actual number of rows each thread is to calculate and store it in: \b blockheight.
 *      - For 0 to \b blockheight and as long as we aren't trying to render off of the screen:
 *        - Make an inner loop which determines whether to color the pixels black or a different color based off of whether they've escaped or not.
 *          - (Basic Mandelbrot calculations; see http://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings ).
 *          - Break if the Canvas is to redraw.
 *        .
 *        - Handle any IO events (OS X version only).
 *        - Break if the Canvas is to redraw.
 *      .
 *      - Output the time it took to compute the screen.
 *      - While the Canvas has not been closed and it isn't time to redraw yet:
 *        - Sleep the thread for one frame until the Canvas is closed by the user or told to redraw.
 *      .
 *   .
 * .
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param threads Reference to the number of threads passed via command-line arguments.
 * \param depth The number of iterations to go to in order to draw the Mandelbrot set.
 */
void mandelbrotFunction(CartesianRasterCanvas& can, unsigned threads, unsigned depth) {
  Mandelbrot m(threads,depth);  //Make the object
  m.bindings(can);              //Bind the buttons
  m.draw(can);                  //Draw the Mandelbrot object onto the Canvas
}

/*!
 * \brief Draws a Gradient Mandelbrot set on a CartesianCanvas.
 * \details Same as mandelbrotFunction(), but with smoother shading.
 * ( see http://linas.org/art-gallery/escape/smooth.html ).
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param threads Reference to the number of threads to use.
 * \param depth The number of iterations to go to in order to draw the Gradient Mandelbrot set.
 * \see mandelbrotFunction(), GradientMandelbrot class.
 */
void gradientMandelbrotFunction(CartesianRasterCanvas& can, unsigned threads, unsigned depth) {
  GradientMandelbrot m(threads,depth);  //Create the GradientMandelbrot
  m.bindings(can);                      //Bind the buttons
  m.draw(can);                          //Draw it
}

/*!
 * \brief Draws a Threaded Mandelbrot set on a CartesianCanvas.
 * \details Same as mandelbrotFunction(), but with colors based on the threads.
 * ( see http://linas.org/art-gallery/escape/smooth.html ).
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param threads Reference to the number of threads to use.
 * \param depth The number of iterations to go to in order to draw the Gradient Mandelbrot set.
 * \see mandelbrotFunction(), GradientMandelbrot class.
 */
void threadMandelbrotFunction(CartesianRasterCanvas& can, unsigned threads, unsigned depth) {
  ThreadMandelbrot m(threads,depth);  //Create the ThreadMandelbrot
  m.bindings(can);                      //Bind the buttons
  m.draw(can);                          //Draw it
}

/*!
 * \brief Draws a Buddhabrot set on a CartesianCanvas.
 * \details Same as mandelbrotFunction(), but with a different shading algorithm and no
 *  ProgressBar.
 * ( see https://en.wikipedia.org/wiki/Buddhabrot ).
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param numberOfThreads Reference to the number of threads to use.
 * \param depth The number of iterations to go to in order to draw the Buddhabrot set.
 */
void buddhabrotFunction(RasterCanvas& can, unsigned threads, unsigned depth) {
  Buddhabrot b(threads, depth);  //Create the Buddhabrot object
  b.draw(can);                   //Draw it
}

/*!
 * \brief Draws a Julia set.
 * \details It is drawn in this way:
 * - It is drawn in a similar way as the Mandelbrots are.
 * - A class contains all of the necessary information to draw the Julia set.
 * - When the Julia class object is made:
 *    - Set class instance variables for keeping track of the number of threads to use,
 *      the number of iterations, point coordinates for zooming in and out, and a redraw flag
 *      that determines when we should redraw the Julia object.
 *    .
 * - When you bind buttons to the CartesianCanvas:
 *    - Bind the spacebar to clear the screen and redraw the Julia object.
 *    - Bind the left mouse button to zoom in and redraw the Julia object at the
 *      focused point.
 *    - Bind the right mouse button to zoom out and redraw the Julia object.
 *    - Bind the mouse wheel to zoom in and out when scrolled up and down (respectively)
 *      and redraw the Julia object.
 *    .
 * - When you actually draw the Julia object onto the CartesianCanvas:
 *    - Store the window width of the Canvas.
 *    - Create a VisualTaskQueue object that displays how much work each thread did in drawing
 *      the Julia object.
 *    - While its time to redraw:
 *        - Set the redraw flag to false.
 *        - Reset the Canvas' internal timer to 0.
 *        - Reset the VisualTaskQueue object.
 *        - A parallel block is made using: \b myThreads threads.
 *            - Store the thread id number and the number of threads.
 *            - Store the thread color.
 *            - An infinite loop is made where the necessary calculations needed to draw the Julia
 *              object are made and the Julia object is drawn. Once the Julia object has been drawn, the
 *              loop is broken out of. Also, if the Canvas is not open or the redraw flag is set, the loop
 *              is also broken out of. Also, if one of the threads tries to render off of the screen, the loop
 *              is broken out of. The VisualTaskQueue is also updated in this loop, and if you are a Mac user,
 *              I/O events are handled here as well.
 *              .
 *           .
 *       - Output the time it took for each iteration.
 *       - While the Canvas is open and the redraw flag is *NOT* set:
 *          - Sleep the internal timer until the next draw cycle.
 *          .
 *       .
 *    - Close the VisualTaskQueue.
 *  .
 * \param can Reference to the CartesianCanvas to draw to.
 * \param threads Reference to the number of threads to use when drawing the Julia object.
 * \param depth The number of iterations to go to in order to draw the Julia object.
 */
void juliaFunction(CartesianRasterCanvas& can, unsigned threads, unsigned depth) {
  Julia j(threads,depth);  //Create the Julia object
  j.bindings(can);         //Bind the buttons
  j.draw(can);             //Draw it
}

/*!
 * \brief Draws a generalized Mandelbrot set on a CartesianCanvas.
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param threads Reference to the number of threads to use.
 * \param depth The number of iterations to go to in order to draw the Generalized Mandelbrot set.
 */
void generalizedFunction(CartesianRasterCanvas& can, unsigned threads, unsigned depth) {
    GeneralizedMandelbrot m(threads,depth);  //Create the Generalized object
    m.bindings(can);        //Bind the buttons
    m.draw(can);            //Draw it
}

/*!
 * \brief Draws a Nova fractal (Newton fractal) on a CartesianCanvas.
 * \param can Reference to the CartesianCanvas being drawn to.
 * \param threads Reference to the number of threads to use.
 * \param depth The number of iterations to go to in order to draw the Mandelbrot set.
 * \see mandelbrotFunction(), Nova class.
 */
void novaFunction(CartesianRasterCanvas& can, unsigned threads, unsigned depth) {
    Nova n(threads,depth);  //Create the Nova object
    n.bindings(can);        //Bind the buttons
    n.draw(can);            //Draw it
}

//Takes command line arguments for the width and height of the screen
//as well as the number of threads to use and the number of iterations to draw the Mandelbrot set
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
  int w2 = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();  //Julia
  int h2 = (argc > 2) ? atoi(argv[2]) : 0.75*w;  //Julia
  int x = Canvas::getDisplayWidth() - w - 64;
  if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
    w = 1200;
    w2 = 1.2 * Canvas::getDisplayHeight();
    h = 900;                  //If not, set the width and height to a default value
    h2 = 0.75 * w2;
  }
  unsigned t = (argc > 3) ? atoi(argv[3]) : 5;    //Get the number of threads to use
  unsigned d = (argc > 4) ? atoi(argv[4]) : MAX_COLOR; //Normal Mandelbrot
  unsigned d2 = (argc > 4) ? atoi(argv[4]) : MAX_COLOR;  //Gradient Mandelbrot
  unsigned d3 = (argc > 4) ? atoi(argv[4]) : MAX_COLOR; //Buddhabrot & Julia
  unsigned d4 = (argc > 4) ? atoi(argv[4]) : 32; //Nova

  //Normal Mandelbrot
  std::cout << "Normal Mandelbrot" << std::endl;
  CartesianRasterCanvas c1(-1, -1, w, h, -2, -1, 1, 1, "Mandelbrot", FRAME / 2);
  c1.run(mandelbrotFunction,t,d);

  //Gradient Mandelbrot
  std::cout << "Gradient Mandelbrot" << std::endl;
  CartesianRasterCanvas c2(-1, -1, w, h, -2, -1.125, 1, 1.125, "Gradient Mandelbrot", FRAME / 2);
  c2.run(gradientMandelbrotFunction,t,d2);

  //Thread Mandelbrot
  std::cout << "Thread Mandelbrot" << std::endl;
  CartesianRasterCanvas c6(-1, -1, w, h, -2, -1.125, 1, 1.125, "Thread Mandelbrot", FRAME / 2);
  c6.run(threadMandelbrotFunction,t,d2);

  //Buddhabrot
  std::cout << "Buddhabrot" << std::endl;
  RasterCanvas c3(-1, -1, w, h,"Buddhabrot", FRAME / 2);
  c3.setBackgroundColor(BLACK);
  c3.run(buddhabrotFunction,t,d3);

  //Julia
  std::cout << "Julia set" << std::endl;
  CartesianRasterCanvas c4(x, -1, h2, h2, -2, -2, 2, 2, "Julia Set", FRAME / 2);
  c4.run(juliaFunction,t,d3);

  //Generalized Mandelbrot
  std::cout << "Generalized Mandelbrot set" << std::endl;
  CartesianRasterCanvas c7(x, -1, h2, h2, -2, -2, 2, 2, "Generalized Mandelbrot Set", FRAME / 2);
  c7.run(generalizedFunction,t,d2);

  // Nova
  std::cout << "Nova" << std::endl;
  CartesianRasterCanvas c5(x, -1, w, h, -1.0, -0.5, 0, 0.5, "Nova (Newton Fractal)", FRAME / 2);
  c5.zoom(-0.361883,-0.217078,0.1f);
  c5.run(novaFunction,t,d4);
}
