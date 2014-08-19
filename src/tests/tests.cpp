/*
 * tests.cpp provides example usage for the TSGL library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/14/2014
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

void print(const double d) {
    std::cout << d << std::endl;
}
float randfloat(int divisor = 10000) {
    return (rand() % divisor) / (float) divisor;
}
float randint(int max = 10000) {
    return rand() % max;
}

void graydientFunction(Canvas& can) {
    int x = 0;
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &x]() {
        std::cout << x++ << std::endl;
    });

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
void colorPointsFunction(Canvas& can) {
    int threads = omp_get_num_procs();
    #pragma omp parallel num_threads(threads)
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
void lineFanFunction(Canvas& can) {
    const double RAD = PI / 180,  // One radian in degrees
    ARC = 7.11;  // Arc length
    Timer t(FRAME);
    int threads = omp_get_num_procs();
    while (can.getIsOpen()) {
        #pragma omp parallel num_threads(threads)
        {
            t.sleep();
            int a, b, c, d, red, green, blue;
            double angle, offset = omp_get_thread_num() * ARC;;
            bool reverse = false;
            angle = offset + t.getReps() * RAD;
            a = can.getWindowWidth() / 2 * (1 + sin(angle));
            b = can.getWindowHeight() / 2 * (1 + cos(angle));
            c = can.getWindowWidth() / 2 * (1 - sin(angle));
            d = can.getWindowHeight() / 2 * (1 - cos(angle));
            red = (a + t.getReps()) % NUM_COLORS;
            green = (b + t.getReps()) % NUM_COLORS;
            blue = (a * b + t.getReps()) % NUM_COLORS;
            if (!reverse)
                can.drawLine(a, b, c, d, ColorInt(red, green, blue));
            else
                can.drawLine(c, d, a, b, ColorInt(red, green, blue));
            reverse = !reverse;
        }
    }
}
void spectrumFunction(Canvas& can) {
    int threads = omp_get_num_procs();
    Timer t(FRAME);

    #pragma omp parallel num_threads(threads)
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

void mandelbrotFunction(CartesianCanvas& can) {
    const unsigned int THREADS = 8;  //omp_get_num_procs();
    const unsigned int DEPTH = MAX_COLOR;
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
        t.reset();
        #pragma omp parallel num_threads(THREADS)
        {
            unsigned int nthreads = omp_get_num_threads();
            double blockstart = can.getCartHeight() / nthreads;
            unsigned int iterations;
            for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
                long double row = blockstart * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
                for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
                    complex originalComplex(col, row);
                    complex c(col, row);
                    iterations = 0;
                    while (std::abs(c) < 2.0 && iterations != DEPTH) {  // Compute it until it escapes or we give up
                        iterations++;
                        c = c * c + originalComplex;
                    }
                    if (iterations == DEPTH) {
                        // If the point never escaped, draw it black
                        can.drawPoint(col, row, BLACK);
                    } else {
                        // Otherwise, draw it with color based on how long it took
                        can.drawPoint(col, row,
                                      ColorInt(iterations % 151,
                                             ((iterations % 131) + omp_get_thread_num() * 128 / nthreads) % 255,
                                             iterations % 255));
                    }
                    if (toRender) break;
                }
                if (toRender) break;
            }

        }
        print(t.getTime());
        while (can.getIsOpen() && !toRender)
            t.sleep();
    }
}

void langtonFunction(Canvas& can) {
    const int WINDOW_W = can.getWindowWidth(),  // Set the screen sizes
              WINDOW_H = can.getWindowHeight();
    bool* filled = new bool[WINDOW_W * WINDOW_H]();  // Create an empty bitmap for the window
    const int IPF = 1000;   // Iterations per frame
    int xx = WINDOW_W / 2,  // Start at the center
        yy = WINDOW_H / 2;
    int direction = UP;
    Timer t(1.0 / (FPS * IPF));
    while (can.getIsOpen()) {
        t.sleep();
        if (filled[xx + WINDOW_W * yy]) {
            direction = (direction + 1) % 4;                 // Turn right
            can.drawPoint(xx, yy, ColorInt(MAX_COLOR, 0, 0));  // Color it
        } else {
            direction = (direction + 3) % 4;                 // Turn left
            can.drawPoint(xx, yy, ColorFloat(0.0f, 0.0f, 0.0f));  // Don't color it
        }
        filled[xx + WINDOW_W * yy] ^= true;  // Invert the square
        switch (direction) {                 // Check for wrap-around and move
            case UP:
                yy = yy > 0 ? yy - 1 : WINDOW_H - 1;
                break;
            case RIGHT:
                xx = xx < WINDOW_W - 1 ? xx + 1 : 0;
                break;
            case DOWN:
                yy = yy < WINDOW_H - 1 ? yy + 1 : 0;
                break;
            case LEFT:
                xx = xx > 0 ? xx - 1 : WINDOW_W - 1;
                break;
            default:
                std::cout << "BAD: dir == " << direction << std::endl;
                break;
        }
    }
    delete filled;
}

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

    Timer t(1.0 / (60.0 * IPF));
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
        while (can.getIsOpen()) {
            t.sleep();
            int start = (NUM_COLORS - t.getReps() % NUM_COLORS + tid * delta) % NUM_COLORS;
            shading = 1 - (float) tid / nthreads;
            x2 = WINDOW_CW + RADIUS * sin(GRADIENT * start);
            y2 = WINDOW_CH + RADIUS * cos(GRADIENT * start);
            x3 = WINDOW_CW + RADIUS * sin(GRADIENT * (start + 1));
            y3 = WINDOW_CH + RADIUS * cos(GRADIENT * (start + 1));
            can.drawTriangle(WINDOW_CW, WINDOW_CH, x2, y2, x3, y3,
                             ColorHSV(start * 6.0f / NUM_COLORS, 1.0f, shading));
        }
    }
}

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

void cosineIntegralFunction(CartesianCanvas& can) {
    can.bindToButton(TSGL_Q, TSGL_PRESS, [&can](){  // Quit on press of Q
        can.end();
    });

    Timer t(FRAME/2);
    const unsigned int THREADS = 8;
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

            color[0] = ColorHSV(start /                         (float) NUM_COLORS * 6, 0.0f, 1.0f, 1.0f);
            color[1] = ColorHSV(start /                         (float) NUM_COLORS * 6, 1.0f, 1.0f, 1.0f);
            color[2] = ColorHSV(( (start+delta) % NUM_COLORS) / (float) NUM_COLORS * 6, 1.0f, 1.0f, 1.0f);
            for (int i = 0; i < 3; i++) {
                color[i].R *= shading;
                color[i].G *= shading;
                color[i].B *= shading;
            }

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
    float wdist[POINTS] = { 0 };                    // Keep track of the worst distances for shading
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
            if (bdist > wdist[bestk]) wdist[bestk] = bdist;

            if (!can.getIsOpen()) break;
        }
    }
    delete x;
    delete y;
    delete kvalue;
}

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
    float wdist[POINTS] = { 0 };                    // Keep track of the worst distances for shading
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
            if (bdist > wdist[bestk]) wdist[bestk] = bdist;

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
//            float xd2 = i-x[nk];
//            float yd2 = j-y[nk];
//            float d2 = sqrt(xd2*xd2+yd2*yd2);     // Then again to its second closest
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

void imageFunction(Canvas& can) {
    can.drawImage("assets/test.png", 0, 0, 400, 300);
    can.drawImage("assets/ship.bmp", 400, 0, 400, 300);
    can.drawImage("assets/shiprgb.bmp", 800, 0, 400, 300);
    can.drawImage("assets/sky_main.jpg", 0, 300, 400, 300);
    can.drawImage("assets/Captain-falcon.png", 400, 300, 400, 300);
    can.drawImage("assets/mini.jpg", 800, 300, 400, 300);

    can.drawImage("assets/bestpicture.png", 200, 0, 800, 600, 0.25f);    //ALPHA
}
void imageCartFunction(Cart& can) {
    can.drawImage("assets/test.png", 0, 3, 2, 1.5);
    can.drawImage("assets/ship.bmp", 2, 3, 2, 1.5);
    can.drawImage("assets/shiprgb.bmp", 4, 3, 2, 1.5);
    can.drawImage("assets/sky_main.jpg", 0, 1.5, 2, 1.5);
    can.drawImage("assets/Captain-falcon.png", 2, 1.5, 2, 1.5);
    can.drawImage("assets/mini.jpg", 4, 1.5, 2, 1.5);

    can.drawImage("assets/bestpicture.png", 1, 3, 4, 3, 0.25f);    //ALPHA
}

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
        dir = randfloat(1000) * 2 * 3.14159f;
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

void getPixelsFunction(Canvas& can) {
    const int THREADS = 1;
    Timer t(FRAME);
    can.drawImage("assets/test.png", 0, 0, 800, 600);
    unsigned int width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    can.setUpdateScreenCopy(true);
    Timer::threadSleepFor(.75);
    can.recordForNumFrames(100);
    uint8_t* buffer = can.getScreenBuffer();

    while (can.getIsOpen()) {
        #pragma omp parallel num_threads(THREADS)
        {
            unsigned int nthreads = omp_get_num_threads();
            unsigned int blocksize = (double)height / nthreads;
            unsigned int row = blocksize * omp_get_thread_num();
            for (unsigned int y = row; y < row + blocksize; y++) {
                int index = y * width * 3;
                for (unsigned int x = 0; x < width; x++) {
                    can.drawPoint(x, height - y,
                                  ColorInt((1 + buffer[index]) % 256, (1 + buffer[index + 1]) % 256,
                                         (1 + buffer[index + 2]) % 256));
                    index += 3;
                }
            }
        }
        t.sleep();
    }
}

void shapeTestFunction(Canvas& can) {
    Timer t(FRAME);
    can.recordForNumFrames(3 * FPS);
    while (can.getIsOpen()) {
        t.sleep();
        can.clear();
        srand(time(NULL));
        for (int i = 0; i < 255; i++)
            can.drawCircle(can.getWindowWidth() / 2, can.getWindowHeight() / 2, i, 64,
                           Colors::randomColor(1.0f), false);
        can.drawCircle(can.getWindowWidth() / 2, can.getWindowHeight() / 2, 256, 64,
                       Colors::randomColor(1.0f), true);
        can.drawRectangle(32, 32, can.getWindowWidth() - 64, can.getWindowHeight() - 64, BLACK, false);
        can.drawTriangle(can.getWindowWidth() / 2, 32, can.getWindowWidth() - 32, can.getWindowHeight() - 32, 32,
                         can.getWindowHeight() - 32, BLACK, false);
    }
}

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

void greyScaleFunction(Canvas& can) {
    const int THREADS = 16;
    const unsigned int thickness = 3;
    Timer t(FRAME * 2);
    can.drawImage("assets/test.png", 0, 0, 800, 600);
    unsigned int width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    can.setUpdateScreenCopy(true);
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
                can.drawPoint(x, y, ColorInt(gray, gray, gray));
                index += 3;
           }
            t.sleep();
        }
        for (unsigned int i = 0; i < thickness; i++) {
            can.drawRectangle(0 + i, row + 1 + i, width - 2*i, blocksize - 2*i, color, false);
        }
    }
}

void mouseFunction(Canvas& can) {
    bool mouseDown = false;
    int x[10000];
    int y[10000];
    ColorFloat color[1000];
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

void test(Canvas& c, void (*f)(Canvas&), bool printFPS = false, const ColorFloat &bg = GREY) {
    c.setBackgroundColor(bg);
    c.start();
    c.setShowFPS(printFPS);
    (*f)(c);
    if (printFPS) {
        c.setShowFPS(false);
        print(c.getTime());
    }
    c.close();
}
void test(Cart& c, void (*f)(Cart&), bool printFPS = false, const ColorFloat &bg = GREY) {
    c.setBackgroundColor(bg);
    c.start();
    c.setShowFPS(printFPS);
    (*f)(c);
    if (printFPS) {
        c.setShowFPS(false);
        print(c.getTime());
    }
    print(c.getTime());
    c.close();
}

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H;

void runHalfoftheFunctions() {
    Canvas c1(BUFFER);
    test(c1,graydientFunction,true);
    Canvas c2(BUFFER);
    test(c2,colorPointsFunction,true);
    Canvas c3(BUFFER);
    test(c3,lineChainFunction,true,BLACK);
    Canvas c4(500);
    test(c4,lineFanFunction,false);
    Canvas c5(65536);
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
    Canvas c15(0, 0, WINDOW_W, WINDOW_H, 512);
    test(c15,alphaRectangleFunction,false,BLACK);
    Canvas c16(0, 0, 960, 960, 30000);
    test(c16,alphaLangtonFunction,true,BLACK);
    Cart c17(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, BUFFER);
    test(c17,gradientMandelbrotFunction,true);
    Cart c18(0, 0, WINDOW_W, WINDOW_H, -1, -0.5, 0, 0.5, BUFFER);
    test(c18,novaFunction,true);
    Canvas c19(0, 0, 1600, 1200, BUFFER);
    test(c19,voronoiFunction,true,WHITE);
    Canvas c20(0, 0, 1600, 1200, BUFFER);
    test(c20,shadedVoronoiFunction,false,WHITE);
    Canvas c21(0, 0, WINDOW_W, WINDOW_H, BUFFER*2);
    test(c21,forestFireFunction,false);
    Canvas c22(0,0,1200,600,100);
    test(c22,imageFunction,false);
    Canvas c23(0, 0, 1200, 900, 1201 * 900);
    test(c23, highData, true);
    Canvas c24(10);
    test(c24,textFunction,false);
    Canvas c25(0,0,1600,600,1000);
    test(c25,pongFunction,false, BLACK);
    Cart c26(0, 0, 1200, 600, 0, 0, 6, 3, 10);
    test(c26,imageCartFunction,false);
    Cart c27(0, 0, WINDOW_W, WINDOW_H, 0, 0, 4, 3, 10);
    test(c27,textCartFunction,true);
    Canvas c28(0, 0, 800, 600, 500000);
    test(c28,getPixelsFunction,true);
    Cart c29(0, 0, 800, 600, 0, 0, 800, 600, 50000);
    test(c29,shapeTestFunction,true);
    Canvas c30(0, 0, 960, 960, 30000);
    test(c30,screenshotLangtonFunction,true,BLACK);
    Canvas c31(0, 0, 800, 600, 500000);
    test(c31,greyScaleFunction,true);
    Canvas c32(0, 0, 800, 600, 5000);
    test(c32,mouseFunction,false,WHITE);
}

int main() {
    glfwInit();  // Initialize GLFW
    std::thread threadA = std::thread(runHalfoftheFunctions);       // Spawn the rendering thread
    std::thread threadB = std::thread(runOtherHalfoftheFunctions);  // Spawn the rendering thread
    threadA.join();
    threadB.join();
    glfwTerminate();    // Release GLFW
}
