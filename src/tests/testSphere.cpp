/*
 * testSphere.cpp
 *
 * Usage: ./testSphere
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void sphereFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0,0,0,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8),
        ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0.5,0.5,1,0.8),
        ColorGLfloat(0.5,1,0.5,0.8), ColorGLfloat(0.5,1,1,0.8), ColorGLfloat(1,0.5,0.5,0.8),
        ColorGLfloat(1,0.5,1,0.8), ColorGLfloat(1,1,0.5,0.8), ColorGLfloat(0,0,0.5,0.8),
        ColorGLfloat(0,0.5,0,0.8), ColorGLfloat(0,0.5,0.5,0.8), ColorGLfloat(0.5,0,0,0.8),
        ColorGLfloat(0.5,0,0.5,0.8), ColorGLfloat(0.5,0.5,0,0.8), ColorGLfloat(0.5,0.5,0.5,0.8)};
    Sphere * testSphere = new Sphere(0.0, 0.0, 0.0, 1.5, 0.0, 0.0, 0.0, colors);
    can.add(testSphere);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // testSphere->setCenterX(sin(rotation));
        // testSphere->setCenterY(cos(rotation));
        // testSphere->setCenterZ(sin(rotation));
        // testSphere->setYaw(rotation*45);
        // testSphere->setPitch(rotation*45);
        // testSphere->setRoll(rotation*45);
        // testSphere->setRadius(cos(rotation)+1.01);
        // if(testSphere->getRadius() >= 2) {
        //     delta = -0.05;
        // }
        // if(testSphere->getRadius() <= 0.05) {
        //     delta = 0.05;
        // }
        // testSphere->changeRadiusBy(delta);
        rotation+=0.01;
    }

    delete testSphere;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Sphere");
    c.setBackgroundColor(BLACK);
    c.run(sphereFunction);
}