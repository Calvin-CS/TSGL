/*
 * testCuboid.cpp
 *
 * Usage: ./testCuboid
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cuboidFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0,0,0,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8) };
    Cuboid * testCuboid = new Cuboid(0.0, 0.0, 0.0, 1, 4, 2, 0.0, 45.0, 0.0, ColorGLfloat(1,0,0,1));
    // Cuboid * testCuboid2 = new Cuboid(-3.0, 0.0, 0.0, 1, 3, 2, 0.0, 0.0, 0.0, colors);
    can.add(testCuboid);
    // can.add(testCuboid2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // testCuboid->setCenterX(sin(rotation)*2);
        // testCuboid->setCenterY(cos(rotation)*2);
        // testCuboid->setCenterZ(sin(rotation));
        // testCuboid->setYaw(rotation*45);
        testCuboid->setPitch(rotation*45);
        // testCuboid->setRoll(rotation*45);
        // testCuboid->setWidth(cos(rotation)+1.01);
        // testCuboid->setHeight(cos(rotation)+3.01);
        // testCuboid->setLength(cos(rotation)+2.01);
        // if(testCuboid->getWidth() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCuboid->getWidth() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCuboid->changeWidthBy(delta);

        // if(testCuboid->getHeight() >= 5) {
        //     delta = -0.05;
        // }
        // if(testCuboid->getHeight() <= 3) {
        //     delta = 0.05;
        // }
        // testCuboid->changeHeightBy(delta);

        // if(testCuboid->getLength() >= 3) {
        //     delta = -0.05;
        // }
        // if(testCuboid->getLength() <= 1) {
        //     delta = 0.05;
        // }
        // testCuboid->changeLengthBy(delta);
        rotation+=0.01;
    }

    delete testCuboid;
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