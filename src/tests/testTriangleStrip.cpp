/*
 * testTriangleStrip.cpp
 *
 * Usage: ./testTriangleStrip
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void triangleStripFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    float x[] = { 0,-50,50,-50,50,0 };
    float y[] = { -100,-50,-50,50,50,100 };
    float z[] = { 0,50,50,50,50,0 };
    TriangleStrip * ts = new TriangleStrip(0,0,0,6,x,y,z,0,0,0,colors/* RED */);
    // ts->setCenterX(2);
    ts->setRotationPoint(0,0,0);
    can.add(ts);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // ts->setCenterX(sin(floatVal/90) * 100);
        // ts->setCenterY(sin(floatVal/90) * 100);
        // ts->setCenterZ(sin(floatVal/90) * 100);
        // ts->setYaw(floatVal);
        ts->setPitch(floatVal);
        // ts->setRoll(floatVal);
        // if (floatVal < 200) {
        //     ts->setColor(colors);
        // } else {
        //     ts->setColor(RED);
        //     if (floatVal > 400) {
        //         floatVal = 0;
        //         ts->setEdgeColor(ColorFloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
        //     }
        // }
        floatVal += 1;
    }

    delete ts;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic TriangleStrip");
    c.setBackgroundColor(BLACK);
    c.run(triangleStripFunction);
}