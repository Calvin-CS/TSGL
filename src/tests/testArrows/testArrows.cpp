/**
 * testArrows.cpp tests displaying the Line class's Arrow and Dotted attributes
 */
#include <tsgl.h>
using namespace tsgl;

void arrowFunction(Canvas& c) {
	ColorFloat colors[] = { ColorFloat(1,0,0,0.5), ColorFloat(0,1,0,1) };
	Arrow* doubleArrow = new Arrow(0, 0, 0, 200, 5,0,0,0, colors, false);
    Arrow* arrow2 = new Arrow(100 ,100 ,-1 ,200 ,5,0,0,0,ColorFloat(0,0,1,0.65), true);
	c.add(doubleArrow);
    c.add(arrow2);
	// doubleArrow->setCenterX(100);
	// doubleArrow->setRotationPoint(0,0,0);
	// doubleArrow->setYaw(45);
    // printf("%f\n", doubleArrow->getAlpha());
	// doubleArrow->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", doubleArrow->getAlpha());
	// doubleArrow->setColor(colors);
    // printf("%f\n", doubleArrow->getAlpha());    
    float floatVal = 0.0f;
    GLfloat delta = 5;
    bool boolean = true;
	while( c.isOpen() ) {
		c.sleep();
        // doubleArrow->setCenterX(sin(floatVal/90));
        // doubleArrow->setCenterY(sin(floatVal/90));
        // doubleArrow->setCenterZ(sin(floatVal/90));
        // doubleArrow->setYaw(floatVal);
        // doubleArrow->setPitch(floatVal);
        // doubleArrow->setRoll(floatVal);
        // doubleArrow->setLength(sin(floatVal/90) * 100 + 200);
        if (doubleArrow->getLength() > 300 || doubleArrow->getLength() < 100) {
            delta *= -1;
            arrow2->setIsOutlined(boolean);
            arrow2->setIsFilled(!boolean);
            boolean = !boolean;
        }
        doubleArrow->changeLengthBy(delta);
        floatVal += 1;
	}

	delete doubleArrow;
}

int main(int argc, char* argv[]) {
  int w = 1000;
  int h = 1000;
  Canvas c(-1, -1, w, h, "Arrows", BLACK);
  c.run(arrowFunction);
}