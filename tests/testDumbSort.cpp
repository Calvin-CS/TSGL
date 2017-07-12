/*
 * testDumbSort.cpp
 *
 * Usage: ./testDumbSort <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Provides a visualization for a basic (and slow) shaker sort.
 * \details
 * - The size of the list of items ( \b SIZE ) and the number of iterations per frame ( \b IPF ) are set.
 * - An integer array of size \b SIZE is allocated.
 * - A flag \b goingUp is set.
 * - Our integer array is filled with random integers under the Canvas' height.
 * - The background color is set to gray for visibility.
 * - The internal timer of the Canvas is set up to expire every \b FRAME seconds.
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn.
 *   - If the minimum sorted element is greater than or equals the maximum, we're done.
 *   - At a rate of \b IPF times a second:
 *     - If we're going up and the element above us is less than us, swap.
 *     - If we're going down and the element below us is less than us, swap.
 *     - Move in the current direction, inverting our direction if we've reached the minimum / maximum.
 *     .
 *   - Pause the animation.
 *   - Clear the Canvas.
 *   - From 0 to \b SIZE:
 *     - Get the height of each element in the integer array.
 *     - Draw it as a yellow rectangle if it's the currently-computed member; draw it red otherwise.
 *     .
 *   - Resume the animation.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void dumbSortFunction(Canvas& can) {
    const int SIZE = 550,   // Size of the data pool (set to 550 by default)
              IPF = 50;           // Iterations per frame
    int numbers[SIZE];      // Array to store the data
    int pos = 0, temp, min = 1, max = SIZE - 2, lastSwap = 0;
    bool goingUp = true;
    for (int i = 0; i < SIZE; i++)
        numbers[i] = rand() % (can.getWindowHeight() - 40);
    can.setBackgroundColor(GRAY);
    while (can.isOpen()) {
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
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
      int start = 50, width = 1, height;
      int cwh = can.getWindowHeight() - 20;
      ColorFloat color;
      can.pauseDrawing(); //Tell the Canvas to stop updating the screen temporarily
      can.clear();
      for (int i = 0; i < SIZE; i++, start += width * 2) {
        height = numbers[i];
        color = ColorInt(MAX_COLOR, (i == pos) ? MAX_COLOR : 0, 0);
        can.drawRectangle(start, cwh - height, start + width, cwh, color);
      }
      can.resumeDrawing(); //Tell the Canvas it can resume drawing
    }
}

//Takes in command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0) {     // Checked the passed width and height if they are valid
      w = 1200; h = 900;        // If not, set the width and height to a default value
    }
    Canvas c(-1, -1, w, h, "Shaker Sort");
    c.run(dumbSortFunction);
}
