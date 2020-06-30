/*
 * testEllipse.cpp
 *
 * Usage: ./testEllipse
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void ellipseFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0,0,1,0.8),
        ColorFloat(0,1,0,0.8), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Ellipse * ellipse = new Ellipse(0,0,0,100,200,0,0,0,colors);
    // ellipse->setCenterX(200);
    // ellipse->setRotationPoint(0,0,0);
    can.add(ellipse);
    float floatVal = 0.0f;
    GLfloat delta = 5;
    while (can.isOpen()) {
        can.sleep();
        // ellipse->setCenterX(sin(floatVal/90) * 100);
        // ellipse->setCenterY(sin(floatVal/90) * 100);
        // ellipse->setCenterZ(sin(floatVal/90) * 100);
        // ellipse->setYaw(floatVal);
        // ellipse->setPitch(floatVal);
        // ellipse->setRoll(floatVal);
        // ellipse->setXRadius(sin(floatVal/90) * 100 + 100);
        // ellipse->setYRadius(sin(floatVal/90) * 100 + 200);
        // if (ellipse->getXRadius() > 300 || ellipse->getXRadius() < 100) {
        //     delta *= -1;
        // }
        // ellipse->changeXRadiusBy(delta);
        if (ellipse->getYRadius() > 300 || ellipse->getYRadius() < 100) {
            delta *= -1;
        }
        ellipse->changeYRadiusBy(delta);
        if (delta > 0) {
            ellipse->setColor(colors);
        } else {
            ellipse->setColor(RED);
        }
        floatVal += 1;
    }

    delete ellipse;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Ellipse");
    c.setBackgroundColor(BLACK);
    c.run(ellipseFunction);
}