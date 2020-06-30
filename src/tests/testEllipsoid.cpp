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
    Ellipsoid * testEllipsoid = new Ellipsoid(200.0, 0.0, 0.0, 200, 200, 200, 0.0, 0.0, 0.0, colors);
    // testEllipsoid->setIsFilled(false);
    Ellipsoid * testEllipsoid2 = new Ellipsoid(-200, 0.0, 0.0, 150, 200, 100, 0.0, 0.0, 0.0, RED);
    testEllipsoid2->setOutlineColor(BLUE);
    can.add(testEllipsoid);
    can.add(testEllipsoid2);
    float rotation = 0.0f;
    // GLfloat delta = 0.05;
    bool boolean = true;
    bool b2 = true;

    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&testEllipsoid, &testEllipsoid2, &b2]() {
        testEllipsoid->setIsOutlined(b2);
        testEllipsoid2->setIsOutlined(b2);
        b2 = !b2;
    });

    // testEllipsoid->setXRadius(150);
    // testEllipsoid->setYRadius(150);
    // testEllipsoid->setZRadius(150);
    // testEllipsoid->changeXRadiusBy(140);
    // testEllipsoid->changeYRadiusBy(140);
    // testEllipsoid->changeZRadiusBy(140);
    while (can.isOpen()) {
        can.sleep();
        // testEllipsoid->setCenterX(sin(rotation));
        // testEllipsoid->setCenterY(cos(rotation));
        // testEllipsoid->setCenterZ(sin(rotation));
        // testEllipsoid->setYaw(rotation*45);
        testEllipsoid->setPitch(rotation*45);
        testEllipsoid2->setPitch(rotation*45);
        // if(boolean)
        // testEllipsoid->setRoll(rotation*45);
        // testEllipsoid->setXRadius(cos(rotation) * 100 +101);
        // testEllipsoid->setYRadius(sin(rotation) * 100 +201);
        // testEllipsoid->setZRadius(sin(rotation) * 100 +301);
        // if(testEllipsoid->getXRadius() >= 200) {
        //     delta = -5;
        // }
        // if(testEllipsoid->getXRadius() <= 5) {
        //     delta = 5;
        // }
        // testEllipsoid->changeXRadiusBy(delta);

        // if(testEllipsoid->getYRadius() >= 500) {
        //     delta = -5;
        // }
        // if(testEllipsoid->getYRadius() <= 300) {
        //     delta = 5;
        // }
        // testEllipsoid->changeYRadiusBy(delta);

        // if(testEllipsoid->getZRadius() >= 300) {
        //     delta = -5;
        // }
        // if(testEllipsoid->getZRadius() <= 100) {
        //     delta = 5;
        // }
        // testEllipsoid->changeZRadiusBy(delta);
        // if (rotation*45 >= 360) {
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