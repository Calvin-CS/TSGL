/*
 * testLangton.cpp
 *
 * Usage: ./testLangton <width> <height>
 */

#include <omp.h>
#include <tsgl.h>

#include "Langton/AntFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates 4 Langton's Ants with alpha transparency used for drawing and a couple
 *  other neat features.
 * \details Mostly the same as langtonColonyFunction(), but with a couple key differences:
 * - Each ant is drawn with an alpha (transparency) value of 16 (~0.06).
 * - There are four LangtonAnts in the AntFarm object (same as langtonColonyFunction() ) *BUT* they have alpha transparency.
 * - We set up an additional timer \b pulse to keep track of intervals between clearing the screen.
 * - We set up a function \b tempo, which resets the \b pulse timer, sets its interval to the
 *   time since the last reset, and makes the Canvas clear itself at that interval.
 * - We bind the left mouse button and the enter button to the described \b tempo function.
 * - We bind the enter key to pause the animation.
 * - We bind the space key to clearing the Canvas.
 * - After all the ants are moved on a given frame, if the \b pulse timer is expired, we clear the screen.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void alphaLangtonFunction(Canvas& can) {
    const int IPF = 5000,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight(),   // Window height
              R = WH / 6;                   // How far apart to space the ants
    bool paused = false;

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
    };
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, tempo);
    can.bindToButton(TSGL_ENTER, TSGL_PRESS, [&paused]() {
        paused = !paused;
    });
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can]() {
        can.clearProcedural();
    });

    while (can.isOpen()) {
      if (!paused) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++)
            farm.moveAnts();
        if (pulse.pastPeriod())
            can.clearProcedural();
      }
    }
}

/*!
 * \brief Simulates Langton's Ant at speeds faster than the Canvas' framerate.
 * \details
 * - The Canvas' width and height are stored.
 * - An AntFarm object is created that will display the LangtonAnt.
 * - The number of iterations per frame is set to a large number.
 * - While the Canvas is still open:
 *    - Sleep the internal timer until the next draw cycle.
 *    - For 0 to the iterations per frame:
 *      - Move the LangtonAnt inside of the AntFarm object.
 *      .
 *    .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void langtonFunction(Canvas& can) {
    const int IPF = 1000,                   // Iterations per frame
              WW = can.getWindowWidth(),    // Window width
              WH = can.getWindowHeight();   // Window height
    AntFarm farm(WW,WH,4,&can);
    farm.setParallel(false);
    farm.addAnt(WW / 2,WH / 2,MAX_COLOR,0,0,0);
    while (can.isOpen()) {
        can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++) {
          farm.moveAnts();
        }
    }
}

/*!
 * \brief Simulates 4 LangtonAnts at speeds faster than the Canvas' framerate.
 * \details Same principle as langtonFunction(). Key differences:
 * - A variable \b R holds the distance from the center for each ant.
 * - There are four LangtonAnts inside of the AntFarm object.
 * - Each ant is tracked separately, with arrays holding each type of variable.
 * - The LangtonAnts are shaded this time.
 * .
 * \param can Reference to the Canvas being drawn to.
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
    while (can.isOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int i = 0; i < IPF; i++)
            farm.moveAnts();
    }
}

/*!
 * \brief Simulates 4 LangtonAnts at speeds faster than the Canvas' framerate, with nicer colors!
 * \details Same as langtonColonyFunction(), but with dynamically-colored LangtonAnts.
 * \param can Reference to the Canvas being drawn to.
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

    while (can.isOpen()) {
        can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
        for (int j = 0; j < 4; j++)
          farm.ants[j]->changeColor(ColorHSV((can.getFrameNumber() + 3 * j) % 12 / 2.0f, 1.0f, 1.0f, .25f));
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
    //Alpha Langton
    std::cout << "Alpha Langton's Ant" << std::endl;
    Canvas c1(-1, -1, w, h, "Langton's Ant w/Alpha (enter to pause)");
    c1.setBackgroundColor(BLACK);
    c1.run(alphaLangtonFunction);

    //Regular Langton
    std::cout << "Regular Langton's Ant" << std::endl;
    Canvas c2(-1, -1, w, h, "Langton's Ant");
    c2.run(langtonFunction);

    //Colony Langton
    std::cout << "Multiple Langton's Ants" << std::endl;
    Canvas c3(-1, -1, w, h, "4x Langton's Ants");
    c3.run(langtonColonyFunction);

    //Colorful Langton
    std::cout << "Dynamically Colored Langton's Ants" << std::endl;
    Canvas c4(-1, -1, w, h, "Colorful Langton's Ants");
    c4.setBackgroundColor(BLACK);
    c4.run(langtonRainbowFunction);
}
