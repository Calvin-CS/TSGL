/*
 * tests.cpp provides example usage for the TSGL library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 */

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#ifdef _WIN32
const double PI = 3.1415926535;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;  // One radian in degrees

// Some constants that get used a lot
const int NUM_COLORS = 256, MAX_COLOR = 255;
// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}

/*!
 * \brief Draws a diagonal black-to-white gradient using OMP
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in \b nthreads
 * - The outer for loop is set up in a striping pattern, and the inner for loop runs from 0 to the Canvas height
 * - The color is set to a shade of gray based on its distance from the top left of the canvas
 * - The point is drawn to the Canvas
 * .
 * \param can Reference to the Canvas being drawn to
 */
void graydientFunction(Canvas& can) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int nthreads = omp_get_num_threads();
        int color;
        for (int i = omp_get_thread_num(); i < can.getWindowWidth(); i += nthreads) {
            for (int j = 0; j < can.getWindowHeight(); j++) {
                color = i * MAX_COLOR / 2 / can.getWindowWidth() + j * MAX_COLOR / 2 / can.getWindowHeight();
                can.drawPoint(i, j, ColorInt(color, color, color));
            }
        }
    }
}

/*!
 * \brief Draws a neat pattern of points to a canvas using OMP
 * \details
 * - A parallel block is set up with #pragma omp parallel using all available processors
 * - The actual number of threads created is stored in \b nthreads
 * - The number of lines per thread is calculated and stored in \b myPart
 * - The starting position of each given thread is calculated and stored in \b myStart
 * - The outer for loop is set up in a block pattern, and the inner for loop runs from 100 to the Canvas width - 100
 * - Some dark voodoo magic is used to calculate the color for a given point
 * - The point is drawn to the Canvas
 * .
 * \param can Reference to the Canvas being drawn to
 */
void colorPointsFunction(Canvas& can) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int nthreads = omp_get_num_threads();
        int myPart = can.getWindowHeight() / nthreads;
        int myStart = myPart * omp_get_thread_num();
        for (int j = myStart; j < myStart + myPart; j++) {
            for (int i = 100; i < can.getWindowWidth() - 100; i++) {
                if (i % 2 == 0)
                    can.drawPoint(i, j, ColorInt(j % NUM_COLORS, i % NUM_COLORS, (i * j) % 113));
                else
                    can.drawPoint(i, j, ColorInt(i % NUM_COLORS, j % NUM_COLORS, (i * j) % NUM_COLORS));
            }
            if (!can.getIsOpen()) break;
        }
    }
}

/*!
 * \brief Draws a chain of randomly colored lines at the target framerate (default 60 FPS)
 * \details
 * - \b xNew and \b yNew are set to the middle of the canvas
 * - A timer is set up to go off every \b FRAME seconds (\b FRAME == 1 / \b FPS)
 * - While the canvas is open:
 *   - The timer sleeps until the next frame is ready to be drawn
 *   - \b xOld and \b yOld are set to \b xNew and \b yNew, while \b xNew and \b yNew are set to random positions
 *   - A random color is chosen
 *   - The line is drawn to the Canvas
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void lineChainFunction(Canvas& can) {
    int xOld, yOld, xNew = can.getWindowWidth() / 2, yNew = can.getWindowHeight() / 2, red, green, blue;
    Timer t(FRAME);
    while (can.getIsOpen()) {  // Checks to see if the window has been closed
        t.sleep();
        xOld = xNew;
        yOld = yNew;
        xNew = rand() % can.getWindowWidth();
        yNew = rand() % can.getWindowHeight();
        red = rand() % NUM_COLORS;
        green = rand() % NUM_COLORS;
        blue = rand() % NUM_COLORS;
        can.drawLine(xOld, yOld, xNew, yNew, ColorInt(red, green, blue));
    }
}

/*!
 * \brief Draws a fan of randomly colored lines at the target framerate and a dynamic number of threads using OMP
 * \details
 * - A timer is set up to go off every \b FRAME seconds (\b FRAME == 1 / \b FPS)
 * - While the canvas is open:
 *   - The number of threads to use is recalculated, and the process is forked.
 *   - The timer sleeps on each thread until the next frame is ready to be drawn.
 *   - An offset is calculated based on the thread's ID and a predefined arc-length.
 *   - An angle is then calculated using the offset and the Canvas' current lifespan ( as calculated by \b t.getReps() ).
 *   - The vertices of the lines to be drawn are chosen using the sines and cosines of the predetermined angle.
 *   - A random color is chosen.
 *   - The line is draw to the Canvas.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void lineFanFunction(Canvas& can) {
    const double ARC = 7.11;  //(Arbitrary) spacing between arcs of the fan
    Timer t(FRAME);
    while (can.getIsOpen()) {
        #pragma omp parallel num_threads(omp_get_num_procs())
        {
            t.sleep();
            int a, b, c, d, red, green, blue;
            double angle, offset = omp_get_thread_num() * ARC;
            angle = offset + t.getReps() * RAD;
            a = can.getWindowWidth() / 2 * (1 + sin(angle));
            b = can.getWindowHeight() / 2 * (1 + cos(angle));
            c = can.getWindowWidth() / 2 * (1 - sin(angle));
            d = can.getWindowHeight() / 2 * (1 - cos(angle));
            red = (a + t.getReps()) % NUM_COLORS;
            green = (b + t.getReps()) % NUM_COLORS;
            blue = (a * b + t.getReps()) % NUM_COLORS;
            can.drawLine(a, b, c, d, ColorInt(red, green, blue));
        }
    }
}

/*!
 * \brief Draws the full spectrum across the x, y, and time dimensions at the given framerate
 * and a static number of threads using OMP
 * \details
 * - A timer is set up to go off every \b FRAME seconds (\b FRAME == 1 / \b FPS).
 * - A parallel block is set up with OMP, using one thread per processor.
 * - The actual number of threads spawned is stored in \b nthreads.
 * - While the canvas is open:
 *   - The timer sleeps until the next frame is ready to be drawn
 *   - An outer for loop from 0 to 255 is set up in a per-thread striping pattern.
 *   - An inner for loop runs from 0 to the 255 normally.
 *   - Each point is drawn to the canvas, with x, y, and time representing red, green, and blue respectively.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void spectrumFunction(Canvas& can) {
    Timer t(FRAME);
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
        int nthreads = omp_get_num_threads();
        while (can.getIsOpen()) {
            t.sleep();
            for (int i = omp_get_thread_num(); i < NUM_COLORS; i += nthreads)
                for (int j = 0; j < NUM_COLORS; j++)
                    can.drawPoint(i, j, ColorInt(i, j, t.getReps() % NUM_COLORS));
        }
    }
}

/*!
 * \brief Draws the Mandelbrot set on a CartesianCanvas with custom controls, a specified target update rate,
 *  and a dynamic number of threads
 * \details
 * - The number of theads to use is predetermined and stored in \b THREADS.
 * - The number of iterations to check is predetermined and stored in \b DEPTH.
 * - A timer is set up to go off every ( \b FRAME / 2 ) seconds.
 * - A flag telling us to redraw is set to true
 * - The spacebar on-press event is set to tell the Canvas to clear and re-render.
 * - The left mouse on-press event is set to grab the mouse's current coordinates
 * - The left mouse on-release event is set to grab the mouse's current coordinates, and tell the Canvas to zoom into the
 * bounding rectangle between the current coordinates and those from the left mouse's on press event.
 * - The right mouse on-press event is set to grab the mouse's current coordinates, and tell the Canvas to zoom out
 * from that area.
 * - The mouse's scroll wheel is set to tell the Canvas to zoom in / out by a predetermined amount at the mouse's
 * current coordinates.
 * While the toRedraw flag is set:
 *   - Set the toRender flag to false
 *   - Reset the timer to 0.
 *   - Fork off the predetermined number of parallel threads using OMP
 *   - Store the actual number of threads spawned in \b nthreads
 *   - Figure the cartesian size of the area each thread is to calculate and store it in \b blocksize
 *   - Figure out the actual number of rows each thread is to calculate and store it in \b blockheight
 *   - Run an outer loop from 0 to blockheight:
 *     - Calculate the cartesian coordinates of the thread's \b row as
 *     \b blocksize * (our ID) + (top of our CartesianCanvas) + (cartesian height of our physical pixels) * k
 *     - Run an inner loop from the minimum to maximum x values of the Cartesian Canvas, stepping by pixel width:
 *       - (Basic Mandelbrot calculations; see http://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings )
 *       - Break if the Canvas is to redraw
 *       .
 *     - Break if the Canvas is to redraw
 *     .
 *   Output the time it took to compute the screen
 *   Sleep the thread for one frame until the Canvas is closed by the user or told to redraw
 *   .
 * .
 * \param can Reference to the CartesianCanvas being drawn to
 */
void mandelbrotFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 8;  //omp_get_num_procs();
    const unsigned int DEPTH = MAX_COLOR;
    Timer t(FRAME / 2);
    Decimal firstX, firstY, secondX, secondY;
    bool toRedraw = true;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &toRedraw]() {
        can.clear();
        toRedraw = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, &firstX, &firstY]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), firstX, firstY);
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, &firstX, &firstY, &secondX, &secondY, &toRedraw]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), secondX, secondY);
        can.zoom(firstX, firstY, secondX, secondY);
        toRedraw = true;
    });
    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, &toRedraw]() {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        can.zoom(x, y, 1.5);
        toRedraw = true;
    });
    can.bindToScroll([&can, &toRedraw](double dx, double dy) {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        Decimal scale;
        if (dy == 1) scale = .5;
        else scale = 1.5;
        can.zoom(x, y, scale);
        toRedraw = true;
    });

    while (toRedraw) {
        toRedraw = false;
        t.reset();
        #pragma omp parallel num_threads(THREADS)
        {
            unsigned int nthreads = omp_get_num_threads();
            double blocksize = can.getCartHeight() / nthreads;
            double blockheight = can.getWindowHeight() / nthreads;
            for (unsigned int k = 0; k <= blockheight && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
                long double row = blocksize * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
                for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                    complex originalComplex(col, row);
                    complex c(col, row);
                    unsigned iterations = 0;
                    while (std::abs(c) < 2.0 && iterations != DEPTH) {  // Compute it until it escapes or we give up
                        iterations++;
                        c = c * c + originalComplex;
                    }
                    if (iterations == DEPTH) { // If the point never escaped, draw it black
                        can.drawPoint(col, row, BLACK);
                    } else { // Otherwise, draw it with color based on how long it took
                        can.drawPoint(col, row,
                                      ColorInt(iterations % 151,
                                             ((iterations % 131) + omp_get_thread_num() * 128 / nthreads) % 255,
                                             iterations % 255));
                    }
                    if (toRedraw) break;
                }
                if (toRedraw) break;
            }

        }
        std::cout << t.getTime() << std::endl;
        while (can.getIsOpen() && !toRedraw)
            t.sleep();
    }
}

