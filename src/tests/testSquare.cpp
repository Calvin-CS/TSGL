/*
 * testSquare.cpp
 *
 * Usage: ./testSquare
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void squareFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Square * square = new Square(0,0,0,1,0,0,0,colors/* ColorFloat(1,0,0,1) */);
    // square->setCenterX(2);
    // square->setRotationPoint(0,0,0);
    can.add(square);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // square->setCenterX(sin(floatVal/90));
        // square->setCenterY(sin(floatVal/90));
        // square->setCenterZ(sin(floatVal/90));
        // square->setYaw(floatVal);
        // square->setPitch(floatVal);
        // square->setRoll(floatVal);
        // square->setSideLength(sin(floatVal/90) + 3);
        // if (square->getSideLength() > 3 || square->getSideLength() < 1) {
        //     delta *= -1;
        //     // square->setEdgeColor(ColorFloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
        // }
        // square->changeSideLengthBy(delta);
        // if (delta > 0) {
        //     square->setColor(colors);
        // } else {
        //     square->setColor(RED);
        // }
        floatVal += 1;
    }

    delete square;
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