/*
 * testBG.cpp
 *
 * Usage: ./testBG
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void squareFunction(Canvas& can) {
    Background * bg = new Background(0,0,0, can.getWindowWidth(), can.getWindowHeight(), 0,0,0,WHITE);
    // printf("draw square\n");
    // Cube * cube = new Cube(-250,0,0,100,0,45,45,YELLOW);
    // can.add(cube);
    bg->drawSquare(0);
    can.setBackground(bg);
    can.add(bg);
    while (can.isOpen()) {
        can.sleep();
        bg->setPitch(can.getFrameNumber()/50);
        // cube->setCenterZ(500 * cos((float) can.getFrameNumber()/50));
    }
    // delete cube;
    delete bg;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Square");
    c.setBackgroundColor(BLACK);
    c.run(squareFunction);
}