/*!
 * \brief Simulates Langton's Ant at speeds faster than the Canvas' framerate
 * \details
 * - The Canvas' width and height are stored
 * - Space is allocated for a flattened 2d array of booleans representing states of the various pixels on the Canvas
 * - The number of iterations per frame is set to a large number
 * - The initial position \b xx, \b yy is set to the center of the Canvas
 * - The initial direction is set to UP
 * - A timer is set up to expire once every \b FRAME / \b IPF seconds
 * - While the Canvas is open:
 *   - The timer sleeps until the next frame is ready to be drawn
 *   - If the ant's current cell is filled, turn right and color it; otherwise, turn left and blacken it
 *   - Invert the filled status of the square
 *   - Move forward one square from the current position, wrapping around the screen if on a boundary
 * - Deallocate space for the filled array
 * .
 * \param can Reference to the Canvas being drawn to
 */
void langtonFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the screen sizes
              WINDOW_H = can.getWindowHeight();
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    const int IPF = 1000;   // Iterations per frame
    int xx = WINDOW_W / 2,  // Start at the center
        yy = WINDOW_H / 2;
    int direction = UP;
    Timer t(FRAME / IPF);
    while (can.getIsOpen()) {
        t.sleep();
        if (filled[xx + WINDOW_W * yy]) {
            direction = (direction + 1) % 4;                      // Turn right
            can.drawPoint(xx, yy, ColorInt(MAX_COLOR, 0, 0));     // Color it
        } else {
            direction = (direction + 3) % 4;                      // Turn left
            can.drawPoint(xx, yy, ColorFloat(0.0f, 0.0f, 0.0f));  // Don't color it
        }
        filled[xx + WINDOW_W * yy] ^= true;  // Invert the square
        switch (direction) {                 // Check for wrap-around and move
            case UP:
                yy = yy > 0 ? yy - 1 : WINDOW_H - 1; break;
            case RIGHT:
                xx = xx < WINDOW_W - 1 ? xx + 1 : 0; break;
            case DOWN:
                yy = yy < WINDOW_H - 1 ? yy + 1 : 0; break;
            case LEFT:
                xx = xx > 0 ? xx - 1 : WINDOW_W - 1; break;
            default:
                std::cout << "BAD: dir == " << direction << std::endl; break;
        }
    }
    delete filled;
}

/*!
 * \brief Simulates 4 Langton's Ants at speeds faster than the Canvas' framerate
 * \details Same principle as langtonFunction(). Key differences:
 * - A variable \b RADIUS holds the distance from the center for each ant
 * - Each ant is tracked separately, with arrays holding each type of variable
 * - Cells are not flipped until all ants have moved
 * .
 * \param can Reference to the Canvas being drawn to
 */
void langtonColonyFunction(Canvas& can) {
    const int IPF = 1000,                       // Iterations per frame
              WINDOW_W = can.getWindowWidth(),  // Set the window sizes
              WINDOW_H = can.getWindowHeight(),
              RADIUS = WINDOW_H / 6;            // How far apart to space the ants
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    int xx[4], yy[4], dir[4], red[4], green[4], blue[4];
    xx[0] = WINDOW_W / 2 - RADIUS;
    yy[0] = WINDOW_H / 2;
    red[0] = MAX_COLOR;
    green[0] = 0;
    blue[0] = 0;
    xx[1] = WINDOW_W / 2;
    yy[1] = WINDOW_H / 2 - RADIUS;
    red[1] = 0;
    green[1] = 0;
    blue[1] = MAX_COLOR;
    xx[2] = WINDOW_W / 2 + RADIUS;
    yy[2] = WINDOW_H / 2;
    red[2] = 0;
    green[2] = MAX_COLOR;
    blue[2] = 0;
    xx[3] = WINDOW_W / 2;
    yy[3] = WINDOW_H / 2 + RADIUS;
    red[3] = MAX_COLOR;
    green[3] = 0;
    blue[3] = MAX_COLOR;

    for (int i = 0; i < 4; i++) {
        dir[i] = i;
    }

    Timer t(FRAME / IPF);
    while (can.getIsOpen()) {
        t.sleep();
        for (int j = 0; j < 4; j++) {
            if (filled[xx[j] + WINDOW_W * yy[j]]) {
                dir[j] = (dir[j] + 1) % 4;
                can.drawPoint(xx[j], yy[j], ColorInt(red[j], green[j], blue[j]));
            } else {
                dir[j] = (dir[j] + 3) % 4;
                can.drawPoint(xx[j], yy[j], ColorInt(red[j] / 2, green[j] / 2, blue[j] / 2));
            }
            switch (dir[j]) {
                case UP:
                    yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H - 1;
                    break;
                case RIGHT:
                    xx[j] = (xx[j] < WINDOW_H - 1) ? xx[j] + 1 : 0;
                    break;
                case DOWN:
                    yy[j] = (yy[j] < WINDOW_H - 1) ? yy[j] + 1 : 0;
                    break;
                case LEFT:
                    xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_H - 1;
                    break;
                default:
                    break;
            }
        }
        for (int j = 0; j < 4; j++)
            filled[xx[j] + WINDOW_W * yy[j]] ^= true;  //Invert the squares the ants are on
    }
    delete filled;
}

/*!
 * \brief Simulates 4 Langton's Ants at speeds faster than the Canvas' framerate, with nicer colors!
 * \details Same as langtonColonyFunction(), but with dynamically-colored ants.
 * \param can Reference to the Canvas being drawn to
 */
void langtonRainbowFunction(Canvas& can) {
    const int IPF = 1000,                                               // Iterations per frame
              WINDOW_W = can.getWindowWidth(),                          // Set the window sizes
              WINDOW_H = can.getWindowHeight(),
              RADIUS = WINDOW_H / 6;                                    // How far apart to space the ants
    bool* filled = new bool[WINDOW_W * WINDOW_H]();                     // Create an empty bitmap for the window
    int xx[4], yy[4], dir[4];
    xx[0] = WINDOW_W / 2 - RADIUS;
    yy[0] = WINDOW_H / 2;
    xx[1] = WINDOW_W / 2;
    yy[1] = WINDOW_H / 2 - RADIUS;
    xx[2] = WINDOW_W / 2 + RADIUS;
    yy[2] = WINDOW_H / 2;
    xx[3] = WINDOW_W / 2;
    yy[3] = WINDOW_H / 2 + RADIUS;

    for (int i = 0; i < 4; i++) {
        dir[i] = i;
    }

    Timer t(FRAME / IPF);
    while (can.getIsOpen()) {
        t.sleep();
        for (int j = 0; j < 4; j++) {
            if (filled[xx[j] + WINDOW_W * yy[j]]) {
                dir[j] = (dir[j] + 1) % 4;
                can.drawPoint(xx[j], yy[j],
                              ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 1.0f, .25f));
            } else {
                dir[j] = (dir[j] + 3) % 4;
                can.drawPoint(xx[j], yy[j],
                              ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 0.5f, .25f));
            }
            switch (dir[j]) {
                case UP:
                    yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H - 1;
                    break;
                case RIGHT:
                    xx[j] = (xx[j] < WINDOW_H - 1) ? xx[j] + 1 : 0;
                    break;
                case DOWN:
                    yy[j] = (yy[j] < WINDOW_H - 1) ? yy[j] + 1 : 0;
                    break;
                case LEFT:
                    xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_H - 1;
                    break;
                default:
                    break;
            }
        }
        for (int j = 0; j < 4; j++)
            filled[xx[j] + WINDOW_W * yy[j]] ^= true;        //Invert the squares the ants are on
    }
    delete filled;
}

