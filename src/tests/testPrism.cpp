/*
 * testPrism.cpp
 *
 * Usage: ./testPrism
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void prismFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,1),
        ColorFloat(0,1,0,1), ColorFloat(0,1,1,1), ColorFloat(1,0,0,1),
        ColorFloat(1,0,1,1), ColorFloat(1,1,0,1), ColorFloat(1,1,1,1),
        ColorFloat(0.5,0,0.5,1), ColorFloat(0,0.5,0.5,1), 
        ColorFloat(0.5,0.5,0,1), ColorFloat(0,0.5,1,1) };
    Prism * testPrism = new Prism(0.0, 0.0, 0.0, 3, 100, 100, 0.0, 0.0, 45.0, ColorFloat(1,0,0,1));
    Prism * testPrism2 = new Prism(-300, 0.0, 0.0, 5, 100, 100, 0.0, 0.0, 45.0, colors);
    Prism * testPrism3 = new Prism(300, 0.0, 0.0, 8, 100, 100, 0.0, 0.0, 45.0, colors);

    // printf("%f\n", testPrism2->getAlpha());
	// testPrism2->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", testPrism2->getAlpha());
	// testPrism2->setColor(colors);
    // printf("%f\n", testPrism2->getAlpha()); 
    can.add(testPrism);
    can.add(testPrism2);
    can.add(testPrism3);
    float rotation = 0.0f;
    GLfloat delta = 0.05;
    bool boolean = false;
    while (can.isOpen()) {
        can.sleep();
        // testPrism->setCenterX(sin(rotation)*200);
        // testPrism->setCenterY(cos(rotation)*200);
        // testPrism->setCenterZ(sin(rotation)*100);
        // testPrism->setYaw(rotation*45);
        testPrism->setPitch(rotation*45);
        // testPrism->setRoll(rotation*45);
        // testPrism->setHeight(sin(rotation)*100+101);
        // testPrism->setRadius(sin(rotation)*100+101);
        // if(testPrism->getHeight() >= 200) {
        //     delta = -5;
        // }
        // if(testPrism->getHeight() <= 5) {
        //     delta = 5;
        // }
        // testPrism->changeHeightBy(delta);
        // if(testPrism->getRadius() >= 200) {
        //     delta = -5;
        // }
        // if(testPrism->getRadius() <= 5) {
        //     delta = 5;
        // }
        // testPrism->changeRadiusBy(delta);
        // if (rotation*45 >= 360) {
        //     rotation = 0;
        // }
        if (rotation*45 >= 360) {
            if (boolean) {
                testPrism->setColor(RED);
            } else {
                testPrism->setColor(colors);
            }
            boolean = !boolean;
            rotation = 0;
        }
        rotation+=0.01;
    }

    delete testPrism;
    delete testPrism2;
    delete testPrism3;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Prism", BLACK);
    c.run(prismFunction);
}