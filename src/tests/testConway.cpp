/*
 * testConway.cpp
 *
 *  Created on: May 27, 2015
 *      Author: Patrick Crain
 */

#include <tsgl.h>
#include "Conway/LifeFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates Conway's Game of Life!
 * ( see https://en.wikipedia.org/wiki/Conway's_Game_of_Life )
 * \details It is drawn in this way:
 * - Get the iterations per frame, window width and window height and store them.
 * - Create a LifeFarm object to hold the objects (ants) that move around on the screen.
 * - Set the ability to draw ants that have "died" in the game to true.
 * - Bind the spacebar so that when it is pressed a screenshot is taken of the current frame.
 * - While the Canvas has not been closed:
 *    - Sleep the internal timer until the next draw cycle.
 *    - Clear the Canvas.
 *    - For 0 to the number of iterations:
 *      - Move the ants in the LifeFarm object.
 *      .
 *   .
 * .
 * \param can Reference to the Canvas to draw to.
 */
void conwayFunction(Canvas& can) {
    const int IPF = 100,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight();   // Window height
    LifeFarm farm(WW,WH,&can,false);    //Change the false to true for something awesome!
//    LifeFarm farm(WW,WH,&can,true);
    farm.setDrawdead(true);
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
      can.recordForNumFrames(1);  //Screenshot
    });
    while (can.getIsOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        can.clear();
        for (int i = 0; i < IPF; i++)
          farm.moveAnts();
    }
}

//Take command-line arguments for the width and height of the Canvas
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    Canvas c6(-1, -1, w, h, "", FRAME);
//    c6.setShowFPS(true);
    c6.setBackgroundColor(BLACK);
    c6.start();
    conwayFunction(c6);
    c6.wait();
}