/*!
 * \brief Provides a visualization for a basic (and slow) shaker sort.
 * \details
 * - The \b SIZE of the list of items and the number of iterations per frame ( \b IPF) are set.
 * - An integer array of size \b SIZE is allocated.
 * - A flag \b goingUp is set.
 * - Our integer array is filled with random integers under the Canvas' height.
 * - The background color is set to gray for visibility
 * - A timer is set up to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - The timer sleeps until the next frame is ready to be drawn
 *   - If the minimum sorted element equals the maximum, we're done
 *   - At a rate of \b IPF times a second:
 *     - If we're going up and the element above us is less than us, swap.
 *     - If we're going down and the element below us is less than us, swap.
 *     - Move in the current direction, inverting our direction if we've reached the minimum / maximum.
 *     .
 *   - Clear the Canvas
 *   - From 0 to \b SIZE:
 *     - Get the height of each element in the integer array
 *     - Draw it as a yellow rectangle if it's the currently-computed member; draw it red otherwise.
 *     .
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void dumbSortFunction(Canvas& can) {
    const int SIZE = 550,   // Size of the data pool
        IPF = 50;           // Iterations per frame
    int numbers[SIZE];      // Array to store the data
    int pos = 0, temp, min = 1, max = SIZE - 1, lastSwap = 0;
    bool goingUp = true;
    for (int i = 0; i < SIZE; i++)
        numbers[i] = rand() % (can.getWindowHeight() - 40);
    can.setBackgroundColor(GREY);
    Timer t(FRAME);
    while (can.getIsOpen()) {
        t.sleep();
        if (min >= max) return;  // We are done sorting

        for (int i = 0; i < IPF; i++) {
            if (goingUp) {
                if (numbers[pos] > numbers[pos + 1]) {
                    temp = numbers[pos];
                    numbers[pos] = numbers[pos + 1];
                    numbers[pos + 1] = temp;
                    lastSwap = pos;
                }
                if (pos >= max) {
                    pos = max;
                    max = (lastSwap < max) ? lastSwap : max - 1;
                    goingUp = !goingUp;
                } else
                    pos++;
            } else {
                if (numbers[pos] < numbers[pos - 1]) {
                    temp = numbers[pos];
                    numbers[pos] = numbers[pos - 1];
                    numbers[pos - 1] = temp;
                    lastSwap = pos;
                }
                if (pos <= min) {
                    pos = min;
                    min = (lastSwap > min) ? lastSwap : min + 1;
                    goingUp = !goingUp;
                } else
                    pos--;
            }
        }

        can.clear();
        int start = 50, width = 1, height;
        ColorFloat color;
        for (int i = 0; i < SIZE; i++, start += width * 2) {
            height = numbers[i];
            if (i == pos)
                color = ColorInt(MAX_COLOR, MAX_COLOR, 0);
            else
                color = ColorInt(MAX_COLOR, 0, 0);
            can.drawRectangle(start, can.getWindowHeight() - 20 - height, width, height, color);
        }
    }
}

/*!
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and private per-thread variables
 * \details
 * - \b THREADS is set to a number greater than the number of physical processors
 * - The center of the canvas is computed and stored.
 * - The radius of the wheel is computed (using the minimum of the Canvas width / height) and stored.
 * - The size of the \b GRADIENT is computed and stored.
 * - Variables for the second and third vertices of a triangle and the current shading are initiallized.
 * - A timer is set up to expire every \b FRAME seconds.
 * - The predetermined number of parallel threads is forked off using OMP, pulling along the coordinate
 * and shading variables.
 * - The actual number of threads is stored in \b nthreads.
 * - A color \b delta is computed.
 * - Each thread's thread id ( \b tid ) is stored.
 * - Each thread's shading is computed using it's id and \b nthreads
 * - While the Canvas is open:
 *   - The timer sleeps until the next frame is ready to be drawn
 *   - Each thread's \b start is computed using the timer's lifetime and it's current id
 *   - The second and third coordinates of a triangle approximating an arc of a circle are
 *   computed using the \b GRADIENT and the thread's \b start position.
 *   A triangle is drawn on the Canvas for each thread, with the first vertex in the center, and
 *   the second and third vertices as computed above, with a hue based on the precomputed \b start,
 *   full saturation, and a value of \b shading.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void colorWheelFunction(Canvas& can) {
    const int THREADS = 64,                           // Number of threads to compute with
              WINDOW_CW = can.getWindowWidth() / 2,   // Set the center of the window
              WINDOW_CH = can.getWindowHeight() / 2;
    const float RADIUS = (WINDOW_CH < WINDOW_CW ? WINDOW_CH : WINDOW_CW) * .95,  // Radius of wheel
    GRADIENT = 2 * PI / NUM_COLORS;                   // Gap between wedges
    float x2, x3, y2, y3, shading;
    Timer t(FRAME);
    #pragma omp parallel num_threads(THREADS) private(x2,x3,y2,y3,shading)
    {
        int nthreads = omp_get_num_threads();
        int delta = NUM_COLORS / nthreads;           // Distance between threads to compute
        int tid = omp_get_thread_num();
        shading = 1 - (float) tid / nthreads;
        while (can.getIsOpen()) {
            t.sleep();
            int start = (NUM_COLORS - t.getReps() % NUM_COLORS + tid * delta) % NUM_COLORS;
            x2 = WINDOW_CW + RADIUS * sin(GRADIENT * start);
            y2 = WINDOW_CH + RADIUS * cos(GRADIENT * start);
            x3 = WINDOW_CW + RADIUS * sin(GRADIENT * (start + 1));
            y3 = WINDOW_CH + RADIUS * cos(GRADIENT * (start + 1));
            can.drawTriangle(WINDOW_CW, WINDOW_CH, x2, y2, x3, y3,
                             ColorHSV(start * 6.0f / NUM_COLORS, 1.0f, shading));
        }
    }
}

/*!
 * \brief Draws the outputs of some functions using CartesianCanvas
 * \details
 * - Draw axes through the origin, with spacing 1.0 between x ticks and 5.0 between y ticks.
 * - Initialize a CosineFunction, and draw it using the currently rendered area of the CartesianCanvas.
 * - Initialize a PowerFunction with argument 2 (square), and draw it using
 * the currently rendered area of the CartesianCanvas.
 * - Declare a new function that computes some bizarre polynomial.
 * - Initialize the new function, and draw it using the currently rendered area of the CartesianCanvas.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void functionFunction(CartesianCanvas& can) {
    can.drawAxes(0, 0, 1, 5);

    CosineFunction function1;
    can.drawFunction(function1);

    PowerFunction function2(2);
    can.drawFunction(function2);

    class myFunction : public Function {
     public:
        virtual long double valueAt(long double x) const {
            return 5 * pow(x, 4) + 2 * pow(x, 3) + x + 15;
        }
    };

    myFunction function3;
    can.drawFunction(function3);
}

/*!
 * \brief Draws the area under a predefined function (the integral) using CartesianCanvas
 * \details
 * - Use a predetermined number of threads, storing it in \b THREADS.
 * - Bind Q's press event to quit the rendering in case it takes too long.
 * - Set up a timer to expire once every \b FRAME / 2 seconds
 * - Draw axes through the origin, with spacing PI/4 between x ticks and 0.5 between y ticks.
 * - Store the width of the canvas's pixel in \b pw to avoid thousands of multiple function calls.
 * - Initialize and draw a CosineFunction using the currently rendered area of the CartesianCanvas.
 * - Set the CartesianCanvas' font from an external font file using setFont().
 * - Draw some labels on the CartesianCanvas to make things look pretty.
 * - Set up a parallel block with OMP using \b THREADS threads.
 * - Set \b nthreads to the actual number of threads spawned.
 * - Calculate each thread's share of the work and store it in \b offset.
 * - Calculate each thread's starting position and store it in \b start.
 * - Calculate each thread's stopping position and store it in \b stop.
 * - For each thread, from \b start to \b stop with step size \b pw:
 *   - If the Canvas was closed, break.
 *   - Sleep the timer until it's ready to render.
 *   - Draw a line from x,0 to x,f(x) for the current x.
 *   .
 * .
 * \param can Reference to the CartesianCanvas being drawn to
 */
void cosineIntegralFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 8;
    can.bindToButton(TSGL_Q, TSGL_PRESS, [&can](){  // Quit on press of Q
        can.end();
    });

    Timer t(FRAME/2);
    can.drawAxes(0, 0, PI/4, .5);
    long double pw = can.getPixelWidth();
    CosineFunction function1;
    can.drawFunction(function1);

    can.setFont("assets/freefont/FreeSerif.ttf");
    can.drawText(L"-1.5π", -1.5 * PI - .1, .25, 20);  // Note the important capital L, used to support Unicode.
    can.drawText(L"1.5\u03C0", 1.5 * PI - .2, .25, 20);
    can.drawText(L"1", .1, 1.05, 20);
    can.drawText(L"-1", .1, -1.1, 20);

    #pragma omp parallel num_threads(THREADS)
    {
        int nthreads = omp_get_num_threads();
        long double offset = 3*PI / nthreads;
        long double start = -1.5*PI + omp_get_thread_num() * offset;
        long double stop = start + offset;
        for (long double i = start; i < stop; i += pw) {
            if (!can.getIsOpen()) break;
            t.sleep();
            can.drawLine(i, 0, i, function1.valueAt(i), Colors::highContrastColor(omp_get_thread_num()));
        }
    }
}

/*!
 * \brief Draws a gradient color wheel using OMP with multiple threads per processor and TSGL's colored polygons
 * \details Same principle as colorWheelFunction(). Since colored polygons take arrays as parameters
 * to allow for arbitrary-length polygons, there are some key differences:
 * - Colors, x and y coordinates are declared within the #pragma omp block so they can be
 * declared as an array
 * - At the end, drawColoredPolygon() is called on a polygon with 3 vertices, with arrays for the
 * x coordinates, y coordinates, and color.
 * \param can Reference to the Canvas being drawn to
 */
void gradientWheelFunction(Canvas& can) {
    const int   THREADS = 256,                          // Number of threads to compute with
                WINDOW_CW = can.getWindowWidth() / 2,   // Center of the screen
                WINDOW_CH = can.getWindowHeight() / 2;
    const float RADIUS = (WINDOW_CH < WINDOW_CW ? WINDOW_CH : WINDOW_CW) * .95,  // Radius of wheel
                ARCLENGTH = 2 * PI / NUM_COLORS;                                    // Gap between wedges
    Timer t(FRAME);
    #pragma omp parallel num_threads(THREADS)
    {
        int nthreads = omp_get_num_threads();
        int tid = omp_get_thread_num();         // Thread ID
        int delta = NUM_COLORS / nthreads;           // Distance between threads to compute
        float shading = 1 - (float) tid / nthreads;  // Shading based on thread ID
        ColorFloat color[3];                         // RGB color to build
        int xx[3], yy[3];                       // Setup the arrays of values for vertices
        int start;
        while (can.getIsOpen()) {
            t.sleep();
            start = (NUM_COLORS - t.getReps() % NUM_COLORS + delta*tid) % NUM_COLORS;  // shapes by the location and frame

            color[0] = ColorHSV(start /                         (float) NUM_COLORS * 6, 0.0f, shading, 1.0f);
            color[1] = ColorHSV(start /                         (float) NUM_COLORS * 6, 1.0f, shading, 1.0f);
            color[2] = ColorHSV(( (start+delta) % NUM_COLORS) / (float) NUM_COLORS * 6, 1.0f, shading, 1.0f);

            xx[0] = WINDOW_CW;                                            // Set first vertex to center of screen
            yy[0] = WINDOW_CH;
            xx[1] = WINDOW_CW + RADIUS * sin(ARCLENGTH * start);          // Add the next two vertices to around the circle
            yy[1] = WINDOW_CH + RADIUS * cos(ARCLENGTH * start);
            xx[2] = WINDOW_CW + RADIUS * sin(ARCLENGTH * (start + 1));
            yy[2] = WINDOW_CH + RADIUS * cos(ARCLENGTH * (start + 1));

            can.drawColoredPolygon(3, xx, yy, color);
        }
    }
}

