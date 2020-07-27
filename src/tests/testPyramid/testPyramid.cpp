/*
 * testPyramid.cpp
 *
 * Usage: ./testPyramid
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void pyramidFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,1),
        ColorFloat(0,1,0,1), ColorFloat(0,1,1,1), ColorFloat(1,0,0,1),
        ColorFloat(1,0,1,1), ColorFloat(1,1,0,1), ColorFloat(1,1,1,1),
        ColorFloat(0.5,0,0.5,1), ColorFloat(0,0.5,0.5,1), 
        ColorFloat(0.5,0.5,0,1), ColorFloat(0,0.5,1,1) };
    Pyramid * testPyramid = new Pyramid(0.0, 0.0, 0.0, 3, 100, 100, 0.0, 0.0, 45.0, ColorFloat(1,0,0,1));
    Pyramid * testPyramid2 = new Pyramid(-300, 0.0, 0.0, 5, 100, 100, 0.0, 0.0, 45.0, colors);
    Pyramid * testPyramid3 = new Pyramid(300, 0.0, 0.0, 10, 100, 100, 0.0, 0.0, 45.0, RED);
    // printf("%f\n", testPyramid2->getAlpha());
	// testPyramid2->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", testPyramid2->getAlpha());
	// testPyramid2->setColor(colors);
    // printf("%f\n", testPyramid2->getAlpha());
    can.add(testPyramid);
    can.add(testPyramid2);
    can.add(testPyramid3);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    bool boolean = false;
    while (can.isOpen()) {
        can.sleep();
        // testPyramid->setCenterX(sin(rotation)*200);
        // testPyramid->setCenterY(cos(rotation)*200);
        // testPyramid->setCenterZ(sin(rotation)*100);
        // testPyramid->setYaw(rotation*45);
        testPyramid->setPitch(rotation*45);
        testPyramid2->setPitch(rotation*45);
        testPyramid3->setPitch(rotation*45);
        // testPyramid->setRoll(sin(rotation*45)*100+101);
        // testPyramid->setHeight(sin(rotation)*100+101);
        // testPyramid->setRadius(sin(rotation)*100+101);
        // if(testPyramid->getHeight() >= 200) {
        //     delta = -5;
        // }
        // if(testPyramid->getHeight() <= 5) {
        //     delta = 5;
        // }
        // testPyramid->changeHeightBy(delta);
        // if(testPyramid->getRadius() >= 200) {
        //     delta = -5;
        // }
        // if(testPyramid->getRadius() <= 5) {
        //     delta = 5;
        // }
        // testPyramid->changeRadiusBy(delta);
        if (rotation*45 >= 360) {
            if (boolean) {
                testPyramid->setColor(RED);
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
    Canvas c(-1, -1, 1024, 620, "Basic Pyramid", BLACK);
    c.run(pyramidFunction);
}