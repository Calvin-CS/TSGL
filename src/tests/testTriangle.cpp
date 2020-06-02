/*
 * testTriangle.cpp
 *
 * Usage: ./testTriangle
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void triangleFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Triangle * triangle = new Triangle(-0.5,-0.5,0,0,0.5,0,0.5,-0.5,0,0,0,0,colors/* ColorFloat(1,0,0,1) */);
    // triangle->setCenterX(2);
    // triangle->setRotationPoint(0,0,0);
    can.add(triangle);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // triangle->setCenterX(sin(floatVal/90));
        // triangle->setCenterY(sin(floatVal/90));
        // triangle->setCenterZ(sin(floatVal/90));
        // triangle->setYaw(floatVal);
        // triangle->setPitch(floatVal);
        // triangle->setRoll(floatVal);
        if (floatVal < 200) {
            triangle->setColor(colors);
        } else {
            triangle->setColor(RED);
            if (floatVal > 400) {
                floatVal = 0;
                triangle->setEdgeColor(ColorFloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
            }
        }
        floatVal += 1;
    }

    delete triangle;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Triangle");
    c.setBackgroundColor(BLACK);
    c.run(triangleFunction);
}