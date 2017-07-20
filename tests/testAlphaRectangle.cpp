/*
 * testAlphaRectangle.cpp
 *
 * Usage: ./testAlphaRectangle <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws semi-transparent rectangles on a Canvas.
 * \details
 * - Store the Canvas' width and height in variables for easy reuse.
 * - Set up the internal timer of the Canvas to expire once every \b FRAME / 10 seconds.
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - Select a random position on the Canvas for a corner of a rectangle.
 *   - Draw a rectangle stretching from the specified corner to another corner on the Canvas,
 *     with a random color and a transparency of 16 (~0.06).
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void alphaRectangleFunction(Canvas& can) {
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;

    srand(time(NULL)); // seed the random number generator

    // Queue to hold the last few rects
    std::queue<Rectangle *> myQueue;

    while (can.isOpen()) {
        can.sleep();

        //Choose random coordinates.
        a = rand() % WW; b = rand() % WH;
        c = rand() % WW; d = rand() % WH;

        // Make the new rectangle and get the pointer
        Rectangle* myRectangle = new Rectangle(a, b, abs(a-c), abs(b-d), ColorInt(rand()%MAX_COLOR, rand()%MAX_COLOR, rand()%MAX_COLOR, 50));

        // Push the rectangle onto the queue and onto the canvas so it can render
        myQueue.push(myRectangle);
        can.add(myRectangle);

        // Remove old rectangles if there are more than 40 of them
        if (myQueue.size() >= 50) {
          can.remove(myQueue.front());  // stop rendering the rectangle each frame
          delete myQueue.front(); // free memory
          myQueue.pop(); // remove the rectangle objects from the queue
        }
    }

    //Delete all pointers from Queue
    while(myQueue.size() > 0) {
      delete myQueue.front(); // free memory
      myQueue.pop(); // remove the shape objects from the queue
    }

}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Fancy Rectangles");
    c.setBackgroundColor(BLACK);
    c.run(alphaRectangleFunction);
}
