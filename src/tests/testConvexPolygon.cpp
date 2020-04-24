/*
 * testConvexPolygon.cpp
 *
 * Usage: ./testConvexPolygon
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void convexPolygonFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8),
        ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0.5,0.5,1,0.8),
        ColorGLfloat(0.5,1,0.5,0.8), ColorGLfloat(0.5,1,1,0.8), ColorGLfloat(1,0.5,0.5,0.8),
        ColorGLfloat(1,0.5,1,0.8), ColorGLfloat(1,1,0.5,0.8), ColorGLfloat(0,0,0.5,0.8),
        ColorGLfloat(0,0.5,0,0.8), ColorGLfloat(0,0.5,0.5,0.8), ColorGLfloat(0.5,0,0,0.8),
        ColorGLfloat(0.5,0,0.5,0.8), ColorGLfloat(0.5,0.5,0,0.8), ColorGLfloat(0.5,0.5,0.5,0.8)};
    float x[] = { -0.5,-0.5,0  ,0.25,0.5,0.5,0.25 };
    float y[] = { -0.5,0.25,0.5,0.4 ,0.1,-0.1,-0.5 };
    ConvexPolygon * cp = new ConvexPolygon(0,0,0,7,x,y,0,0,0,colors/* ColorGLfloat(1,0,0,1) */);
    // cp->setCenterX(2);
    // cp->setRotationPoint(0,0,0);
    can.add(cp);
    float floatVal = 0.0f;
    while (can.isOpen()) {
        can.sleep();
        // cp->setCenterX(sin(floatVal/90));
        // cp->setCenterY(sin(floatVal/90));
        // cp->setCenterZ(sin(floatVal/90));
        // cp->setYaw(floatVal);
        // cp->setPitch(floatVal);
        // cp->setRoll(floatVal);
        // if (floatVal < 200) {
        //     cp->setColor(colors);
        // } else {
        //     cp->setColor(ColorGLfloat(1,0,0,1));
        //     if (floatVal > 400) {
        //         floatVal = 0;
        //         cp->setEdgeColor(ColorGLfloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
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
    Canvas c(-1, -1, 1024, 620, "Basic ConvexPolygon");
    c.setBackgroundColor(BLACK);
    c.run(convexPolygonFunction);
}