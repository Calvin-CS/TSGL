/*
 * testCube.cpp
 *
 * Usage: ./testCube
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cubeFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(1,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8) };
    Cube * testCube = new Cube(0.0, 0.0, 0.0, 200, 0.0, 45.0, 45.0, RED);
    Cube * testCube2 = new Cube(-300, 0.0, 0.0, 200, 0.0, 45.0, 45.0, colors);
    can.add(testCube);

    // printf("%f\n", testCube2->getAlpha());
	// testCube2->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", testCube2->getAlpha());
	// testCube2->setColor(colors);
    // printf("%f\n", testCube2->getAlpha());    
    can.add(testCube2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    bool boolean = false;

    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&testCube, &boolean]() {
        testCube->setIsOutlined(boolean);
        boolean = !boolean;
    });

    bool ss = false;
    while (can.isOpen()) {
        can.sleep();
        // testCube->setCenterX(sin(rotation)*200);
        // testCube->setCenterY(cos(rotation)*200);
        // testCube->setCenterZ(sin(rotation)*100);
        // testCube->setYaw(rotation*45);
        testCube->setPitch(rotation*45);
        // testCube->setRoll(rotation*45);
        // testCube->setSideLength(cos(rotation) * 100 +101);
        // if(testCube->getSideLength() >= 2) {
        //     delta = -5;
        // }
        // if(testCube->getSideLength() <= 5) {
        //     delta = 5;
        // }
        // testCube->changeSideLengthBy(delta);
        //testCube2->setRoll(rotation);
        // if (rotation*45 >= 360) {
        //     if (boolean) {
        //         testCube->setColor(RED);
        //     } else {
        //         testCube->setColor(colors);
        //     }
        //     boolean = !boolean;
        //     rotation = 0;
        // }
        if (can.getFrameNumber() > 50 && !ss) {
            can.takeScreenShot();
            ss = true;
        }
        rotation+=0.01;
    }

    delete testCube;
    delete testCube2;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Cube", BLACK);
    c.run(cubeFunction);
}