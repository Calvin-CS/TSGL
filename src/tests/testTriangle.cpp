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
    Triangle * triangle = new Triangle(-50,-50,0,0,50,0,50,-50,0,0,0,0,colors);
    Triangle * triangle2 = new Triangle(-150, -200,0,-200,100,0,-100,0,0,0,0,0,RED);
    // triangle->setCenterX(200);
    // triangle->setRotationPoint(0,0,0);
    can.add(triangle);
    can.add(triangle2);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // triangle->setCenterX(sin(floatVal/90) * 100);
        // triangle->setCenterY(sin(floatVal/90) * 100);
        // triangle->setCenterZ(sin(floatVal/90) * 100);
        // triangle->setYaw(floatVal);
        // triangle->setPitch(floatVal);
        // triangle->setRoll(floatVal);
        if (floatVal < 200) {
            triangle->setColor(colors);
        } else {
            triangle->setColor(RED);
            if (floatVal > 400) {
                floatVal = 0;
            }
        }
        floatVal += 1;
    }

    delete triangle;
    delete triangle2;
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