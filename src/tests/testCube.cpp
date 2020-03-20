/*
 * testCube.cpp
 *
 * Usage: ./testCube
 */

#include <tsgl.h>

using namespace tsgl;

void cubeFunction(Canvas& can) {
    Cube * testCube = new Cube(0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, ColorGLfloat(0,0,0,1));
    printf("Cube completed.\n");
    // can.add(testCube);
    // while (can.isOpen()) {
    //     can.sleep();
    // }
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Fancy Rectangles");
    c.setBackgroundColor(BLACK);
    c.run(cubeFunction);
}