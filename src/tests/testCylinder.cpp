/*
 * testCylinder.cpp
 *
 * Usage: ./testCylinder
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void cylinderFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0.5,0.5,0.5,1), ColorGLfloat(0,0,1,1),
        ColorGLfloat(0,1,0,1), ColorGLfloat(0,1,1,1), ColorGLfloat(1,0,0,1),
        ColorGLfloat(1,0,1,1), ColorGLfloat(1,1,0,1), ColorGLfloat(1,1,1,1),
        ColorGLfloat(0.5,0,0.5,1), ColorGLfloat(0,0.5,0.5,1), 
        ColorGLfloat(0.5,0.5,0,1), ColorGLfloat(0,0.5,1,1) };
    Cylinder * testCylinder = new Cylinder(0.0, 0.0, 0.0, 1, 1, 0.0, 0.0, 45.0, ColorGLfloat(1,0,0,1));
    // Cylinder * testCylinder2 = new Cylinder(-3.0, 0.0, 0.0, 2, 0.0, 45.0, 45.0, colors);
    can.add(testCylinder);
    // can.add(testCylinder2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // testCylinder->setCenterX(sin(rotation)*2);
        // testCylinder->setCenterY(cos(rotation)*2);
        // testCylinder->setCenterZ(sin(rotation));
        // testCylinder->setYaw(rotation*45);
        testCylinder->setPitch(rotation*45);
        // testCylinder->setRoll(rotation*45);
        // testCylinder->setHeight(sin(rotation)+1.01);
        // testCylinder->setRadius(sin(rotation)+1.01);
        // if(testCylinder->getHeight() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCylinder->getHeight() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCylinder->changeHeightBy(delta);
        // if(testCylinder->getRadius() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCylinder->getRadius() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCylinder->changeRadiusBy(delta);
        rotation+=0.01;
    }

    delete testCylinder;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Cylinder");
    c.setBackgroundColor(BLACK);
    c.run(cylinderFunction);
}