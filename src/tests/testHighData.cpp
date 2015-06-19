/*
 * testHighData.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws a large number of pixels on a Canvas at a high framerate.
 * \details Very basic stress test for the Canvas' drawPoint() function.
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
 * - Set Local variables to track the internal timer's repetitions, and the Canvas' dimensions.
 * - While the Canvas is open:
 *   - Set \b reps to the timer's current number of repetitions.
 *   - Compute the blue component of the current color based on reps.
 *   - Attempt to draw every pixel with the current 1.0,1.0,blue
 *   - Sleep the timer until the Canvas is ready to draw again.
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void highData(Canvas& can) {
    unsigned int reps,
                 width = can.getWindowWidth(),
                 height = can.getWindowHeight();
    while (can.getIsOpen()) {
        reps = can.getReps();  //Added a getReps() function to Canvas that gets the internal timer's reps
        float blue = (reps % 255) / 255.0f;
        for (unsigned int i = 0; i < width; i++) {
            for (unsigned int j = 0; j < height; j++) {
                can.drawPoint(i, j, ColorFloat(1.0f, 1.0f, blue, 1.0f));
            }
        }
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
    }
}

int main() {
    int w = 1.2*Canvas::getDisplayHeight();
    int h = 0.75*w;
    Canvas c22(-1, -1, w, h, "", FRAME);
    c22.start();
    highData(c22);
    c22.wait();
}
