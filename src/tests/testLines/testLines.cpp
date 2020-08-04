/**
 * testLines.cpp tests displaying the Line and Polyline classes.
 */
#include <tsgl.h>
using namespace tsgl;

void lineFunction(Canvas& c) {
	ColorFloat colors[] = { ColorFloat(1,0,0,1), ColorFloat(0,1,0,0.6),
                            ColorFloat(0,0,1,0.9), ColorFloat(1,0,1,0.5), 
                            ColorFloat(1,1,0,0.8), ColorFloat(0,1,1,0.4), 
                            ColorFloat(0,0,1,0.7) };
    Line * l = new Line(0,0,0,200,0,0,0,ColorFloat(1,0,0,0.5));

    // l->setColor(RED);
    l->setColor(colors);

    float vertices[] = { -150,-100,-100, 
                        -100,100,0, 
                        -50,-100,100, 
                        0,-100,-100, 
                        50,100,0, 
                        100,-100,100, 
                        150,-100,-100 };

    Polyline * p = new Polyline(0,0,0,7,vertices,0,0,0,colors);

    Line * l2 = new Line(-250, -250, -100, -150, 200, 100, 0,0,0, colors);
    // printf("Line: %f\n", l->getAlpha());
    // printf("Pline: %f\n", p->getAlpha());

    c.bindToButton(TSGL_SPACE, TSGL_PRESS, [&l2] () {
       // l2->setFirstEndpoint(-250, -250, -100);
       printf("%f\n", l2->getLength());
    });

    // p->setColor(BLUE);
    // p->setColor(colors);
    c.add(l);
    c.add(p);
    c.add(l2);
    float floatVal = 0.0f;
    GLfloat delta = 5;
	while( c.isOpen() ) {
		c.sleep();
        // l->setCenterX(sin(floatVal/90));
        // l->setCenterY(sin(floatVal/90));
        // l->setCenterZ(sin(floatVal/90));
        // l->setYaw(floatVal);
        // l->setPitch(floatVal);
        // l->setRoll(floatVal);
        // l->setLength(100 * sin(floatVal/90) + 200);
        // if (l->getLength() > 300 || l->getLength() < 100) {
        //     delta *= -1;
        // }
        // l->changeLengthBy(delta);
        // p->setCenterX(sin(floatVal/90));
        // p->setCenterY(sin(floatVal/90));
        // p->setCenterZ(sin(floatVal/90));
        // p->setYaw(floatVal);
        // p->setPitch(floatVal);
        // p->setRoll(floatVal);
        // l2->setFirstEndpoint(-250,-250 * cos((float) c.getFrameNumber()/180),-100);
        // l2->setSecondEndpoint(-150,200 * cos((float) c.getFrameNumber()/180),100);
        // l2->setLength(350 + 150 * cos(floatVal/90));
        // if (l2->getLength() > 500 || l2->getLength() < 300) {
        //     delta *= -1;
        // }
        // l2->changeLengthBy(delta);
        // printf("%f, %f, %f - %f, %f, %f\n",l2->getFirstEndpointX(),l2->getFirstEndpointY(),l2->getFirstEndpointZ(),l2->getSecondEndpointX(),l2->getSecondEndpointY(),l2->getSecondEndpointZ());
        floatVal += 1;
	}

	delete l;
    delete p;
    delete l2;
}

int main(int argc, char* argv[]) {
  int w = 1000;
  int h = 600;
  Canvas c(-1, -1, w, h, "Lines");
  c.run(lineFunction);
}