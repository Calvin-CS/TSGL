/*
 * testMouse.cpp
 *
 * Usage: ./testMouse <width> <height> <numThreads>
 */

#include <tsgl.h>

using namespace tsgl;

inline float dist(float x1, float y1, float x2, float y2) {
  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

inline float angle(float x1, float y1, float x2, float y2) {
  return atan2(y1 - y2, x1 - x2);
}

inline void rotate(float cx, float cy, int& xx, int& yy, float rot) {
  float scale = cy/cx;
  float stretchy = cy + yy/scale - cx;
  float mydist = dist(xx,stretchy,cx,cy);
  float newang = angle(xx,stretchy,cx,cy)+rot;
  xx = cx + mydist*cos(newang);
  yy = cy + mydist*sin(newang)*scale;
}

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
<<<<<<< HEAD
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
        can.drawConcavePolygon(index, x, y, color, true);
        can.drawConvexPolygon(index, x, y, color, true);  //new, convex polygon
        mouseDown = false;
    });
    while (can.isOpen()) {
        if (mouseDown) {
            can.drawLine(lastX, lastY, can.getMouseX(), can.getMouseY());
            x[index] = lastX = can.getMouseX();
            y[index] = lastY = can.getMouseY();
            color[index] = Colors::randomColor(1.0f);
            index++;
=======
void mouseFunction(Canvas& can, int threads) {
  const int CX = can.getWindowWidth() / 2, CY = can.getWindowHeight() / 2;
  int x[3], y[3], index = 0;
  bool mouseDown = false;
  ColorFloat color[3];

  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
      can.clear();
  });
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&mouseDown, &can, &index, &x, &y, &color]() {
      x[0] = can.getMouseX();
      y[0] = can.getMouseY();
      color[0] = Colors::randomColor(1.0f);
      index = 0;
      mouseDown = true;
  });
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&mouseDown, &can, &index, &x, &y, &color]() {
      mouseDown = false;
  });

  while (can.isOpen()) {
    if (mouseDown) {
      color[1] = color[2];
      color[2] = Colors::randomColor(1.0f);
      x[1] = x[2]; y[1] = y[2];
      x[2] = can.getMouseX(); y[2] = can.getMouseY();
      if (++index > 2)
        #pragma omp parallel num_threads (threads)
        {
          float tdelta = (2*PI*omp_get_thread_num())/omp_get_num_threads();
          int myx[3], myy[3];
          for (int i = 0; i < 3; ++i) {
            myx[i] = x[i]; myy[i] = y[i];
            rotate(CX,CY,myx[i],myy[i],tdelta);
          }
          can.drawConvexPolygon(3,myx,myy,color,true);
>>>>>>> 537c46ba6c9b4aff4c592277352ca791cf994e5a
        }
    }
    can.sleep();
  }
}

int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : -1;
  int h = (argc > 2) ? atoi(argv[2]) : w;
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  Canvas c(-1, -1, w, h, "Draw With Your Mouse!");
  c.setBackgroundColor(WHITE);
  c.run(mouseFunction,t);
}
