/*
 * testRegularPolygon.cpp
 *
 * Usage: ./testRegularPolygon
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void rpFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8),
        ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0.5,0.5,1,0.8),
        ColorGLfloat(0.5,1,0.5,0.8), ColorGLfloat(0.5,1,1,0.8), ColorGLfloat(1,0.5,0.5,0.8),
        ColorGLfloat(1,0.5,1,0.8), ColorGLfloat(1,1,0.5,0.8), ColorGLfloat(0,0,0.5,0.8),
        ColorGLfloat(0,0.5,0,0.8), ColorGLfloat(0,0.5,0.5,0.8), ColorGLfloat(0.5,0,0,0.8),
        ColorGLfloat(0.5,0,0.5,0.8), ColorGLfloat(0.5,0.5,0,0.8), ColorGLfloat(0.5,0.5,0.5,0.8)};
    RegularPolygon * rp = new RegularPolygon(0,0,0,1,7,0,0,0,colors/* ColorGLfloat(1,0,0,1) */);
    // rp->setCenterX(2);
    // rp->setRotationPoint(0,0,0);
    can.add(rp);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // rp->setCenterX(sin(floatVal/90));
        // rp->setCenterY(sin(floatVal/90));
        // rp->setCenterZ(sin(floatVal/90));
        // rp->setYaw(floatVal);
        // rp->setPitch(floatVal);
        // rp->setRoll(floatVal);
        // rp->setRadius(sin(floatVal/90) + 3);
        // if (rp->getRadius() > 3 || rp->getRadius() < 1) {
        //     delta *= -1;
        //     rp->setEdgeColor(ColorGLfloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
        // }
        // rp->changeRadiusBy(delta);
        // if (delta > 0) {
        //     rp->setColor(colors);
        // } else {
        //     rp->setColor(ColorGLfloat(1,0,0,1));
        // }
        floatVal += 1;
    }

    delete rp;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic RegularPolygon");
    c.setBackgroundColor(BLACK);
    c.run(rpFunction);
}