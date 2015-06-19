/*
 * testLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

#include "Langton/AntFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates Langton's Ant at speeds faster than the Canvas' framerate
 * \details
 * - The Canvas' width and height are stored
 * - Space is allocated for a flattened 2d array of booleans representing states of the various pixels on the Canvas
 * - The number of iterations per frame is set to a large number
 * - The initial position \b xx, \b yy is set to the center of the Canvas
 * - The initial direction is set to UP
 * - The internal timer of the Canvas is set up to expire once every \b FRAME / \b IPF seconds (see main() )
 * - While the Canvas is open:
 *   - The internal timer sleeps until the next frame is ready to be drawn
 *   - If the ant's current cell is filled, turn right and color it; otherwise, turn left and blacken it
 *   - Invert the filled status of the square
 *   - Move forward one square from the current position, wrapping around the screen if on a boundary
 * - Deallocate space for the filled array
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void langtonFunction(Canvas& can) {
    const int IPF = 1000,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight();   // Window height
    AntFarm farm(WW,WH,4,&can);
    farm.addAnt(WW / 2,WH / 2,MAX_COLOR,0,0,0);
    while (can.getIsOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++)
          farm.moveAnts();
    }
}

//Take command-line arguments for the width and height of the Canvas
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c6(-1, -1, w, h, "", FRAME);
    c6.setBackgroundColor(GREY);
    c6.start();
    langtonFunction(c6);
    c6.wait();
}
