/*
 * testPyramid.cpp
 *
 * Usage: ./testPyramid
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void pyramidFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0.5,0.5,0.5,1), ColorGLfloat(0,0,1,1),
        ColorGLfloat(0,1,0,1), ColorGLfloat(0,1,1,1), ColorGLfloat(1,0,0,1),
        ColorGLfloat(1,0,1,1), ColorGLfloat(1,1,0,1), ColorGLfloat(1,1,1,1),
        ColorGLfloat(0.5,0,0.5,1), ColorGLfloat(0,0.5,0.5,1), 
        ColorGLfloat(0.5,0.5,0,1), ColorGLfloat(0,0.5,1,1) };
    Pyramid * testPyramid = new Pyramid(0.0, 0.0, 0.0, 3, 1, 1, 0.0, 0.0, 45.0, ColorGLfloat(1,0,0,1));
    Pyramid * testPyramid2 = new Pyramid(-3.0, 0.0, 0.0, 5, 1, 1, 0.0, 0.0, 45.0, colors);
    Pyramid * testPyramid3 = new Pyramid(3.0, 0.0, 0.0, 8, 1, 1, 0.0, 0.0, 45.0, colors);
    can.add(testPyramid);
    can.add(testPyramid2);
    can.add(testPyramid3);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    bool boolean = false;
    while (can.isOpen()) {
        can.sleep();
        // testPyramid->setCenterX(sin(rotation)*2);
        // testPyramid->setCenterY(cos(rotation)*2);
        // testPyramid->setCenterZ(sin(rotation));
        // testPyramid->setYaw(rotation*45);
        testPyramid->setPitch(rotation*45);
        testPyramid2->setPitch(rotation*45);
        testPyramid3->setPitch(rotation*45);
        // testPyramid->setRoll(rotation*45);
        // testPyramid->setHeight(sin(rotation)+1.01);
        // testPyramid->setRadius(sin(rotation)+1.01);
        // if(testPyramid->getHeight() >= 2) {
        //     delta = -0.05;
        // }
        // if(testPyramid->getHeight() <= 0.05) {
        //     delta = 0.05;
        // }
        // testPyramid->changeHeightBy(delta);
        // if(testPyramid->getRadius() >= 2) {
        //     delta = -0.05;
        // }
        // if(testPyramid->getRadius() <= 0.05) {
        //     delta = 0.05;
        // }
        // testPyramid->changeRadiusBy(delta);
        if (rotation*45 >= 360) {
            if (boolean) {
                testPyramid->setColor(ColorGLfloat(1,0,0,1));
            } else {
                testPyramid->setColor(colors);
            }
            boolean = !boolean;
            rotation = 0;
        }
        rotation+=0.01;
    }

    delete testPyramid;
    delete testPyramid2;
    delete testPyramid3;
}

int main(int argc, char* argv[]) {
    Canvas c(-1, -1, 1024, 620, "Basic Pyramid");
    c.setBackgroundColor(BLACK);
    c.run(pyramidFunction);
}