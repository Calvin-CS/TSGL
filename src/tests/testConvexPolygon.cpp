/*
 * testConvexPolygon.cpp
 *
 * Usage: ./testConvexPolygon
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void convexPolygonFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    float x[] = { -50,-50, 0,25,50, 50, 25 };
    float y[] = { -50, 25,50,40,10,-10,-50 };
    ConvexPolygon * cp = new ConvexPolygon(0,0,0,7,x,y,0,0,0,colors);
    // cp->setCenterX(2);
    // cp->setRotationPoint(0,0,0);
    can.add(cp);
    float floatVal = 0.0f;
    while (can.isOpen()) {
        can.sleep();
        // cp->setCenterX(sin(floatVal/90) * 100);
        // cp->setCenterY(sin(floatVal/90) * 100);
        // cp->setCenterZ(sin(floatVal/90) * 100);
        // cp->setYaw(floatVal);
        // cp->setPitch(floatVal);
        // cp->setRoll(floatVal);
        // if (floatVal < 200) {
        //     cp->setColor(colors);
        // } else {
        //     cp->setColor(RED);
        //     if (floatVal > 400) {
        //         floatVal = 0;
        //     }
        // }
        floatVal += 1;
    }

    delete cp;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic ConvexPolygon", BLACK);
    c.run(convexPolygonFunction);
}