/*!
 * \brief Draws semi-transparent rectangles on a Canvas
 * \details
 * - Store the Canvas' width and height in variables for easy reuse.
 * - Set up a timer to expire once every \b FRAME / 10 seconds
 * - While the Canvas is open:
 *   - Sleep the timer until the Canvas is ready to draw.
 *   - Select a random position on the Canvas for a corner of a rectangle
 *   - Draw a rectangle stretching from the specified corner to anoter corner on the Canvas,
 *   with a random color and a transparency of 16 (~0.06).
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void alphaRectangleFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the center of the window
              WINDOW_H = can.getWindowHeight();
    int a, b;
    Timer t(FRAME / 10);
    while (can.getIsOpen()) {
        t.sleep();
        a = rand() % WINDOW_W;
        b = rand() % WINDOW_H;
        can.drawRectangle(a, b, rand() % (WINDOW_W - a), rand() % (WINDOW_H - b),
                          ColorInt(rand() % MAX_COLOR, rand() % MAX_COLOR, rand() % MAX_COLOR, 16));
    }
}

/*!
 * \brief Simulates 4 Langton's Ants with alpha transparency used for drawing and a couple
 * other neat features.
 * \details Mostly the same as langtonColonyFunction(), but with a couple key differences:
 * - Each ant is drawn with an alpha (transparency) value of 16 (~0.06)
 * - We set up an additional timer \b pulse to keep track of intervals between clearing the screen
 * - We set up a function \b tempo, which resets the \b pulse timer, sets its interval to the
 * time since the last reset, and makes the Canvas clear itself at that interval.
 * - We bind the left mouse button and the enter button to the described \b tempo function.
 * - We bind the space button to clearing the Canvas.
 * - After all the ants are moved on a given frame, if the \b pulse timer is expired, we clear
 * the screen.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void alphaLangtonFunction(Canvas& can) {
    const int IPF = 5000,                         // Iterations per frame
              WINDOW_W = can.getWindowWidth(),    // Set the window sizes
              WINDOW_H = can.getWindowHeight(),
              RADIUS = WINDOW_H / 6;              // How far apart to space the ants
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    int xx[4], yy[4], dir[4], red[4], green[4], blue[4];
    xx[0] = WINDOW_W / 2 - RADIUS;
    yy[0] = WINDOW_H / 2;
    red[0] = MAX_COLOR;
    green[0] = 0;
    blue[0] = 0;
    xx[1] = WINDOW_W / 2;
    yy[1] = WINDOW_H / 2 - RADIUS;
    red[1] = 0;
    green[1] = 0;
    blue[1] = MAX_COLOR;
    xx[2] = WINDOW_W / 2 + RADIUS;
    yy[2] = WINDOW_H / 2;
    red[2] = 0;
    green[2] = MAX_COLOR;
    blue[2] = 0;
    xx[3] = WINDOW_W / 2;
    yy[3] = WINDOW_H / 2 + RADIUS;
    red[3] = MAX_COLOR;
    green[3] = 0;
    blue[3] = MAX_COLOR;

    for (int i = 0; i < 4; i++) {
        dir[i] = i;
    }
    Timer t(FRAME);
    Timer pulse(28.72 / 60);
    double time = pulse.getTime();

    auto tempo = [&can, &pulse, &time]() {
        std::cout << (pulse.getTime() - time) << std::endl;
        pulse.reset(pulse.getTime() - time);
        time = pulse.getTime();
        can.clear();
    };
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_ENTER, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clear();
    });

    while (can.getIsOpen()) {
        t.sleep();
        for (int i = 0; i < IPF; i++) {
            for (int j = 0; j < 4; j++) {
                if (filled[xx[j] + WINDOW_W * yy[j]]) {
                    dir[j] = (dir[j] + 1) % 4;
                    can.drawPoint(xx[j], yy[j], ColorInt(MAX_COLOR / 2, MAX_COLOR / 2, MAX_COLOR / 2, 16));
                } else {
                    dir[j] = (dir[j] + 3) % 4;
                    can.drawPoint(xx[j], yy[j], ColorInt(red[j], green[j], blue[j], 16));
                }
            }
            for (int j = 0; j < 4; j++) {
                filled[xx[j] + WINDOW_W * yy[j]] ^= true;
                switch (dir[j]) {
                    case UP:
                        yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H - 1;
                        break;
                    case RIGHT:
                        xx[j] = (xx[j] < WINDOW_W - 1) ? xx[j] + 1 : 0;
                        break;
                    case DOWN:
                        yy[j] = (yy[j] < WINDOW_H - 1) ? yy[j] + 1 : 0;
                        break;
                    case LEFT:
                        xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_W - 1;
                        break;
                    default:
                        break;
                }
            }
            if (pulse.pastPeriod()) {
                can.clear();
            }
        }
    }
    delete filled;
}

/*!
 * \brief Draws a gradient Mandelbrot set on a CartesianCanvas
 * \details Same as mandelbrotFunction(), but with smoother shading ( see
 * http://linas.org/art-gallery/escape/smooth.html ).
 * \param can Reference to the CartesianCanvas being drawn to
 */
void gradientMandelbrotFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 32;
    const unsigned int DEPTH = 32;
    Timer t(FRAME / 2);
    Decimal firstX, firstY, secondX, secondY;
    bool toRender = true;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &toRender]() {
        can.clear();
        toRender = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, &firstX, &firstY]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), firstX, firstY);
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, &firstX, &firstY, &secondX, &secondY, &toRender]() {
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), secondX, secondY);
        can.zoom(firstX, firstY, secondX, secondY);
        toRender = true;
    });
    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, &toRender]() {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        can.zoom(x, y, 1.5);
        toRender = true;
    });
    can.bindToScroll([&can, &toRender](double dx, double dy) {
        Decimal x, y;
        can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
        Decimal scale;
        if (dy == 1) scale = .5;
        else scale = 1.5;
        can.zoom(x, y, scale);
        toRender = true;
    });

    while (toRender) {
        toRender = false;
        #pragma omp parallel num_threads(THREADS)
        {
            unsigned int nthreads = omp_get_num_threads();
            double blockstart = can.getCartHeight() / nthreads;
            unsigned int iterations;
            double smooth;
            for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
                long double row = blockstart * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
                for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                    complex c(col, row);
                    complex z(col, row);
                    smooth = exp(-std::abs(z));
                    iterations = 0;
                    while (std::abs(z) < 2.0l && iterations != DEPTH) {
                        iterations++;
                        z = z * z + c;
                        smooth += exp(-std::abs(z));
                    }
                    int i;
                    for (i = 0; i < 2; i++) {
                        iterations++;
                        z = z * z + c;
                        smooth += exp(-std::abs(z));
                    }
                    smooth /= (DEPTH + i + 1);
                    can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, 1.0f, 1.0f));
                    if (toRender) break;
                }
                if (toRender) break;
            }
        }
        while (can.getIsOpen() && !toRender)
            t.sleep();
    }
}

/*!
 * \brief Draws a gradient Nova set on a CartesianCanvas
 * \details Same as gradientMandelbrotFunction(), but with a nova fractal ( more time-consuming
 * to compute; see http://en.wikipedia.org/wiki/Nova_fractal ).
 * \param can Reference to the CartesianCanvas being drawn to
 */
void novaFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 32;
    const unsigned int DEPTH = 200;
    const long double R = 1.0l;
    #pragma omp parallel num_threads(THREADS)
    {
        unsigned int nthreads = omp_get_num_threads();
        double BLOCKSTART = (can.getMaxY() - can.getMinY()) / nthreads;
        unsigned int iterations;
        double smooth;
        for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
            long double row = BLOCKSTART * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
            for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                complex c(col, row);
                complex z(1, 0);
                smooth = exp(-std::abs(z));
                complex n, d, c1;
                complex r(1, 0);
                iterations = 0;
                while (std::abs(z) < 2.0l && iterations != DEPTH) {
                    iterations++;
                    n = z * z * z - 1.0l;
                    d = z * z * 3.0l;
                    z = z + c - (R * n / d);
                    smooth += exp(-std::abs(z));
                }
                for (int i = 0; i < 20; i++) {
                    iterations++;
                    n = z * z * z - 1.0l;
                    d = z * z * 3.0l;
                    z = z + c - (R * n / d);
                    smooth += exp(-std::abs(z));
                }
                smooth /= DEPTH;
                if (smooth != smooth || smooth < 0)  // Check to see if smooth is NAN
                smooth = 0;
                while (smooth > 1)
                    smooth -= 1;
                can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, (float) smooth, 1.0f));
            }
        }
    }
}

