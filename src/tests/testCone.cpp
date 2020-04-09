/*
 * testCone.cpp
 *
 * Usage: ./testCone
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void coneFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0.5,0.5,0.5,1), ColorGLfloat(0,0,1,1),
        ColorGLfloat(0,1,0,1), ColorGLfloat(0,1,1,1), ColorGLfloat(1,0,0,1),
        ColorGLfloat(1,0,1,1), ColorGLfloat(1,1,0,1), ColorGLfloat(1,1,1,1),
        ColorGLfloat(0.5,0,0.5,1), ColorGLfloat(0,0.5,0.5,1), 
        ColorGLfloat(0.5,0.5,0,1), ColorGLfloat(0,0.5,1,1) };
    Cone * testCone = new Cone(0.0, 0.0, 0.0, 1, 1, 0.0, 0.0, 0.0, colors);
    // Cone * testCone2 = new Cone(-3.0, 0.0, 0.0, 2, 0.0, 45.0, 45.0, colors);
    can.add(testCone);
    // can.add(testCone2);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // testCone->setCenterX(sin(rotation)*2);
        // testCone->setCenterY(cos(rotation)*2);
        // testCone->setCenterZ(sin(rotation));
        // testCone->setYaw(rotation*45);
        // testCone->setPitch(rotation*45);
        // testCone->setRoll(rota tion*45);
        // testCone->setHeight(sin(rotation)+1.01);
        // testCone->setRadius(sin(rotation)+1.01);
        // if(testCone->getHeight() >= 2) {
        //     delta = -0.05;
        // }
        // if(testCone->getHeight() <= 0.05) {
        //     delta = 0.05;
        // }
        // testCone->changeHeightBy(delta);
        if(testCone->getRadius() >= 2) {
            delta = -0.05;
        }
        if(testCone->getRadius() <= 0.05) {
            delta = 0.05;
        }
        testCone->changeRadiusBy(delta);
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