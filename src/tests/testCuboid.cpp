/*
 * testCuboid.cpp
 *
 * Usage: ./testCuboid
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cuboidFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8) };
    Cuboid * testCuboid = new Cuboid(0.0, 0.0, 0.0, 100, 400, 200, 0.0, 45.0, 0.0, ColorFloat(1,0,0,1));
    Cuboid * testCuboid2 = new Cuboid(-300, 0.0, 0.0, 100, 400, 200, 0.0, 0.0, 0.0, colors);
    can.add(testCuboid);
    can.add(testCuboid2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    bool boolean = false;;
    while (can.isOpen()) {
        can.sleep();
        // testCuboid->setCenterX(sin(rotation)*200);
        // testCuboid->setCenterY(cos(rotation)*200);
        // testCuboid->setCenterZ(sin(rotation)*100);
        // testCuboid->setYaw(rotation*45);
        testCuboid->setPitch(rotation*45);
        // testCuboid->setRoll(rotation*45);
        // testCuboid->setWidth(cos(rotation)*100+101);
        // testCuboid->setHeight(cos(rotation)*100+301);
        // testCuboid->setLength(cos(rotation)*100+201);
        // if(testCuboid->getWidth() >= 200) {
        //     delta = -5;
        // }
        // if(testCuboid->getWidth() <= 5) {
        //     delta = 5;
        // }
        // testCuboid->changeWidthBy(delta);

        // if(testCuboid->getHeight() >= 500) {
        //     delta = -5;
        // }
        // if(testCuboid->getHeight() <= 300) {
        //     delta = 5;
        // }
        // testCuboid->changeHeightBy(delta);

        // if(testCuboid->getLength() >= 300) {
        //     delta = -5;
        // }
        // if(testCuboid->getLength() <= 100) {
        //     delta = 5;
        // }
        // testCuboid->changeLengthBy(delta);
        if (rotation*45 >= 360) {
            if (boolean) {
                testCuboid->setColor(RED);
            } else {
                testCuboid->setColor(colors);
            }
            boolean = !boolean;
            rotation = 0;
        }
        rotation+=0.01;
    }

    delete testCuboid;
    delete testCuboid2;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Cuboid");
    c.setBackgroundColor(BLACK);
    c.run(cuboidFunction);
}