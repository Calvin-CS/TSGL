/**
 * testLines.cpp tests displaying the Line and Polyline classes.
 */
#include <tsgl.h>
using namespace tsgl;

void lineFunction(Canvas& c) {
	ColorFloat colors[] = { ColorFloat(1,0,0,1), ColorFloat(0,1,0,1),
                            ColorFloat(0,0,1,1), ColorFloat(1,0,1,1), 
                            ColorFloat(1,1,0,1), ColorFloat(0,1,1,1), 
                            ColorFloat(0,0,1,1) };
    Line * l = new Line(0,0,0,200,0,0,0,RED);

    // l->setColor(RED);
    l->setColor(colors);

    float vertices[] = { -150,-100,-100, 
                        -100,100,0, 
                        -50,-100,100, 
                        0,-100,-100, 
                        50,100,0, 
                        100,-100,100, 
                        150,-100,-100 };

    Polyline * p = new Polyline(0,0,0,7,vertices,0,0,0,BLUE);

    // p->setColor(BLUE);
    p->setColor(colors);
    c.add(l);
    c.add(p);
    float floatVal = 0.0f;
    GLfloat delta = 0.05;
	while( c.isOpen() ) {
		c.sleep();
        // l->setCenterX(sin(floatVal/90));
        // l->setCenterY(sin(floatVal/90));
        // l->setCenterZ(sin(floatVal/90));
        // l->setYaw(floatVal);
        // l->setPitch(floatVal);
        // l->setRoll(floatVal);
        // l->setLength(sin(floatVal/90) + 2);
        // if (l->getLength() > 3 || l->getLength() < 1) {
        //     delta *= -1;
        // }
        // l->changeLengthBy(delta);
        // p->setCenterX(sin(floatVal/90));
        // p->setCenterY(sin(floatVal/90));
        // p->setCenterZ(sin(floatVal/90));
        // p->setYaw(floatVal);
        // p->setPitch(floatVal);
        p->setRoll(floatVal);
        floatVal += 1;
	}

	delete l;
    delete p;
}

int main(int argc, char* argv[]) {
  int w = 1000;
  int h = 1000;
  Canvas c(-1, -1, w, h, "Lines");
  c.run(lineFunction);
}