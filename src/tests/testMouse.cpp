/*
 * testMouse.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

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
 * \param can, Reference to the Canvas being drawn to
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
        can.drawConcavePolygon(index, x, y, color, true);
        can.drawConvexPolygon(index, x, y, color, true);  //new, convex polygon
        mouseDown = false;
    });
    while (can.getIsOpen()) {
        if (mouseDown) {
            can.drawLine(lastX, lastY, can.getMouseX(), can.getMouseY());
            x[index] = lastX = can.getMouseX();
            y[index] = lastY = can.getMouseY();
            color[index] = Colors::randomColor(1.0f);
            index++;
        }
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
}

int main() {
    Canvas c32(0, 0, 800, 600, "", FRAME);
    c32.setBackgroundColor(WHITE);
    c32.start();
    mouseFunction(c32);
    c32.wait();
}
