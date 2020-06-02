/*
 * testEllipsoid.cpp
 *
 * Usage: ./testEllipsoid
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void ellipsoidFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Ellipsoid * testEllipsoid = new Ellipsoid(0.0, 0.0, 0.0, 2, 2, 2, 0.0, 0.0, 0.0, colors/* ColorFloat(1,0,0,1) */);
    // Ellipsoid * testEllipsoid2 = new Ellipsoid(-2.0, 0.0, 0.0, 1.5, 1.5, 1.5, 0.0, 0.0, 0.0, colors);
    can.add(testEllipsoid);
    // can.add(testEllipsoid2);
    float rotation = 0.0f;
    // GLfloat delta = 0.05;
    bool boolean = true;
    // testEllipsoid->setXRadius(1.5);
    // testEllipsoid->setYRadius(1.5);
    // testEllipsoid->setZRadius(1.5);
    // testEllipsoid->changeXRadiusBy(1.4);
    // testEllipsoid->changeYRadiusBy(1.4);
    // testEllipsoid->changeZRadiusBy(1.4);
    while (can.isOpen()) {
        can.sleep();
        // testEllipsoid->setCenterX(sin(rotation));
        // testEllipsoid->setCenterY(cos(rotation));
        // testEllipsoid->setCenterZ(sin(rotation));
        // testEllipsoid->setYaw(rotation*45);
        testEllipsoid->setPitch(rotation*45);
        // testEllipsoid2->setPitch(rotation*45);
        // testEllipsoid->setRoll(rotation*45);
        // testEllipsoid->setXRadius(cos(rotation)+1.01);
        // testEllipsoid->setYRadius(sin(rotation)+2.01);
        // testEllipsoid->setZRadius(sin(rotation)+3.01);
        // if(testEllipsoid->getXRadius() >= 2) {
        //     delta = -0.05;
        // }
        // if(testEllipsoid->getXRadius() <= 0.05) {
        //     delta = 0.05;
        // }
        // testEllipsoid->changeXRadiusBy(delta);

        // if(testEllipsoid->getYRadius() >= 5) {
        //     delta = -0.05;
        // }
        // if(testEllipsoid->getYRadius() <= 3) {
        //     delta = 0.05;
        // }
        // testEllipsoid->changeYRadiusBy(delta);

        // if(testEllipsoid->getZRadius() >= 3) {
        //     delta = -0.05;
        // }
        // if(testEllipsoid->getZRadius() <= 1) {
        //     delta = 0.05;
        // }
        // testEllipsoid->changeZRadiusBy(delta);
        // if (rotation*45 >= 360) {
        //     testEllipsoid->displayOutlineEdges(boolean);
        //     boolean = !boolean;
        //     rotation = 0;
        // }
        if (rotation*45 >= 360) {
            if (boolean) {
                testEllipsoid->setColor(RED);
            } else {
                testEllipsoid->setColor(colors);
            }
            boolean = !boolean;
            rotation = 0;
        }
        rotation+=0.01;
    }

    delete testEllipsoid;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Ellipsoid");
    c.setBackgroundColor(BLACK);
    c.run(ellipsoidFunction);
}