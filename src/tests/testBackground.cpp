/*
 * testBackground.cpp tests the core functions of TSGL::Background
 *
 * Usage: ./testBackground
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void backgroundFunction(Canvas& can) {
    Background * bg = can.getBackground();
    Cube * cube = new Cube(-250,0,0,100,0,45,45,YELLOW);
    float x = 0;

    can.bindToButton(TSGL_LEFT, TSGL_PRESS, [&cube]() {
        cube->changeXBy(-10);
    });

    can.bindToButton(TSGL_RIGHT, TSGL_PRESS, [&cube]() {
        cube->changeXBy(10);
    });

    can.bindToButton(TSGL_UP, TSGL_PRESS, [&cube]() {
        cube->changeYBy(10);
    });

    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [&cube]() {
        cube->changeYBy(-10);
    });

    can.bindToButton(TSGL_ENTER, TSGL_PRESS, [&cube]() {
        cube->changeZBy(-50);
    });

    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&cube]() {
        cube->changeZBy(50);
    });

    // bg->drawSquare(0,0,0,200,0,0,0,RED);

    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&bg, &can]() {
        bg->setClearColor(Colors::randomColor());
        // can.setBackgroundColor(Colors::randomColor());
        bg->clear();
        // can.clearBackground();
        // printf("%f, %f, %f, %f\n", can.getBackgroundColor().R, can.getBackgroundColor().G, can.getBackgroundColor().B, can.getBackgroundColor().A);
        // printf("%f, %f, %f, %f\n", bg->getClearColor().R, bg->getClearColor().G, bg->getClearColor().B, bg->getClearColor().A);
    });

    can.add(cube);
    while (can.isOpen()) {
        can.sleep();
        // bg->clear();
        x = 200 * sin( (float) can.getFrameNumber()/90);
        bg->drawSquare(x,0,0,200,0,0,0,RED, true);
        // cube->setCenterZ(500 * cos((float) can.getFrameNumber()/50));
    }
    delete cube;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Background", BLUE);
    c.run(backgroundFunction);
}