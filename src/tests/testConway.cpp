/*
 * testLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

#include "Conway/LifeFarm.h"

void conwayFunction(Canvas& can) {
    const int IPF = 100,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight(),   // Window height
              HW = WW / 2,
              HH = WH / 2;
    LifeFarm farm(WW,WH,&can,true);
    farm.setDrawdead(true);
    farm.addAnt(HW-1,HH);
    farm.addAnt(HW,HH);
    farm.addAnt(HW+1,HH);
    farm.addAnt(HW+1,HH-1);
    farm.addAnt(HW,HH-2);
    while (can.getIsOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++)
          farm.moveAnts();
    }
}

//Take command-line arguments for the width and height of the Canvas
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 960;
    int h = (argc > 2) ? atoi(argv[2]) : w;
    Canvas c6(0, 0, w, h, 300, "", FRAME);
//    c6.setShowFPS(true);
    c6.setBackgroundColor(WHITE);
    c6.start();
    conwayFunction(c6);
    c6.close();
}
