/*
 * test2Dvs3D.cpp
 *
 * Usage: ./test2Dvs3D
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void contrastFunction(Canvas& can) {
    ColorFloat pyramidcolors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,1),
        ColorFloat(0,1,0,1), ColorFloat(0,1,1,1), ColorFloat(1,0,0,1),
        ColorFloat(1,0,1,1), ColorFloat(1,1,0,1), ColorFloat(1,1,1,1),
        ColorFloat(0.5,0,0.5,1), ColorFloat(0,0.5,0.5,1), 
        ColorFloat(0.5,0.5,0,1), ColorFloat(0,0.5,1,1) };
    ColorFloat triangle2Colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,1,1,1), ColorFloat(0,1,0,1) }; 
    ColorFloat triangle3Colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(1,0,1,1), ColorFloat(1,0,0,1) }; 
    ColorFloat triangle6Colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,1,1,1), ColorFloat(1,0,0,1) };
    Pyramid * testPyramid = new Pyramid(-200, 0.0, -100, 4, 200, 200, 0.0, 0.0, 45.0, pyramidcolors);
    // can.add(testPyramid);
    Triangle * triangle1 = new Triangle(139,66,0,350,-52,0,162,-200,0,0,0,0,pyramidcolors);
    // can.add(triangle1);
    Triangle * triangle2 = new Triangle(139,66,0,49,-52,0,162,-200,0,0,0,0,triangle2Colors);
    // can.add(triangle2);
    Triangle * triangle3 = new Triangle(139,66,0,350,-52,0,227,40,0,0,0,0,triangle3Colors);
    // can.add(triangle3);
    Triangle * triangle4 = new Triangle(265,64,0,331,-44,0,263,-194,0,0,0,0,pyramidcolors);
    // can.add(triangle4);
    Triangle * triangle5 = new Triangle(265,64,0,16,-70,0,263,-194,0,0,0,0,triangle2Colors);
    // can.add(triangle5);
    Triangle * triangle6 = new Triangle(265,64,0,16,-70,0,166,47,0,0,0,0,triangle6Colors);
    // can.add(triangle6);
    // testPyramid->setPitch(45);
    int stepsTaken = 0;
    while (can.isOpen()) {
        can.sleep();
        if (can.getFrameNumber() >= 1700 && stepsTaken < 1) {
            can.add(triangle1);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 1800 && stepsTaken < 2) {
            can.add(triangle2);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 1900 && stepsTaken < 3) {
            can.add(triangle3);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 2600 && stepsTaken < 4) {
            can.add(testPyramid);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 3300 && stepsTaken < 5) {
            can.remove(triangle1);
            can.remove(triangle2);
            can.remove(triangle3);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 3800 && stepsTaken < 6) {
            can.add(triangle4);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 3900 && stepsTaken < 7) {
            can.add(triangle5);
            stepsTaken++;
        }
        if (can.getFrameNumber() >= 4000 && stepsTaken < 8) {
            can.add(triangle6);
            stepsTaken++;
        }  
        if (can.getFrameNumber() >= 4400 && stepsTaken < 9) {
            if (testPyramid->getPitch() < 45) {
                testPyramid->changePitchBy(0.5);
            } else {
                testPyramid->setPitch(45);
                stepsTaken++;
            }
        }
    }

    delete testPyramid;
    delete triangle1;
    delete triangle2;
    delete triangle3;
    delete triangle4;
    delete triangle5;
    delete triangle6;

}

int main(int argc, char* argv[]) {
    Canvas c(-1, -1, 1024, 620, "3D Drawing vs. 2D Drawing");
    c.setBackgroundColor(BLACK);
    c.run(contrastFunction);
}