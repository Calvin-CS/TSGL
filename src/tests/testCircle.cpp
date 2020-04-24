/*
 * testCircle.cpp
 *
 * Usage: ./testCircle
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void circleFunction(Canvas& can) {
    ColorGLfloat colors[] = { ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0,0,1,0.8),
        ColorGLfloat(0,1,0,0.8), ColorGLfloat(0,1,1,0.8), ColorGLfloat(1,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8),
        ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0.5,0.5,1,0.8),
        ColorGLfloat(0.5,1,0.5,0.8), ColorGLfloat(0.5,1,1,0.8), ColorGLfloat(1,0.5,0.5,0.8),
        ColorGLfloat(1,0.5,1,0.8), ColorGLfloat(1,1,0.5,0.8), ColorGLfloat(0,0,0.5,0.8),
        ColorGLfloat(0,0.5,0,0.8), ColorGLfloat(0,0.5,0.5,0.8), ColorGLfloat(0.5,0,0,0.8),
        ColorGLfloat(1,0,1,0.8), ColorGLfloat(1,1,0,0.8), ColorGLfloat(1,1,1,0.8),
        ColorGLfloat(0.5,0.5,0.5,0.8), ColorGLfloat(0.5,0.5,1,0.8),
        ColorGLfloat(0.5,1,0.5,0.8), ColorGLfloat(0.5,1,1,0.8), ColorGLfloat(1,0.5,0.5,0.8),
        ColorGLfloat(0.5,0,0.5,0.8), ColorGLfloat(0.5,0.5,0,0.8), ColorGLfloat(0.5,0.5,0.5,0.8)};
    Circle * circle = new Circle(0,0,0,1,0,0,0,colors/* ColorGLfloat(1,0,0,1) */);
    // circle->setCenterX(2);
    // circle->setRotationPoint(0,0,0);
    can.add(circle);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // circle->setCenterX(sin(floatVal/90));
        // circle->setCenterY(sin(floatVal/90));
        // circle->setCenterZ(sin(floatVal/90));
        // circle->setYaw(floatVal);
        // circle->setPitch(floatVal);
        // circle->setRoll(floatVal);
        // circle->setRadius(sin(floatVal/90) + 1);
        // if (circle->getRadius() > 3 || circle->getRadius() < 1) {
        //     delta *= -1;
        //     circle->setEdgeColor(ColorGLfloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
        // }
        // circle->changeRadiusBy(delta);
        // if (delta > 0) {
        //     circle->setColor(colors);
        // } else {
        //     circle->setColor(ColorGLfloat(1,0,0,1));
        // }
        floatVal += 1;
    }

    delete circle;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Circle");
    c.setBackgroundColor(BLACK);
    c.run(circleFunction);
}