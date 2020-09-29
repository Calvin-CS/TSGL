/**
 * testArrows.cpp tests displaying the Arrow class.
 */
#include <tsgl.h>
using namespace tsgl;

void arrowFunction(Canvas& c) {
	ColorFloat colors[] = { ColorFloat(1,0,0,0.5), ColorFloat(0,1,0,1) };
	Arrow* doubleArrow = new Arrow(100, 100, 50, 200, 5,0,0,0, colors, true);
    Arrow* arrow2 = new Arrow(0 ,0 ,0 ,200 ,5,0,0,0,ColorFloat(0,0,1,0.65), false);
	c.add(doubleArrow);
    // doubleArrow->setLength(200);
    // doubleArrow->changeLength(0);
    c.add(arrow2);

    Line * line1 = new Line(100,200,50,300,0,0,0,PURPLE);
    c.add(line1);
    Line * line = new Line(100,150,50,100,0,0,0,ORANGE);
    c.add(line);
    Line * line2 = new Line(-250, 150, 75, -200, -50, -75, 0,0,0, PURPLE);
    c.add(line2);

    Arrow * a3 = new Arrow(-250, 250, 150, -150, -150, -150, 10, 0,0,0, colors, true);
    c.add(a3);
    a3->setLength(a3->getLength() / 2);
	// doubleArrow->setCenterX(100);
	// doubleArrow->setRotationPoint(0,0,0);
	// doubleArrow->setYaw(45);
    // printf("%f\n", doubleArrow->getAlpha());
	// doubleArrow->setColor(ColorFloat(1,0,0,0.9));
    // printf("%f\n", doubleArrow->getAlpha());
	// doubleArrow->setColor(colors);
    // printf("%f\n", doubleArrow->getAlpha());  

    c.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&c, &a3] () {
        a3->setSecondEndpoint(c.getMouseX(), c.getMouseY(), 0);
        a3->setWidth(saferand(1, 45));
    });

    GLfloat delta = 5;
    bool boolean = true;
	while( c.isOpen() ) {
		c.sleep();
        // doubleArrow->setCenterX(sin((float)c.getFrameNumber()/90));
        // doubleArrow->setCenterY(sin((float)c.getFrameNumber()/90));
        // doubleArrow->setCenterZ(sin((float)c.getFrameNumber()/90));
        // doubleArrow->setYaw(c.getFrameNumber());
        // doubleArrow->setPitch(c.getFrameNumber());
        // doubleArrow->setRoll(c.getFrameNumber());
        // printf("1: %f, %f, %f\n", doubleArrow->getFirstEndpointX(),doubleArrow->getFirstEndpointY(),doubleArrow->getFirstEndpointZ());
        // printf("2: %f, %f, %f\n", doubleArrow->getSecondEndpointX(),doubleArrow->getSecondEndpointY(),doubleArrow->getSecondEndpointZ());
        // doubleArrow->setLength(sin((float)c.getFrameNumber()/90) * 100 + 200);
        // if (doubleArrow->getLength() > 300 || doubleArrow->getLength() < 100) {
        //     delta *= -1;
        //     arrow2->setIsOutlined(boolean);
        //     arrow2->setIsFilled(!boolean);
        //     boolean = !boolean;
        // }
        // doubleArrow->changeLengthBy(delta);
        // a3->setLength(sin((float)c.getFrameNumber()/45) * 100 + 150);
	}

	delete doubleArrow;
    delete line1;
    delete line;
    delete line2;
    delete arrow2;
    delete a3;
}

int main(int argc, char* argv[]) {
  int w = 600;
  int h = 600;
  Canvas c(-1, -1, w, h, "Arrows", BLACK);
  c.run(arrowFunction);
}