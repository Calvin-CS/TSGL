/*
 * testShakerSort.cpp
 *
 * Usage: ./testShakerSort <width> <height>
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
void shakerSortFunction(Canvas& can) {
    const int SIZE = 550,   // Size of the data pool (set to 550 by default)
              IPF = 50;           // Iterations per frame
    Rectangle* rectangles[SIZE];      // Array to store the data
    float numbers[SIZE];      // Array to store the data
    int pos = 0, min = 0, max = SIZE - 1, lastSwap = 0;
    float temp;
    bool goingUp = true;
    int canWidth = (can.getWindowWidth() > can.getDisplayWidth()) ? can.getDisplayWidth() : can.getWindowWidth();
    float start = canWidth * -.45;
    float rectangleWidth = canWidth * .9 / SIZE;
    for (int i = 0; i < SIZE; i++) {
        rectangles[i] = new Rectangle(start + i * rectangleWidth, 0, 0, rectangleWidth, saferand(1,can.getWindowHeight()*.9), 0, 0, 0, RED);
        numbers[i] = rectangles[i]->getHeight();
        can.add(rectangles[i]);
    }
    can.setBackgroundColor(GRAY);
    while (can.isOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        if (min >= max) { // We are done sorting.
            // If we don't call wait(), we segfault. If we call return instead, rectangles[] doesn't get deallocated.
            // return would be fine if we hadn't allocated memory in this method to which Canvas needed access.
            can.wait();
        }
        for (int i = 0; i < IPF; i++) {
            if (goingUp) {
                if (numbers[pos] > numbers[pos + 1]) {
                    temp = numbers[pos];
                    numbers[pos] = numbers[pos + 1];;
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
        ColorFloat color;
        can.pauseDrawing(); //Tell the Canvas to stop updating the screen temporarily
        for (int i = 0; i < SIZE; i++) {
            color = (i == pos) ? YELLOW : RED;
            rectangles[i]->setColor(color);
            rectangles[i]->setHeight(numbers[i]);
        }
        can.resumeDrawing(); //Tell the Canvas it can resume drawing
    }

    for (int i = 0; i < SIZE; i++) {
        delete rectangles[i];
    }
}

//Takes in command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0) {     // Checked the passed width and height if they are valid
        w = 1300; h = 900;        // If not, set the width and height to a default value
    }
    Canvas c(-1, -1, w, h, "Shaker Sort");
    c.run(shakerSortFunction);
}
