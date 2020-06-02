/*
 * testCone.cpp
 *
 * Usage: ./testCone
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void coneFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,1),
        ColorFloat(0,1,0,1), ColorFloat(0,1,1,1), ColorFloat(1,0,0,1),
        ColorFloat(1,0,1,1), ColorFloat(1,1,0,1), ColorFloat(1,1,1,1),
        ColorFloat(0.5,0,0.5,1), ColorFloat(0,0.5,0.5,1), 
        ColorFloat(0.5,0.5,0,1), ColorFloat(0,0.5,1,1), 
        ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,1),
        ColorFloat(0,1,0,1), ColorFloat(0,1,1,1), ColorFloat(1,0,0,1),
        ColorFloat(1,0,1,1), ColorFloat(1,1,0,1), ColorFloat(1,1,1,1),
        ColorFloat(0.5,0,0.5,1), ColorFloat(0,0.5,0.5,1), 
        ColorFloat(0.5,0.5,0,1), ColorFloat(0,0.5,1,1), 
        ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,1) };
    Cone * testCone = new Cone(0.0, 0.0, 0.0, 1, 1, 0.0, 0.0, 0.0, ColorFloat(1,0,0,1));
    // Cone * testCone2 = new Cone(-3.0, 0.0, 0.0, 2, 0.0, 45.0, 45.0, colors);
    can.add(testCone);
    // can.add(testCone2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    bool boolean = false;
    while (can.isOpen()) {
        can.sleep();
        // testCone->setCenterX(sin(rotation)*2);
        // testCone->setCenterY(cos(rotation)*2);
        // testCone->setCenterZ(sin(rotation));
        // testCone->setYaw(rotation*45);
        // testCone->setPitch(rotation*45);
        testCone->setRoll(rotation*45);
        // testCone->setHeight(sin(rotation)+1.01);
        // testCone->setRadius(sin(rotation)+1.01);
        // if(testCone->getHeight() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCone->getHeight() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCone->changeHeightBy(delta);
        // if(testCone->getRadius() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCone->getRadius() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCone->changeRadiusBy(delta);
        if (rotation*45 >= 360) {
            if (boolean) {
                testCone->setColor(RED);
            } else {
                testCone->setColor(colors);
            }
            boolean = !boolean;
            rotation = 0;
        }
        rotation+=0.01;
    }

    delete testCone;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Cone");
    c.setBackgroundColor(BLACK);
    c.run(coneFunction);
}