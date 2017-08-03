/*
 * Kaleidoscope.cpp
 *
 * Usage: ./Kaleidoscope <width> <height> <numThreads>
 */

#include <tsgl.h>
#include <queue>
#include <mutex>

using namespace tsgl;

/*!
 * \brief Tiny little painting function for drawing with the mouse.
 * \details
 * - Initialize and unset a flag for whether the mouse is pressed.
 * - Allocate some arrays for x,y coordinates and a ColorFloat for the colors.
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
 *     - Add a triangle with coordiantes at the first moue click, recent position, and current location, for each thread.
 *     - Set the corresponding color randomly.
 *     - Increment the index.
 *     .
 *   - Sleep the timer until the Canvas is ready to draw again.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void mouseFunction(Canvas& can, int threads) {
  const int CX = can.getWindowWidth() / 2, CY = can.getWindowHeight() / 2;
  int x[3], y[3], index = 0;
  bool mouseDown = false;
  ColorFloat color;
  std::queue<Shape*> shapes; //Stores shapes for clearing
  std::mutex queueLock; //Protects the shape queue all threads use

  //Clear Canvas when spacebar pressed
  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can,&shapes,&queueLock]() {
    queueLock.lock();
    while(shapes.size() > 0) { //Empty the shape queue
      ConvexPolygon* oldShape = shapes.front();
      shapes.pop();
      can.remove(oldShape);
      delete oldShape;
    }
    queueLock.unlock();
  });

  //Start new shape drawing when left mouse button pressed
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&mouseDown, &can, &index, &x, &y, &color]() {
      x[0] = can.getMouseX();
      y[0] = can.getMouseY();
      color = Colors::randomColor(1.0f);
      index = 0;
      mouseDown = true;
  });

  //Stop drawing when left mouse button released
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&mouseDown, &can, &index, &x, &y, &color]() {
      mouseDown = false;
  });

  while (can.isOpen()) {
    if (mouseDown) {
      x[1] = x[2]; y[1] = y[2];
      x[2] = can.getMouseX(); y[2] = can.getMouseY();
      if (++index > 2)
      color = Colors::randomColor();
        #pragma omp parallel num_threads (threads)
        {
          float tdelta = (360*omp_get_thread_num())/omp_get_num_threads();
          Polygon* newShape = new Triangle(x[0],y[0],x[1],y[1],x[2],y[1],color);
          newShape->setHasOutline(false);
          newShape->setRotation(tdelta,CX,CY);
          queueLock.lock(); shapes.push(newShape); queueLock.unlock();
          can.add(newShape);
        }
    }
    can.sleep();
  }

  //Delete shape pointers after Canvas is closed
  queueLock.lock();
  while(shapes.size()>0) {
    delete shapes.front();
    shapes.pop();
  }
  queueLock.unlock();
}

int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : -1;
  int h = (argc > 2) ? atoi(argv[2]) : w;
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  Canvas c(-1, -1, w, h, "Draw With Your Mouse!");
  c.setBackgroundColor(WHITE);
  c.run(mouseFunction,t);
}