/*!
 * \brief Draws a randomly generated Voronoi diagram, using OMP and private variables
 * ( see http://en.wikipedia.org/wiki/Voronoi_diagram ).
 * \details
 * - The Canvas's dimensions are stored in a local variable.
 * - The number of control points are predetermined and stored in \b POINTS.
 * - We seed the random number generator with the time.
 * - We allocate arrays for x and y coordinates for each of the points.
 * - We allocate an array of k-values for each pixel on the Canvas
 * - We declare an array of colors for each of the points
 * - We initialize variables for the top, right, left, bottom, x-center, and y-center colors
 * - The best k-value is initially set to 0.
 * - We initialize variables keeping track of the best and current distances.
 * - We randomize the locations of the control points.
 * - We randomize the colors of key areas of the diagram.
 * - For each control point:
 *   - We get its x coordinate and y coordinate
 *   - We determine its x-color based on a linear interpolation on the x-axis.
 *   - We determine its y-color based on a linear interpolation on the y-axis.
 *   - We determine its color based on an even mixture of its x-color and y-color.
 *   .
 * - We start a parallel OMP block, allowing the system to determine the best means of parallelization
 * - For each column, we set the best k to 0. Then:
 *   - For each row:
 *     - For each point:
 *       - We calculate the distance from row,col to the control point
 *       - If this distance is less than the best distance, we update the best distance and best-k
 *       (best point).
 *       - We set the k-value for row,col to bestk.
 *       - We draw a pixel at row,col with the point bestk's color.
 *       - If we closed the Canvas, break.
 *       .
 *     .
 *   .
 *   Deallocate the arrays for x, y, and kvalue.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void voronoiFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),      // Set the screen sizes
              WINDOW_H = can.getWindowHeight(),
              POINTS = 100*4;                       // Set the number of control points
    srand(time(NULL));                              // Seed the random number generator
    int* x = new int[POINTS]();                     // Initialize an array for POINTS x coords
    int* y = new int[POINTS]();                     // Do the same for y coords
    int* kvalue = new int[WINDOW_W * WINDOW_H]();   // Create a mapping of control point values
    ColorFloat color[POINTS];                            // And for an array of colors
    ColorFloat tc, rc, lc, bc, xc, yc;                   // Color for the top, right, left, bottom, x-average, and y-average
    int bestk = 0;                                  // Keep track of the current best k-value
    float bdist, dist, xd, yd;                      // Keep track of the closes matches and current distances
    for (int i = 0; i < POINTS; i++) {              // Randomize the control points
        x[i] = rand() % WINDOW_W;
        y[i] = rand() % WINDOW_H;
    }
    srand(time(NULL));
    tc = Colors::randomColor(1.0f);                            // Randomize the axis colors
    rc = Colors::randomColor(1.0f);
    lc = Colors::randomColor(1.0f);
    bc = Colors::randomColor(1.0f);
    for (int i = 0; i < POINTS; i++) {              // For each control point...
        float xx = (float) x[i] / WINDOW_W;         // Calculate an value from 0:1 based on x coord
        float yy = (float) y[i] / WINDOW_H;         // Do the same for y
        xc = Colors::blendedColor(lc, rc, xx);              // Interpolate between the left and right colors
        yc = Colors::blendedColor(tc, bc, yy);              // Do the same for top and bottom
        color[i] = Colors::blendedColor(xc, yc, 0.5f);      // Complete the 4-way interpolation
    }
    #pragma omp parallel for private(bdist, xd, yd, dist, bestk)
    for (int i = 0; i < WINDOW_W; i++) {            // For each individual point...
        bestk = 0;
        for (int j = 0; j < WINDOW_H; j++) {
            bdist = 9999;                           // Reset the best distance
            for (int k = 0; k < POINTS; k++) {      // Find the closest control point
                xd = i - x[k];                      // Calculate the distance from each control point
                yd = j - y[k];
                dist = sqrt(xd * xd + yd * yd);
                if (dist < bdist) {                 // If it's the closest one
                    bdist = dist;                   // Update the best distance and control point
                    bestk = k;
                }
            }
            kvalue[i * WINDOW_H + j] = bestk;
            can.drawPoint(i, j, color[bestk]);      // Draw the point with the closest control's color
            if (!can.getIsOpen()) break;
        }
    }
    delete x;
    delete y;
    delete kvalue;
}

/*!
 * \brief Draws a randomly generated Voronoi diagram with fancy shading
 * \details Same principle as voronoiFunction(). Key differences:
 * - We keep track of the second best distance to each point in \b nbdist.
 * - We keep track of the kvalues of each 2nd best point in the array \b kvalue2.
 * - In a second post-processing loop through the screen:
 *   - Find the closest and 2nd closest control points to each pixel.
 *   - Find the distance from the pixel to the closest control point and store it in \b d1.
 *   - Find the distance from the closest to the 2nd closest control point and store it in \b kd.
 *   - Set \b shading to ( \b d1 / \b kd ).
 *   - Bind \b shading between 0 and 1, and shade the pixel with \b shading.
 *   - Break if the Canvas is closed.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void shadedVoronoiFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),        // Set the screen sizes
              WINDOW_H = can.getWindowHeight(),
              POINTS = 100;                         // Set the number of control points
    srand(time(NULL));                              // Seed the random number generator
    int* x = new int[POINTS]();                     // Initialize an array for POINTS x coords
    int* y = new int[POINTS]();                     // Do the same for y coords
    int* kvalue = new int[WINDOW_W * WINDOW_H]();   // Create a mapping of control point values
    int* kvalue2 = new int[WINDOW_W * WINDOW_H]();  // Create a mapping of more control point values
    ColorFloat color[POINTS];                     // And for an array of colors
    ColorFloat tc, rc, lc, bc, xc, yc;            // Color for the top, right, left, bottom, x-average, and y-average
    float bdist, nbdist, dist, xd, yd;              // Keep track of the closes matches and current distances
    int bestk = -1, nextbestk = -1;                 // Keep track of the current best k-value
    for (int i = 0; i < POINTS; i++) {              // Randomize the control points
        x[i] = rand() % WINDOW_W;
        y[i] = rand() % WINDOW_H;
    }
    srand(time(NULL));
    tc = Colors::randomColor(1.0f);                       // Randomize the axis colors
    rc = Colors::randomColor(1.0f);
    lc = Colors::randomColor(1.0f);
    bc = Colors::randomColor(1.0f);
    for (int i = 0; i < POINTS; i++) {              // For each control point...
        float xx = (float) x[i] / WINDOW_W;         // Calculate an value from 0:1 based on x coord
        float yy = (float) y[i] / WINDOW_H;         // Do the same for y
        xc = Colors::blendedColor(lc, rc, xx);              // Interpolate between the left and right colors
        yc = Colors::blendedColor(tc, bc, yy);              // Do the same for top and bottom
        color[i] = Colors::blendedColor(xc, yc, 0.5f);      // Complete the 4-way interpolation
//        color[i] = randomColor(rand());           // Or random colors, if you prefer
    }
    for (int i = 0; i < WINDOW_W; i++) {            // For each individual point...
        for (int j = 0; j < WINDOW_H; j++) {
            bdist = nbdist = 9999;                  // Reset the best distance
            for (int k = 0; k < POINTS; k++) {      // Find the closest control point
                xd = i - x[k];                      // Calculate the distance from each control point
                yd = j - y[k];
                dist = sqrt(xd * xd + yd * yd);
                if (dist < bdist) {                 // If it's the closest one
                    nbdist = bdist;                 // Update the next best distance and control point
                    nextbestk = bestk;
                    bdist = dist;                   // Update the best distance and control point
                    bestk = k;
                } else if (dist < nbdist) {         // If it's the second closest one
                    nbdist = dist;                  // Just update the next best distance / CP
                    nextbestk = k;
                }
            }
            kvalue[i * WINDOW_H + j] = bestk;
            kvalue2[i * WINDOW_H + j] = nextbestk;
            can.drawPoint(i, j, color[bestk]);      // Draw the point with the closest control's color
            if (!can.getIsOpen()) break;
        }
    }
    for (int i = 0; i < WINDOW_W; i++) {            // For each individual point...
        for (int j = 0; j < WINDOW_H; j++) {
            int k = kvalue[i * WINDOW_H + j];       // Find its closest control point
            int nk = kvalue2[i * WINDOW_H + j];     // Then find its second closest
            float xd1 = i - x[k];
            float yd1 = j - y[k];
            float d1 = sqrt(xd1 * xd1 + yd1 * yd1); // Find the distance to it closest
            float xkd = x[k] - x[nk];
            float ykd = y[k] - y[nk];
            float kd = sqrt(xkd * xkd + ykd * ykd); // Find the distance between the CPs themselves
            float shading = d1 / kd;
            if (shading > 1)
                shading = 1;
            else if (shading < 0) shading = 0;
            can.drawPoint(i, j, ColorFloat(0.0f, 0.0f, 0.0f, shading));  // Draw the point with the closest control's color

            if (!can.getIsOpen()) break;
        }
    }
    delete x;
    delete y;
    delete kvalue;
    delete kvalue2;
}

/*!
 * \brief Pseudo-simulates a forest fire using a lot of probability and randomness.
 * \details
 * - Store the Canvas's dimensions for ease of use.
 * - Set the fire's life, strength, and maximum spread distance to some predetermined numbers.
 * - Seed the random number generator.
 * - Allocate arrays for storing each pixel's onFire status and flammability.
 * - For each pixel:
 *   - Get its distance from the center of the Canvas.
 *   - Set its flammability and color based upon a semi-arbitrary single-line function.
 *   .
 * - Draw 32 random square "lakes" with very low flammabilities on the Canvas
 * - Declare a mini-firePoint struct with coordinates, life, and strength.
 * - Make a 3x3 square of fire in the middle of the Canvas, and color the pixels accordingly
 * - A timer is set up to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - Sleep until the Canvas is ready to draw.
 *   - For each fire point:
 *     - Pop it from the queue, pushing it back on only if its life > 0.
 *     - For each cell adjacent to the fire, if it is not on the edge of the screen, not already
 *     on fire, and the random number generator rolls a number nower than the cell's flammability,
 *     set that cell on fire.
 *     .
 *   .
 * - Deallocate the onFire and flammability arrays.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void forestFireFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the screen sizes
              WINDOW_H = can.getWindowHeight();
    const float LIFE = 10,
                STRENGTH = 0.03,
                MAXDIST = sqrt(WINDOW_W * WINDOW_W + WINDOW_H * WINDOW_H) / 2;
    srand(time(NULL));  // Seed the random number generator
    bool* onFire = new bool[WINDOW_W * WINDOW_H]();
    float* flammability = new float[WINDOW_W * WINDOW_H]();
    for (int i = 0; i < WINDOW_W; i++) {  // For each individual point
        for (int j = 0; j < WINDOW_H; j++) {
            float xi = std::abs(WINDOW_W / 2 - i);
            float yi = std::abs(WINDOW_H / 2 - j);
            float tdist = (MAXDIST - sqrt(xi * xi + yi * yi)) / MAXDIST;
            float f = 0.01 + (i * j % 100) / 100.0 * randfloat(100) / 2 * tdist;
            flammability[i * WINDOW_H + j] = f;
            can.drawPoint(i, j, ColorFloat(0.0f, f, 0.0f, 1.0f));
        }
    }
    for (int reps = 0; reps < 32; reps++) {
        int x = rand() % WINDOW_W;
        int y = rand() % WINDOW_H;
        int w = rand() % (WINDOW_W - x);
        int h = rand() % (WINDOW_H - y);
        if (w > 32) w = 32;
        if (h > 32) h = 32;
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                flammability[(x + i) * WINDOW_H + (y + j)] = 0.01;
                can.drawPoint(x + i, y + j, ColorFloat(0.0f, 0.0f, 1.0f, 0.25f));
            }
        }
    }
    struct firePoint {
        int x;
        int y;
        float life;
        float strength;
    };
    std::queue<firePoint> fires;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            firePoint fire = { WINDOW_W / 2 - 1 + i, WINDOW_H / 2 - 1 + j, LIFE, STRENGTH };
            fires.push(fire);
            can.drawPoint(WINDOW_W / 2 - 1 + i, WINDOW_H / 2 - 1 + j, ColorFloat(1.0f, 0.0f, 0.0f, STRENGTH));
        }
    }
    Timer t(FRAME);
    while (can.getIsOpen()) {
        t.sleep();
        int l = fires.size();
        for (int i = 0; i < l; i++) {
            firePoint f = fires.front();
            fires.pop();
            if (--f.life > 0) fires.push(f);
            int myCell = f.x * WINDOW_H + f.y;
            if (f.x > 0 && !onFire[myCell - WINDOW_H] && randfloat() < flammability[myCell - WINDOW_H]) {
                firePoint fire = { f.x - 1, f.y, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell - WINDOW_H] = true;
                can.drawPoint(f.x - 1, f.y, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.x < WINDOW_W - 1 && !onFire[myCell + WINDOW_H]
                && randfloat() < flammability[myCell + WINDOW_H]) {
                firePoint fire = { f.x + 1, f.y, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell + WINDOW_H] = true;
                can.drawPoint(f.x + 1, f.y, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.y > 0 && !onFire[myCell - 1] && randfloat() < flammability[myCell - 1]) {
                firePoint fire = { f.x, f.y - 1, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell - 1] = true;
                can.drawPoint(f.x, f.y - 1, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
            if (f.y < WINDOW_H && !onFire[myCell + 1] && randfloat() < flammability[myCell + 1]) {
                firePoint fire = { f.x, f.y + 1, LIFE, f.strength };
                fires.push(fire);
                onFire[myCell + 1] = true;
                can.drawPoint(f.x, f.y + 1, ColorFloat(f.life / LIFE, 0.0f, 0.0f, f.life / LIFE));
            }
        }
    }
    delete onFire;
    delete flammability;
}

/*!
 * \brief Draws various images on a Canvas.
 * \details Very basic test function showcasing image drawing capabilities.
 * - The first 6 images are drawn opaque.
 * - The 7th image is drawn across the entire Canvas with alpha transparency.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void imageFunction(Canvas& can) {
    can.drawImage("assets/test.png", 0, 0, 400, 300);
    can.drawImage("assets/ship.bmp", 400, 0, 400, 300);
    can.drawImage("assets/shiprgb.bmp", 800, 0, 400, 300);
    can.drawImage("assets/sky_main.jpg", 0, 300, 400, 300);
    can.drawImage("assets/Captain-falcon.png", 400, 300, 400, 300);
    can.drawImage("assets/mini.jpg", 800, 300, 400, 300);

    can.drawImage("assets/bestpicture.png", 200, 0, 800, 600, 0.25f);    //ALPHA
}

/*!
 * \brief Draws various images on a CartesianCanvas.
 * \details Same as imageFunction, but on a CartesianCanvas.
 * \param can Reference to the Canvas being drawn to
 */
