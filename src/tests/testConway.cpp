/*
 * testLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>
#include "Conway/LifeFarm.h"

using namespace tsgl;

void conwayFunction(Canvas& can) {
    const int IPF = 100,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight();   // Window height
    LifeFarm farm(WW,WH,&can,false);
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
    int w = (argc > 1) ? atoi(argv[1]) : 960;
    int h = (argc > 2) ? atoi(argv[2]) : w;
    Canvas c6(0, 0, w, h, "", FRAME);
//    c6.setShowFPS(true);
    c6.setBackgroundColor(BLACK);
    c6.start();
    conwayFunction(c6);
    c6.wait();
}
