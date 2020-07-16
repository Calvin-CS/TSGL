/*
 * testSquare.cpp
 *
 * Usage: ./testSquare
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void squareFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(1,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Square * square = new Square(0,0,0,200,0,0,0,colors);
    Square * square2 = new Square(-250,0,0,200,0,0,0,RED);
    // square->setCenterX(200);
    // square->setRotationPoint(0,0,0);
    can.add(square);
    can.add(square2);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    bool ss = false;
    while (can.isOpen()) {
        can.sleep();
        // square->setCenterX(sin(floatVal/90) * 100);
        // square->setCenterY(sin(floatVal/90) * 100);
        // square->setCenterZ(sin(floatVal/90) * 100);
        // square->setYaw(floatVal);
        // square->setPitch(floatVal);
        // square->setRoll(floatVal);
        // square->setSideLength(sin(floatVal/90) * 100 + 300);
        // if (square->getSideLength() > 300 || square->getSideLength() < 100) {
        //     delta *= -1;
        // }
        // square->changeSideLengthBy(delta);
        // if (delta > 0) {
        //     square->setColor(colors);
        // } else {
        //     square->setColor(RED);
        // }
        // if (can.getFrameNumber() > 50 && !ss) {
        //     can.takeScreenShot();
        //     ss = true;
        // }
        floatVal += 1;
    }

    delete square;
    delete square2;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Square", BLACK);
    c.run(squareFunction);
}