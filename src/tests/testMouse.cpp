/*
 * testMouse.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Tiny little painting function for drawing with the mouse.
 * \details
 * - Initialize and unset a flag for whether the mouse is pressed.
 * - Allocate some large arrays for x,y coordinates and colors.
 * - Set an array index variable to 0.
 * - Declare variables for last x and y coordinates.
 * - Bind the spacebar on-press event to clearing the Canvas.
 * - Bind the left mouse on-press event to setting the lastX, lastY, and first x,y array
 *   coordinate to the mouse's current position, and the first color to a random color; also,
 *   set the array index to 1, and set the mouseDown flag.
 * - Bind the left mouse on-release event to draw a Concave & Convex polygon with the built-up vertices,
 *   and to unset the mouseDown flag.
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
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
 * \param can Reference to the Canvas being drawn to.
 */
inline float dist(float x1, float y1, float x2, float y2) {
  float xs = (x1-x2), ys = (y1-y2);
  return sqrt(xs*xs+ys*ys);
}

inline float angle(float x1, float y1, float x2, float y2) {
  return atan2(y1 - y2, x1 - x2);
}

void mouseFunction(Canvas& can, int threads) {
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
//        can.drawConcavePolygon(index, x, y, color, true);
//        can.drawConvexPolygon(index, x, y, color, true);  //new, convex polygon
        mouseDown = false;
    });
    const int CX = can.getWindowWidth() / 2, CY = can.getWindowHeight() / 2;
    while (can.getIsOpen()) {
        if (mouseDown) {
            can.drawLine(lastX, lastY, can.getMouseX(), can.getMouseY());
            x[index] = lastX = can.getMouseX();
            y[index] = lastY = can.getMouseY();
            color[index] = Colors::randomColor(1.0f);
            int x1 = x[0], y1 = y[0];
            int x2 = x[index-1], y2 = y[index-1];
            int x3 = x[index], y3 = y[index];
            #pragma omp parallel num_threads (threads)
            {
              int tid = omp_get_thread_num(), nthreads = omp_get_num_threads();
              float ang1 = angle(x1,y1,CX,CY) + 2*PI*tid/nthreads;
              float ang2 = angle(x2,y2,CX,CY) + 2*PI*tid/nthreads;
              float ang3 = angle(x3,y3,CX,CY) + 2*PI*tid/nthreads;
              float dist1 = dist(x1,y1,CX,CY);
              float dist2 = dist(x2,y2,CX,CY);
              float dist3 = dist(x3,y3,CX,CY);
              if (index > 1) {
                can.drawTriangle(
                    CX+dist1*cos(ang1),CY+dist1*sin(ang1),
                    CX+dist2*cos(ang2),CY+dist2*sin(ang2),
                    CX+dist3*cos(ang3),CY+dist3*sin(ang3),
                    color[index]
                );
              }
            }
            index++;
        }
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
    Canvas c32(-1, -1, w, h, "Draw With Your Mouse!", FRAME);
    c32.setBackgroundColor(WHITE);
    c32.start();
    mouseFunction(c32,t);
    c32.wait();
}
