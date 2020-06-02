/*
 * test3DRotation.cpp
 *
 * Usage: ./test3DRotation
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cubeFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Cube * testCube = new Cube(2.0, 0.0, 0.0, 1, 0.0, 0.0, 0.0, colors);
    testCube->setRotationPoint(0,0,0);
    Prism * testPrism = new Prism(0.0, 0.0, 0.0, 3, 1, 1, 0.0, 0.0, 90.0, colors);
    Sphere * testSphere = new Sphere(3, 0, 0, 1, 0.0, 0.0, 0.0, colors);
    testSphere->setRotationPoint(testCube->getCenterX(), testCube->getCenterY(), testCube->getCenterZ());
    can.add(testCube);
    can.add(testPrism);
    can.add(testSphere);
    float rotation = 0.0f;
    // GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        testCube->setYaw(rotation);
        testPrism->setYaw(-rotation);
        testSphere->setRotationPoint(testCube->getCenterX(), testCube->getCenterY(), testCube->getCenterZ());
        testSphere->setCenter(testCube->getCenterX() + 1, testCube->getCenterY(), testCube->getCenterZ());
        testSphere->setYaw(-rotation);
        rotation += 1;
    }

    delete testCube;
    delete testPrism;
    delete testSphere;
}

int main(int argc, char* argv[]) {
    Canvas c(-1, -1, 1024, 620, "3D Rotation");
    c.setBackgroundColor(BLACK);
    c.run(cubeFunction);
}