void imageCartFunction(Cart& can) {
    can.drawImage("assets/test.png", 0, 3, 2, 1.5);
    can.drawImage("assets/ship.bmp", 2, 3, 2, 1.5);
    can.drawImage("assets/shiprgb.bmp", 4, 3, 2, 1.5);
    can.drawImage("assets/sky_main.jpg", 0, 1.5, 2, 1.5);
    can.drawImage("assets/Captain-falcon.png", 2, 1.5, 2, 1.5);
    can.drawImage("assets/mini.jpg", 4, 1.5, 2, 1.5);

    can.drawImage("assets/bestpicture.png", 1, 3, 4, 3, 0.25f);    //ALPHA
}

/*!
 * \brief Draws a large number of pixels on a Canvas at a high framerate.
 * \details Very basic stress test for the Canvas' drawPoint() function.
 * - Set up a timer to expire every \b FRAME seconds.
 * - Set Local variables to track the timer's repetitions, and the Canvas' dimensions.
 * - While the Canvas is open:
 *   - Set \b reps to the timer's current number of repetitions.
 *   - Compute the blue component of the current color based on reps.
 *   - Attempt to draw every pixel with the current 1.0,1.0,blue
 *   - Sleep the timer until the Canvas is ready to draw again.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void highData(Canvas& can) {
    Timer t(FRAME);
    unsigned int reps,
                 width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    while (can.getIsOpen()) {
        reps = t.getReps();
        float blue = (reps % 255) / 255.0f;
        for (unsigned int i = 0; i < width; i++) {
            for (unsigned int j = 0; j < height; j++) {
                can.drawPoint(i, j, ColorFloat(1.0f, 1.0f, blue, 1.0f));
            }
        }
        t.sleep();
    }
}

/*!
 * \brief Draws some text on a Canvas
 * \details
 * - We declare some colors to use for drawing.
 * - We set the Canvas' font from an external font file using setFont().
 * - We draw a few lines of text in various colors using drawText().
 * .
 * \param can Reference to the Canvas being drawn to
 */
void textFunction(Canvas& can) {
    ColorFloat RED = ColorFloat(1.0, 0.0, 0.0, 1.0);
    ColorFloat GREEN = ColorFloat(0.0, 1.0, 0.0, 1.0);
    ColorFloat BLUE = ColorFloat(0.0, 0.0, 1.0, 1.0);

    can.setFont("assets/freefont/FreeSerif.ttf");
    can.drawText(L"A long time ago, in a galaxy far, far away.", 16, 50, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", 16, 150, 32, RED);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", 16, 250, 32, GREEN);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", 16, 350,
                 32, BLUE);
    can.drawText(L"Of *what* exactly that extraordinary event was.", 16, 450, 32, GREY);
    can.drawText(L"And to that I say...oh well.", 16, 550, 32, WHITE);
}

/*!
 * \brief Draws some text on a CartesianCanvas
 * \details Same as textFunction, but with a CartesianCanvas and black text.
 * \param can Reference to the CartesianCanvas being drawn to
 */
void textCartFunction(Cart& can) {
    can.setFont("assets/freefont/FreeSerif.ttf");
    can.drawText(L"A long time ago, in a galaxy far, far away.", .05, .8, 32, BLACK);
    can.drawText(L"Something extraordinary happened.", .05, .6, 32, BLACK);
    can.drawText(L"Something far more extraordinary than anything mankind has ever seen.", .05, .4, 32, BLACK);
    can.drawText(L"Unfortunately, as nobody was around to witness the event, we are largely ignorant", .05, .3,
                 32, BLACK);
    can.drawText(L"Of *what* exactly that extraordinary event was.", .05, .2, 32, BLACK);
    can.drawText(L"And to that I say...oh well.", .05, .1, 32, BLACK);
}

/*!
 * \brief Plays a nice game of Pong!
 * \details
 * - Store the Canvas' dimensions for ease of use, and declare some useful variables.
 * - Seed the random number generator.
 * - Set the intial position of the paddles and the ball.
 * - Randomize the initial direction of the ball until it's at a reasonable speed.
 * - Set up a timer to expire every \b FRAME seconds.
 * - Bind the up arrow's on-press and on-release events to set the right
 * paddle's direction to -1 (up) and 0 (static) respectively.
 * - Bind the down arrow's on-press and on-release events to set the right
 * paddle's direction to 1 (down) and 0 respectively.
 * - Bind the W key's on-press and on-release events to set the left
 * paddle's direction to -1 and 0 respectively.
 * - Bind the S key's on-press and on-release events to set the left
 * paddle's direction to 1 and 0 respectively.
 * - While the Canvas is open:
 *   - Sleep the timer until the Canvas is ready to draw.
 *   - Move the ball in its current direction at its current speed.
 *   - If the ball passed by one of the goal areas, increment the appropriate player's score,
 *   reset the ball's position, and send it off in a random direction and speed.
 *   - If the ball hits the top or bottom of the screen, bounce it.
 *   - If the ball hits a paddle, reverse its x-speed and randomly adjust its y-speed.
 *   - Move the paddles according to their current directions.
 *   - Draw the balls and paddles.
 *   - Draw the scores at the top of the screen.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void pongFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(), WINDOW_H = can.getWindowHeight();
    float leftY, rightY, ballX, ballY;
    float xx, yy, speed, dir;
    int leftdir = 0, rightdir = 0, leftPoints = 0, rightPoints = 0;
    srand(time(NULL));
    // Set initial positions
    leftY = rightY = WINDOW_H / 2 - 32;
    speed = 8;
    ballX = WINDOW_W / 2 - 8;
    ballY = WINDOW_H / 2 - 8;
    // Start the ball off in a random direction
    do {
        dir = randfloat(1000) * 2 * PI;
        xx = speed * cos(dir);
        yy = speed * sin(dir);
    } while (xx > -4 && xx < 4);
    Timer t(FRAME);
    // Set up button bindings
    can.bindToButton(TSGL_UP, TSGL_PRESS, [&rightdir]() {rightdir = -1;});
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&rightdir]() {rightdir = 1;});
    can.bindToButton(TSGL_UP, TSGL_RELEASE, [&rightdir]() {if (rightdir == -1) rightdir = 0;});
    can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [&rightdir]() {if (rightdir == 1) rightdir = 0;});
    can.bindToButton(TSGL_W, TSGL_PRESS, [&leftdir] () {leftdir = -1;});
    can.bindToButton(TSGL_S, TSGL_PRESS, [&leftdir] () {leftdir = 1;});
    can.bindToButton(TSGL_W, TSGL_RELEASE, [&leftdir] () {if (leftdir == -1) leftdir = 0;});
    can.bindToButton(TSGL_S, TSGL_RELEASE, [&leftdir] () {if (leftdir == 1) leftdir = 0;});
    // Check to see if the window has been closed
    while (can.getIsOpen()) {
        t.sleep();
        // Move the ball
        ballX += xx;
        ballY += yy;
        // Handle ball boundary collisions
        if (ballX > WINDOW_W + 8) {
            leftPoints++;
            ballX = WINDOW_W / 2 - 8;
            ballY = WINDOW_H / 2 - 8;
            do {
                dir = randfloat(1000) * 2 * 3.14159f;
                xx = speed * cos(dir);
                yy = speed * sin(dir);
            } while (xx > -4 && xx < 4);
        } else if (ballX < -8) {
            rightPoints++;
            ballX = WINDOW_W / 2 - 8;
            ballY = WINDOW_H / 2 - 8;
            do {
                dir = randfloat(1000) * 2 * 3.14159f;
                xx = speed * cos(dir);
                yy = speed * sin(dir);
            } while (xx > -4 && xx < 4);
        } else if (ballY > WINDOW_H - 8 || ballY < 8) yy = -yy;
        // Handle ball paddle collisions
        if (ballX < 32 && ballX > 0 && ballY > leftY - 8 && ballY < leftY + 72) {
            xx = -xx;
            yy += randfloat(1000) * 2 - 1;
        } else if (ballX > WINDOW_W - 32 && ballX < WINDOW_W && ballY > rightY - 8 && ballY < rightY + 72) {
            xx = -xx;
            yy += randfloat(1000) * 2 - 1;
        }
        // Move the paddles if necessary
        rightY += 4 * rightdir;
        leftY += 4 * leftdir;
        can.clear();
        // Draw paddles and balls
        can.drawRectangle(8, leftY, 24, 64, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
        can.drawRectangle(WINDOW_W - 24 - 8, rightY, 24, 64, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
        can.drawRectangle(ballX - 8, ballY - 8, 16, 16, WHITE);
        // Draw Scores
        for (int i = 0; i < leftPoints; i++) {
            can.drawRectangle(WINDOW_W / 2 - 64 - 4 * i, 16, 2, 8, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
        }
        for (int i = 0; i < rightPoints; i++) {
            can.drawRectangle(WINDOW_W / 2 + 64 + 4 * i, 16, 2, 8, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
        }
    }
}

/*!
 * \brief Grabs the pixels from an image on the Canvas and plays with them.
 * \details
 * - Set a predetermined number of threads and store it in \b THREADs.
 * - Store the Canvas' dimensions for easy use.
 * - Draw an image on the initially blank Canvas, stretched to fill it.
 * - Initialize a pointer to the Canvas' screen buffer.
 * - Set up a parallel OMP block with \b THREADS threads.
 * - Set up a timer that expires every 1/100th of a second.
 * - Determine a block size for each thread based on the Canvas' height and the number
 * of spawned threads.
 * - Determine a starting row for each thread based on \b blocksize and the thread's id.
 * - While the Canvas is open:
 *   - Initialize a new offset pointer to the Canvas' draw buffer + row*width pixels
 *   ( times 3 colors ).
 *   - For each row:
 *     - Reset the index to 0.
 *     - For each column:
 *       - Over each old pixel, draw a new pixel with each of the RGB components incremented
 *       and wrapped.
 *       .
 *     - Increment the buffer offset by \b width pixels ( times 3 colors ).
 *     .
 *   - Sleep until the Canvas is ready to draw again.
 *   .
 * .
 * \note The point is drawn at height - y in the inner for loop because CartesianCanvas, like
 * a real Cartesian plane, has an inverted y-axis.
 * \param can Reference to the Canvas being drawn to
 */
