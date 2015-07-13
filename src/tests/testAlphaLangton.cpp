/*
 * testAlphaLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

#include "Langton/AntFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates 4 Langton's Ants with alpha transparency used for drawing and a couple
 * other neat features.
 * \details Mostly the same as langtonColonyFunction(), but with a couple key differences:
 * - Each ant is drawn with an alpha (transparency) value of 16 (~0.06).
 * - There are four LangtonAnts in the AntFarm object (same as langtonColonyFunction() ) *BUT* they have alpha transparency.
 * - We set up an additional timer \b pulse to keep track of intervals between clearing the screen.
 * - We set up a function \b tempo, which resets the \b pulse timer, sets its interval to the
 *   time since the last reset, and makes the Canvas clear itself at that interval.
 * - We bind the left mouse button and the enter button to the described \b tempo function.
 * - We bind the space button to clearing the Canvas.
 * - After all the ants are moved on a given frame, if the \b pulse timer is expired, we clear
 * the screen.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void alphaLangtonFunction(Canvas& can) {
    const int IPF = 5000,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight(),   // Window height
              R = WH / 6;                   // How far apart to space the ants

    AntFarm farm(WW,WH,4,&can);
    farm.addAnt(WW / 2 - R,WH / 2,MAX_COLOR,0,0,0);
    farm.addAnt(WW / 2,WH / 2 - R,0,0,MAX_COLOR,1);
    farm.addAnt(WW / 2 + R,WH / 2,0,MAX_COLOR,0,2);
    farm.addAnt(WW / 2,WH / 2 + R,MAX_COLOR,0,MAX_COLOR,3);

    Timer pulse(28.72 / 60);
    double time = pulse.getTime();

    auto tempo = [&can, &pulse, &time]() {
        std::cout << (pulse.getTime() - time) << std::endl;
        pulse.reset(pulse.getTime() - time);
        time = pulse.getTime();
        can.clear();
    };
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_ENTER, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clear();
    });

    while (can.getIsOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++)
            farm.moveAnts();
        if (pulse.pastPeriod())
            can.clear();
    }
}

//Takes command-line arguments and uses them as the height and width of the Canvas
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c15(-1, -1, w, h, "Langton's Ant (Cool Alpha Edition)", FRAME);
    c15.setBackgroundColor(BLACK);
    c15.start();
    alphaLangtonFunction(c15);
    c15.wait();
}
