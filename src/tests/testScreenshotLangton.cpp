/*
 * testScreenshotLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

#include "Langton/AntFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates 4 LangtonAnts with alpha transparency (with screenshot capabilities).
 * \details Same as alphaLangtonFunction, with a few key differences:
 * - The enter key is bound to pause the whole animation.
 * - The space key is bound to clear the screen.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void screenshotLangtonFunction(Canvas& can) {
    const int IPF = 5000,                         // Iterations per frame
              WW = can.getWindowWidth(),    // Set the window sizes
              WH = can.getWindowHeight(),
              R = WH / 6;              // How far apart to space the ants
    bool paused = false;

    AntFarm farm(WW,WH,4,&can);
    farm.addAnt(WW / 2 - R,WH / 2,MAX_COLOR,0,0,0);
    farm.addAnt(WW / 2,WH / 2 - R,0,0,MAX_COLOR,1);
    farm.addAnt(WW / 2 + R,WH / 2,0,MAX_COLOR,0,2);
    farm.addAnt(WW / 2,WH / 2 + R,MAX_COLOR,0,MAX_COLOR,3);

    can.bindToButton(TSGL_ENTER, TSGL_PRESS, [&paused]() {
        paused = !paused;
    });
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clear();
    });

    while (can.isOpen()) {
        if (!paused) {
          can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
          for (int j = 0; j < 4; j++)
            farm.ants[j]->changeColor(ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 1.0f, .25f));
          for (int i = 0; i < IPF; i++)
              farm.moveAnts();
        } else {
            can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        }
    }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c30(-1, -1, w, h, "Langton's Ant (enter to pause)", FRAME);
    c30.setBackgroundColor(BLACK);
    c30.start();
    screenshotLangtonFunction(c30);
    c30.wait();
}
