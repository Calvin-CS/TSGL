/*
 * testSphere.cpp
 *
 * Usage: ./testSphere
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void sphereFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0,0,0,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Sphere * testSphere = new Sphere(225.0, 0.0, 0.0, 200, 0.0, 0.0, 0.0, colors);
    Sphere * testSphere2 = new Sphere(-225.0, 0.0, 0.0, 200, 0.0, 0.0, 0.0, RED);
    testSphere->setIsOutlined(true);
    testSphere2->setIsOutlined(true);
    // printf("%f\n", testSphere->getAlpha());
	// testSphere->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", testSphere->getAlpha());
	// testSphere->setColor(colors);
    // printf("%f\n", testSphere->getAlpha()); 
    can.add(testSphere);
    can.add(testSphere2);
    float rotation = 0.0f;
    // GLfloat delta = 5;
    bool boolean = true;
    while (can.isOpen()) {
        can.sleep();
        // testSphere->setCenterX(sin(rotation) * 100);
        // testSphere->setCenterY(cos(rotation) * 100);
        // testSphere->setCenterZ(sin(rotation) * 100);
        // testSphere->setYaw(rotation*45);
        // testSphere->setPitch(rotation*45);
        testSphere->setRoll(rotation*45);
        // testSphere->setRadius(cos(rotation) * 100 +101);
        // if(testSphere->getRadius() >= 200) {
        //     delta = -5;
        // }
        // if(testSphere->getRadius() <= 500) {
        //     delta = 5;
        // }
        // testSphere->changeRadiusBy(delta);
        // if (rotation*45 >= 360) {
        //     boolean = !boolean;
        //     rotation = 0;
        // }
        // printf("%f\n", rotation*45);
        if (rotation*45 >= 360) {
            if (boolean) {
                testSphere->setColor(RED);
            } else {
                testSphere->setColor(colors);
            }
            boolean = !boolean;
            rotation = 0;
        }

        rotation+=0.01;
    }

    delete testSphere;
    delete testSphere2;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Sphere", BLACK);
    c.run(sphereFunction);
}