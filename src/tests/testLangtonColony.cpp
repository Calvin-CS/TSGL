/*
 * testLangtonColony.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

#include "Langton/AntFarm.h"

/*!
 * \brief Simulates 4 Langton's Ants at speeds faster than the Canvas' framerate
 * \details Same principle as langtonFunction(). Key differences:
 * - A variable \b RADIUS holds the distance from the center for each ant
 * - Each ant is tracked separately, with arrays holding each type of variable
 * - Cells are not flipped until all ants have moved
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void langtonColonyFunction(Canvas& can) {
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

    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++)
            farm.moveAnts();
    }
}

//Takes command-line arguments and uses them as the width and height of the Canvas
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 960;
    int h = (argc > 2) ? atoi(argv[2]) : w;
    Canvas c7(0, 0, w, h, "", FRAME);
    c7.setBackgroundColor(GREY);
    c7.start();
    langtonColonyFunction(c7);
    c7.wait();
}
