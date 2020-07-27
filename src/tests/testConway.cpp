/*
 * testConway.cpp
 *
 * Usage: ./testConway <width> <height>
 */

#include <tsgl.h>
#include "Conway/LifeFarm.h"

using namespace tsgl;

/*!
 * \brief Simulates Conway's Game of Life! (Now interactive!)
 * \note See https://en.wikipedia.org/wiki/Conway's_Game_of_Life
 * \details It is drawn in this way:
 * - Get the iterations per frame, window width and window height and store them.
 * - Create a LifeFarm object to hold the objects (ants) that move around on the screen.
 * - Set the ability to draw ants that have "died" in the game to true.
 * - Set boolean flags that determine when the animation has been paused and when the left mouse button
 *   has been clicked.
 * - Bind the spacebar so that when it is pressed a screenshot is taken of the current frame.
 *   Also, set the paused boolean flag to true.
 * - Bind the left mouse button so that when it is clicked the boolean flag for keeping track of the mouse's
 *   state is set to true. When it is released, set that flag to false.
 * - While the Canvas has not been closed:
 *    - Sleep the internal timer until the next draw cycle.
 *    - If the paused boolean flag is not set:
 *      - Clear the Canvas.
 *      - For 0 to the number of iterations:
 *        - If the mouse has been clicked:
 *          - Add an ant to the LifeFarm object.
 *          - Draw it on the Canvas.
 *          .
 *        - Move the ants in the LifeFarm object.
 *        .
 *      .
 *    - If the mouse has been clicked while the Canvas has been paused,
 *      add an ant to the LifeFarm object and draw it to the Canvas.
 *    .
 * .
 * \param can Reference to the Canvas to draw to.
 */
void conwayFunction(Canvas& can) {
  Background * bg = can.getBackground();
  const int IPF = 100,                   // Iterations per frame
            WW = can.getWindowWidth(),    // Window width
            WH = can.getWindowHeight();   // Window height
  LifeFarm farm(WW,WH,&can,false);    //Change the false to true for something awesome!
  farm.setDrawdead(true);
  bool paused = false;
  bool mouseDown = false;
  // can.drawRectangle(1,1,can.getWindowWidth()-2,can.getWindowHeight()-2,GREEN,false,PI/4);

  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, &paused]() {
    paused = !paused;
    can.recordForNumFrames(1);  //Screenshot
  });

  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&mouseDown]() {
    mouseDown = true;
  });
  can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&mouseDown]() {
    mouseDown = false;
  });

  while (can.isOpen()) {
    can.sleep();
    if(!paused) {
      for (int i = 0; i < IPF; i++) {
        if(mouseDown) {
          farm.addAnt(can.getMouseX(), can.getMouseY());
          bg->drawPixel(can.getMouseX() - WW/2, WH/2 - can.getMouseY(), WHITE);
        }
        farm.moveAnts();
      }
    }
    if(mouseDown) {
      farm.addAnt(can.getMouseX(), can.getMouseY());
      bg->drawPixel(can.getMouseX() - WW/2, WH/2 - can.getMouseY(), WHITE);
    }
  }
}

//Take command-line arguments for the width and height of the Canvas
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  Canvas c(-1, -1, w, h, "Conway's Game of Life", BLACK);
  c.run(conwayFunction);
}