void getPixelsFunction(Canvas& can) {
    const int THREADS = 2;
    unsigned int width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    can.drawImage("assets/test.png", 0, 0, width, height);
    Timer::threadSleepFor(.75);
//    can.recordForNumFrames(100);
    uint8_t* buffer = can.getScreenBuffer();

    #pragma omp parallel num_threads(THREADS)
    {
        Timer t(.01);
        unsigned int blocksize = (double)height / omp_get_num_threads();
        unsigned int row = blocksize * omp_get_thread_num();
        while (can.getIsOpen()) {
            uint8_t* buffer_offset = buffer + row * width * 3;
            for (unsigned int y = row; y < row + blocksize; y++) {
                for (unsigned int x = 0; x < width; x++) {
                    ColorInt c = can.getPoint(x,y);
                    can.drawPoint(x, y, ColorInt((1+c.R) % 256, (1+c.G) % 256, (1+c.B) % 256));
                }
            }
            t.sleep();
            printf("%f\n", t.getTimeBetweenSleeps());
        }
    }
}

/*!
 * \brief Simulates 4 Langton's Ants with alpha transparency (with screenshot capabilities).
 * \details Same as alphaLangtonFunction, with a few key differences:
 * - The enter key is bound to pause the whole animation.
 * - The space key is bound to clear the screen.
 * -
 * \param can Reference to the Canvas being drawn to
 */
void screenshotLangtonFunction(Canvas& can) {
    const int IPF = 5000,                         // Iterations per frame
              WINDOW_W = can.getWindowWidth(),    // Set the window sizes
              WINDOW_H = can.getWindowHeight(), RADIUS = WINDOW_H / 6;              // How far apart to space the ants
    bool paused = false;
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    int xx[4], yy[4], dir[4], red[4], green[4], blue[4];
    xx[0] = WINDOW_W / 2 - RADIUS;
    yy[0] = WINDOW_H / 2;
    red[0] = MAX_COLOR;
    green[0] = 0;
    blue[0] = 0;
    xx[1] = WINDOW_W / 2;
    yy[1] = WINDOW_H / 2 - RADIUS;
    red[1] = 0;
    green[1] = 0;
    blue[1] = MAX_COLOR;
    xx[2] = WINDOW_W / 2 + RADIUS;
    yy[2] = WINDOW_H / 2;
    red[2] = 0;
    green[2] = MAX_COLOR;
    blue[2] = 0;
    xx[3] = WINDOW_W / 2;
    yy[3] = WINDOW_H / 2 + RADIUS;
    red[3] = MAX_COLOR;
    green[3] = 0;
    blue[3] = MAX_COLOR;

    for (int i = 0; i < 4; i++) {
        dir[i] = i;
    }
    Timer t(FRAME);

    can.bindToButton(TSGL_ENTER, TSGL_PRESS, [&paused]() {
        paused = !paused;
    });
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clear();
    });

    while (can.getIsOpen()) {
        if (!paused) {
            t.sleep();
            for (int i = 0; i < IPF; i++) {
                for (int j = 0; j < 4; j++) {
                    if (filled[xx[j] + WINDOW_W * yy[j]]) {
                        dir[j] = (dir[j] + 1) % 4;
                        can.drawPoint(xx[j], yy[j], ColorInt(MAX_COLOR / 2, MAX_COLOR / 2, MAX_COLOR / 2, 16));
                    } else {
                        dir[j] = (dir[j] + 3) % 4;
                        can.drawPoint(xx[j], yy[j], ColorInt(red[j], green[j], blue[j], 16));
                    }
                }
                for (int j = 0; j < 4; j++) {
                    filled[xx[j] + WINDOW_W * yy[j]] ^= true;
                    switch (dir[j]) {
                        case UP:
                            yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H - 1;
                            break;
                        case RIGHT:
                            xx[j] = (xx[j] < WINDOW_W - 1) ? xx[j] + 1 : 0;
                            break;
                        case DOWN:
                            yy[j] = (yy[j] < WINDOW_H - 1) ? yy[j] + 1 : 0;
                            break;
                        case LEFT:
                            xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_W - 1;
                            break;
                        default:
                            break;
                    }
                }
            }
        } else {
            t.sleep();
        }
    }
    delete filled;
}

/*!
 * \brief Grabs the pixels from an image on the Canvas and converts them to grayscale.
 * \details
 * - Predetermine the number of threads and line thickness and store them in variables.
 * - Set up a timer to expire every ( \b FRAME / 2 ) seconds.
 * - Store the Canvas' dimensions for ease of use.
 * - Stretch a fancy image over the Canvas.
 * - Tell the timer to manually sleep for a quarter of a second (to assure the draw buffer is filled).
 * - Initialize a pointer to the Canvas' screen buffer.
 * - Set up a parallel OMP block with \b THREADS threads.
 * - Get the actual number of spawned threads and store it in \b nthreads.
 * - Compute the \b blocksize based on the Canvas height and \b nthreads.
 * - Compute the current thread's row based on \b blocksize and the thread's id.
 * - Generate a nice color based on the thread's id.
 * - Set a grayscale color variable to 0.
 * - For each row:
 *   - Set a buffer index variable based on the row and column ( times 3 colors ).
 *   - For each column:
 *     - Set the gray color variable to the average of the RGB components.
 *     - Draw the grayed point over the old point, and increment the index by one pixel.
 *     .
 *   - Break if the Canvas was closed.
 *   - Sleep until the Canvas is ready to render again.
 *   .
 * - Once a thread is finished grayscaling, draw a box around its rendered area using
 * the predetermined high contrast color.
 * .
 * \param can Reference to the Canvas being drawn to
 */
void greyScaleFunction(Canvas& can) {
    const int THREADS = 4;
    const unsigned int thickness = 3;
    Timer t(FRAME * 2);
    unsigned int width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    can.drawImage("assets/colorful_cars.jpg", 0, 0, width, height);
    Timer::threadSleepFor(.25);
    uint8_t* buffer = can.getScreenBuffer();

    #pragma omp parallel num_threads(THREADS)
    {
        unsigned int nthreads = omp_get_num_threads();
        unsigned int blocksize = height / nthreads;
        unsigned int row = blocksize * omp_get_thread_num();
        ColorFloat color = Colors::highContrastColor(omp_get_thread_num());
        int gray = 0;

        for (unsigned int y = row; y < row + blocksize; y++) {
            int index = y * width * 3;
            for (unsigned int x = 0; x < width; x++) {
                gray = (buffer[index] + buffer[index + 1] + buffer[index + 2]) / 3;
                can.drawPoint(x, height - y, ColorInt(gray, gray, gray));
                index += 3;
            }
            if (! can.getIsOpen()) break;
            t.sleep();
        }
        for (unsigned int i = 0; i < thickness; i++) {
            can.drawRectangle(0 + i, row + 1 + i, width - 2*i, blocksize - 2*i, color, false);
        }
    }
}

/*!
 * \brief Tiny little painting function for drawing with the mouse
 * \details
 * - Initialize and unset a flag for whether the mouse is pressed.
 * - Allocate some large arrays for x,y coordinates and colors.
 * - Set an array index variable to 0.
 * - Declare variables for last x and y coordinates.
 * - Bind the spacebar on-press event to clearing the Canvas.
 * - Bind the left mouse on-press event to setting the lastX, lastY, and first x,y array
 * coordinate to the mouse's current position, and the first color to a random color; also,
 * set the array index to 1, and set the mouseDown flag.
 * - Bind the left mouse on-release event to draw a colored polygon with the built-up vertices,
 * and to unset the mouseDown flag.
 * - Set up a timer to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - If the mouse is down:
 *     - Draw a line from the mouse's last coordinates to the current ones.
 *     - Set the coordinates at position \b index to the mouse's current position.
 *     - Set the corresponding color randomly.
 *     - Increment the index.
 *     .
 *   - Sleep the timer until the Canvas is ready to draw again.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to
 */
