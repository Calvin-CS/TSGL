/*
 *testAlphaRectangle.cpp
 *
 * Usage: ./testAlphaRectangle <width> <height>
 */

#include "TSGL/tsgl.h"

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

// Rectangle* drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color) {
//        if (x2 < x1) { int t = x1; x1 = x2; x2 = t; }
//        if (y2 < y1) { int t = y1; y1 = y2; y2 = t; }
//        Rectangle* rec = new Rectangle(x1, y1, x2-x1, y2-y1, color);  // Creates the Rectangle with the specified coordinates and color
//        return rec;
//       //  drawShape(rec);                                     // Push it onto our drawing buffer
// }

void alphaRectangleFunction(Canvas& can) {
    const int WW = can.getWindowWidth(), WH = can.getWindowHeight();
    int a, b, c, d;
    // int counter = 0;

    //  Add a traingle to the test page
    Triangle* testTri = new Triangle(100, 100, 200, 100, 50, 150, ColorInt(0*MAX_COLOR, 0*MAX_COLOR, 1*MAX_COLOR, 255));
    testTri->setLayer(5);
    can.add(testTri);

    // Testing old rectangle api
    can.setDefaultLayer(7);
    can.drawRectangle(200, 200, 250, 250, PURPLE);
    can.setDefaultLayer(0);

    // Add the red and white rects over the top
    Rectangle* topRedRect = new Rectangle(500, 500, 50, 50, ColorInt(1*MAX_COLOR, 0*MAX_COLOR, 0*MAX_COLOR, 255));
    Rectangle* topWhiteRect = new Rectangle(540, 540, 50, 50, ColorInt(1*MAX_COLOR, 1*MAX_COLOR, 1*MAX_COLOR, 255));
    topRedRect->setLayer(2);
    topWhiteRect->setLayer(3);
    can.add(topRedRect);
    can.add(topWhiteRect);

    // Add a blue rectangle at the bottom
    Rectangle* botBlueRect = new Rectangle(300, 300, 120, 50, ColorInt(0*MAX_COLOR, 0*MAX_COLOR, 1*MAX_COLOR, 255));
    botBlueRect->setLayer(0);
    can.add(botBlueRect);

    // Queue to hold the last few rects
    std::queue<Rectangle *> myQueue;

    can.setDefaultLayer(1);
    while (can.isOpen()) {
        can.sleep();

        // Some random widths and heights
        a = rand() % WW; b = rand() % WH;
        c = rand() % WW; d = rand() % WH;

        // Make the new rectangle and get the pointer
        Rectangle* myRectangle = new Rectangle(a, b, abs(a-c), abs(b-d), ColorInt(rand()%MAX_COLOR, rand()%MAX_COLOR, rand()%MAX_COLOR, 50));

        // Push the rectangle onto the queue and onto the canvas so it can render
        myQueue.push(myRectangle);
        can.add(myRectangle);

        // Remove old rectangles if there are more than 40 of them
        if (myQueue.size() >= 40) {
          can.remove(myQueue.front());  // stop rendering the rectangle each frame
          delete myQueue.front(); // free memory
          myQueue.pop(); // remove the rectangle object from the queue
        }
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Super Cool Rectangles That Can Be Layered and Removed and Stuff");
    // c.setShowFPS(true);
    c.setBackgroundColor(BLACK);
    c.run(alphaRectangleFunction);
}
