/*
 * testLangtonRainbow.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

#include "Langton/AntFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates 4 Langton's Ants at speeds faster than the Canvas' framerate, with nicer colors!
 * \details Same as langtonColonyFunction(), but with dynamically-colored ants.
 * \param can, Reference to the Canvas being drawn to
 */
void langtonRainbowFunction(Canvas& can) {
    const int IPF = 5000,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight(),   // Window height
              R = WH / 6;                   // How far apart to space the ants

    AntFarm farm(WW,WH,4,&can);
    farm.addAnt(WW / 2 - R,WH / 2,MAX_COLOR,0,0,0);
    farm.addAnt(WW / 2,WH / 2 - R,0,0,MAX_COLOR,1);
    farm.addAnt(WW / 2 + R,WH / 2,0,MAX_COLOR,0,2);
    farm.addAnt(WW / 2,WH / 2 + R,MAX_COLOR,0,MAX_COLOR,3);
    farm.setShading(true);
    for (int j = 0; j < 4; j++)
      farm.ants[j]->setAlpha(64);

    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int j = 0; j < 4; j++)
          farm.ants[j]->changeColor(ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 1.0f, .25f));
        for (int i = 0; i < IPF; i++)
            farm.moveAnts();
    }
}

//Takes in command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    Canvas c8(-1, -1, w, h, "", FRAME);
    c8.setBackgroundColor(BLACK);
    c8.start();
    langtonRainbowFunction(c8);
    c8.wait();
}

