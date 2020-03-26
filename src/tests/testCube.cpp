/*
 * testCube.cpp
 *
 * Usage: ./testCube
 */

#include <tsgl.h>

using namespace tsgl;

void cubeFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0,0,0,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8) };
    Cube * testCube = new Cube(0.0, 0.0, 0.0, 2, 0.0, 0.0, 0.0, colors);
    // Cube * testCube2 = new Cube(-3.0, 0.0, 0.0, 2, 0.0, 0.0, 0.0, colors);
    can.add(testCube);
    // can.add(testCube2);
    float rotation = 0.0f;
    while (can.isOpen()) {
        can.sleep();
        testCube->setCenterZ(rotation);
        //testCube2->setRoll(rotation);
        rotation+=0.1;
        printf("CenterZ %f\n", testCube->getCenterZ());
        // printf("Roll %f\n", testCube2->getRoll());
    }
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