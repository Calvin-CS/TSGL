/*
 * testCube.cpp
 *
 * Usage: ./testCube
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cubeFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0,0,0,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8) };
    Cube * testCube = new Cube(0.0, 0.0, 0.0, 2, 0.0, 45.0, 45.0, ColorGLfloat(1,0,0,1));
    Cube * testCube2 = new Cube(-3.0, 0.0, 0.0, 2, 0.0, 45.0, 45.0, colors);
    can.add(testCube);
    can.add(testCube2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // testCube->setCenterX(sin(rotation)*2);
        // testCube->setCenterY(cos(rotation)*2);
        // testCube->setCenterZ(sin(rotation));
        // testCube->setYaw(rotation*45);
        testCube->setPitch(rotation*45);
        // testCube->setRoll(rotation*45);
        // testCube->setSideLength(cos(rotation)+1.01);
        // if(testCube->getSideLength() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCube->getSideLength() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCube->changeSideLengthBy(delta);
        //testCube2->setRoll(rotation);
        rotation+=0.01;
        // printf("Roll %f\n", testCube2->getRoll());
    }

    delete testCube;
    delete testCube2;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Cube");
    c.setBackgroundColor(BLACK);
    c.run(cubeFunction);
}