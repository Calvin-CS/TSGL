/*
 * testRectangle.cpp
 *
 * Usage: ./testRectangle
 */

#include <tsgl.h>
#include <cmath>

using namespace tsgl;

void rectangleFunction(Canvas& can) {
    ColorFloat colors[] = { ColorFloat(0.5,0.5,0.5,1), ColorFloat(0,0,1,0.5),
        ColorFloat(0,1,0,0.6), ColorFloat(0,1,1,0.8), ColorFloat(1,0,0,0.8),
        ColorFloat(1,0,1,0.8), ColorFloat(1,1,0,0.8), ColorFloat(1,1,1,0.8),
        ColorFloat(0.5,0.5,0.5,0.8), ColorFloat(0.5,0.5,1,0.8),
        ColorFloat(0.5,1,0.5,0.8), ColorFloat(0.5,1,1,0.8), ColorFloat(1,0.5,0.5,0.8),
        ColorFloat(1,0.5,1,0.8), ColorFloat(1,1,0.5,0.8), ColorFloat(0,0,0.5,0.8),
        ColorFloat(0,0.5,0,0.8), ColorFloat(0,0.5,0.5,0.8), ColorFloat(0.5,0,0,0.8),
        ColorFloat(0.5,0,0.5,0.8), ColorFloat(0.5,0.5,0,0.8), ColorFloat(0.5,0.5,0.5,0.8)};
    Rectangle * rectangle = new Rectangle(0,0,0,100,200,0,0,0,colors/* ColorFloat(1,0,0,1) */);
    // rectangle->setCenterX(200);
    // rectangle->setRotationPoint(0,0,0);
    // printf("%f\n", rectangle->getAlpha());
	// rectangle->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", rectangle->getAlpha());
	// rectangle->setColor(colors);
    // printf("%f\n", rectangle->getAlpha()); 
    can.add(rectangle);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
    while (can.isOpen()) {
        can.sleep();
        // rectangle->setCenterX(sin(floatVal/90)*100);
        // rectangle->setCenterY(sin(floatVal/90)*100);
        // rectangle->setCenterZ(sin(floatVal/90)*100);
        // rectangle->setYaw(floatVal);
        // rectangle->setPitch(floatVal);
        // rectangle->setRoll(floatVal);
        // rectangle->setWidth(sin(floatVal/90) *100 + 100);
        // rectangle->setHeight(sin(floatVal/90) *100 + 200);
        // if (rectangle->getWidth() > 200 || rectangle->getWidth() < 100) {
        //     delta *= -1;
        // }
        // rectangle->changeWidthBy(delta);
        // if (rectangle->getHeight() > 300 || rectangle->getHeight() < 100) {
        //     delta *= -1;
        // }
        // rectangle->changeHeightBy(delta);
        // if (delta > 0) {
        //     rectangle->setColor(colors);
        // } else {
        //     rectangle->setColor(RED);
        // }
        floatVal += 1;
    }

    delete rectangle;
}

int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;            //If not, set the width and height to a default value
    Canvas c(-1, -1, 1024, 620, "Basic Rectangle", BLACK);
    c.run(rectangleFunction);
}