void mouseFunction(Canvas& can) {
    bool mouseDown = false;
    int x[10000];
    int y[10000];
    ColorFloat color[10000];
    unsigned int index = 0;
    int lastX, lastY;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clear();
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&mouseDown, &can, &lastX, &lastY, &index, &x, &y, &color]() {
        x[0] = lastX = can.getMouseX();
        y[0] = lastY = can.getMouseY();
        color[0] = Colors::randomColor(1.0f);
        index = 1;
        mouseDown = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&mouseDown, &can, &index, &x, &y, &color]() {
        can.drawColoredPolygon(index, x, y, color, true);
        mouseDown = false;
    });
    Timer t(FRAME);
    while (can.getIsOpen()) {
        if (mouseDown) {
            can.drawLine(lastX, lastY, can.getMouseX(), can.getMouseY());
            x[index] = lastX = can.getMouseX();
            y[index] = lastY = can.getMouseY();
            color[index] = Colors::randomColor(1.0f);
            index++;
        }
        t.sleep();
    }
}

/*!
 * \brief Draws a bunch of triangles and outputs each frame to an image.
 * \details
 * - Declare and initialize variables to keep track of each of three vertices for a triangle.
 * - Set the Canvas to record screenshots for 30 seconds (1800 frames).
 * - Set up a timer to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - Sleep the timer until the Canvas is ready to draw.
 *   - Set the old coordinates to last frame's middle ones.
 *   - Set the middle coordinates to last frame's new ones.
 *   - Set the new coordinates to a random position on the Canvas.
 *   - Draw a traingle on the canvas with the given coordinates and a random color.
 *   .
 * .
 * \note The details of the recordForNumFrames() function are handled automatically in Canvas, and
 * are by default written the \i frames/ directory.
 * \param can Reference to the Canvas being drawn to
 */
void screenShotFunction(Canvas& can) {
    int xNew = can.getWindowWidth() / 2, yNew = can.getWindowHeight() / 2, xMid = xNew, yMid = yNew, xOld, yOld;
    can.recordForNumFrames(FPS * 30);
    Timer t(FRAME);
    while (can.getIsOpen()) {  // Checks to see if the window has been closed
        t.sleep();
        xOld = xMid;
        yOld = yMid;
        xMid = xNew;
        yMid = yNew;
        xNew = rand() % can.getWindowWidth();
        yNew = rand() % can.getWindowHeight();
        can.drawTriangle(xOld, yOld, xMid, yMid, xNew, yNew, Colors::randomColor(), true);
    }
}

/*!
 * \brief A function for testing a Canvas easily
 * \details This function compacts most of the boilerplate code for setting up a Canvas,
 * executing its function, and drawing the results.
 * - The Canvas' background is set according to \b bg
 * - The Canvas is started.
 * - The Canvas' showFPS flag is set according to \b printFPS
 * - The specified function \b f is run on the Canvas.
 * - After the Canvas is finished with the function, it's showFPS flag is unset, and the total
 * Canvas runtime is printed.
 * - The Canvas is closed.
 * \param c Reference to the Canvas being tested
 * \param f The function the Canvas will execute
 * \param printFPS Whether the Canvas will output its FPS to the console
 * \param bg The color of the Canvas' background.
 */
void test(Canvas& c, void (*f)(Canvas&), bool printFPS = false, const ColorFloat &bg = GREY) {
    c.setBackgroundColor(bg);
    c.start();
    c.setShowFPS(printFPS);
    (*f)(c);
    if (printFPS) {
        c.setShowFPS(false);
        std::cout << c.getTime() << std::endl;
    }
    c.close();
}

/*!
 * \brief A function for testing a CartesianCanvas easily
 * \details Exactly the same as test() for the Canvas, but for the CartesianCanvas.
 * \param c Reference to the CartesianCanvas being tested
 * \param f The function the CartesianCanvas will execute
 * \param printFPS Whether the CartesianCanvas will output its FPS to the console
 * \param bg The color of the CartesianCanvas' background.
 */
void test(Cart& c, void (*f)(Cart&), bool printFPS = false, const ColorFloat &bg = GREY) {
    c.setBackgroundColor(bg);
    c.start();
    c.setShowFPS(printFPS);
    (*f)(c);
    if (printFPS) {
        c.setShowFPS(false);
        std::cout << c.getTime() << std::endl;
    }
    c.close();
}

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

void runHalfoftheFunctions() {
   Canvas c1(BUFFER);
   test(c1,graydientFunction,true);
   Canvas c2(BUFFER);
   test(c2,colorPointsFunction,true);
   Canvas c3(BUFFER);
   test(c3,lineChainFunction,true,BLACK);
   Canvas c4(500);
   test(c4,lineFanFunction,false);
   Canvas c5(0,0,255,255,65536);
   test(c5,spectrumFunction,false);
   Cart c6(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, BUFFER);
   test(c6,mandelbrotFunction,false);
   Canvas c7(0, 0, WINDOW_W, WINDOW_H, BUFFER);
   test(c7,langtonFunction,false);
   Canvas c8(0, 0, WINDOW_H, WINDOW_H, BUFFER);
   test(c8,langtonColonyFunction,false);
   Canvas c9(0, 0, WINDOW_H, WINDOW_H, BUFFER);
   test(c9,langtonRainbowFunction,true,BLACK);
   Canvas c10(0, 0, WINDOW_W, WINDOW_H, 1000);
   test(c10,dumbSortFunction,true);
   Canvas c11(0, 0, WINDOW_W, WINDOW_H, 512);
   test(c11,colorWheelFunction);
   Cart c12(0, 0, WINDOW_W, WINDOW_H, -5,-5,5,50, 100);
   test(c12,functionFunction,true,WHITE);
   Cart c13(0, 0, WINDOW_W, WINDOW_H, -5,-1.5,5,1.5, 16000);
   test(c13,cosineIntegralFunction,false,WHITE);
   Canvas c14(0, 0, 1000, 1000, 1024);
   test(c14,gradientWheelFunction,false,BLACK);
}

void runOtherHalfoftheFunctions() {
//   Canvas c15(0, 0, WINDOW_W, WINDOW_H, 512);
//   test(c15,alphaRectangleFunction,false,BLACK);
//   Canvas c16(0, 0, 960, 960, 30000);
//   test(c16,alphaLangtonFunction,true,BLACK);
//   Cart c17(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, BUFFER);
//   test(c17,gradientMandelbrotFunction,true);
//   Cart c18(0, 0, WINDOW_W, WINDOW_H, -1, -0.5, 0, 0.5, BUFFER);
//   test(c18,novaFunction,true);
//   Canvas c19(0, 0, 1600, 1200, BUFFER);
//   test(c19,voronoiFunction,true,WHITE);
//   Canvas c20(0, 0, 1600, 1200, BUFFER);
//   test(c20,shadedVoronoiFunction,false,WHITE);
//   Canvas c21(0, 0, WINDOW_W, WINDOW_H, BUFFER*2);
//   test(c21,forestFireFunction,false);
//   Canvas c22(0,0,1200,600,100);
//   test(c22,imageFunction,false);
//   Canvas c23(0, 0, 1200, 900, 1201 * 900);
//   test(c23, highData, true);
//   Canvas c24(10);
//   test(c24,textFunction,false);
//   Canvas c25(0,0,1600,600,1000);
//   test(c25,pongFunction,false, BLACK);
//   Cart c26(0, 0, 1200, 600, 0, 0, 6, 3, 10);
//   test(c26,imageCartFunction,false);
//   Cart c27(0, 0, WINDOW_W, WINDOW_H, 0, 0, 4, 3, 10);
//   test(c27,textCartFunction,true);
   Canvas c28(0, 0, 800, 600, 500000);
   test(c28,getPixelsFunction,true);
//   Cart c29(0, 0, 800, 600, 0, 0, 800, 600, 50000);
//   test(c29,screenShotFunction,true);
//   Canvas c30(0, 0, 960, 960, 30000);
//   test(c30,screenshotLangtonFunction,true,BLACK);
//   Canvas c31(0, 0, 1280, 1024, 500000);
//   test(c31,greyScaleFunction,true);
//   Canvas c32(0, 0, 800, 600, 5000);
//   test(c32,mouseFunction,false,WHITE);

    Canvas can1(0, 0, 1024, 768, 500000);
    Canvas can2(0, 0, 1024, 768, 500000);
    can2.setBackgroundColor(GREY);
    can1.start();
    can2.start();

    const int THREADS = 4;
    const unsigned int thickness = 3;
    Timer t(FRAME * 2);
    unsigned int width = can1.getWindowWidth(),
                 height = can1.getWindowHeight();
    can1.drawImage("assets/colorful_cars.jpg", 0, 0, width, height);
    Timer::threadSleepFor(1);
    uint8_t* buffer = can1.getScreenBuffer();

    bool toStart = false;
    can2.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&toStart](){
        toStart = true;
    });
    while (can1.getIsOpen() && can2.getIsOpen() && toStart == false) {
        t.sleep();
    }

    #pragma omp parallel num_threads(THREADS)
    {
        Timer t(FRAME * 2);
        unsigned int nthreads = omp_get_num_threads();
        unsigned int blocksize = height / nthreads;
        unsigned int row = blocksize * omp_get_thread_num();
        ColorFloat color = Colors::highContrastColor(omp_get_thread_num());
        int gray = 0;

        for (unsigned int y = row; y < row + blocksize; y++) {
            for (unsigned int x = 0; x < width; x++) {
                ColorInt pixel = can1.getPoint(x, y);
                gray = (pixel.R + pixel.G + pixel.B) / 3;
                can2.drawPoint(x, y, ColorInt(gray, gray, gray));
            }
            if (!(can1.getIsOpen() && can2.getIsOpen())) {
                can1.end();
                can2.end();
                break;
            }
            t.sleep();
        }
        for (unsigned int i = 0; i < thickness; i++) {
            can2.drawRectangle(0 + i, row + 1 + i, width - 2*i, blocksize - 2*i, color, false);
        }
    }
    while (can1.getIsOpen() && can2.getIsOpen()) {
        t.sleep();
    }

    can1.end();
    can2.end();
    can1.close();
    can2.close();
}

int main() {
    glfwInit();  // Initialize GLFW
    Canvas::setDrawBuffer(GL_RIGHT);	// For Patrick's laptop
//    std::thread threadA = std::thread(runHalfoftheFunctions);       // Spawn the rendering thread
    std::thread threadB = std::thread(runOtherHalfoftheFunctions);  // Spawn the rendering thread
//    threadA.join();
    threadB.join();
//    Canvas c29(0, 0, 1200, 900, 50000);
//    test(c29,screenShotFunction,true);
    glfwTerminate();  // Release GLFW
}
