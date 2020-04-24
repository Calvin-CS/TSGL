/**
 * testStar.cpp tests displaying the Star class
 * Note: tests currently indicating that performance is awful on a Macbook Pro 2012.
 */
#include <tsgl.h>
using namespace tsgl;

int main() {
	Canvas c(-1, -1, 1000, 1000, "Stars", FRAME * 13);
	c.start();

	ColorGLfloat * colors = new ColorGLfloat[16];
	colors[0] = ColorGLfloat(1,0,0,1);
	for(int i = 1; i < 16; i ++) {
		colors[i] = ColorGLfloat(0,1,0,1);
	}

	Star * s1 = new Star(0, 0, 0, 1, 5, 0,0,0, colors, true);
	s1->displayOutlineEdges(false);
	// s1->setColor(ColorGLfloat(1,0,0,1));
	c.add(s1);

    float floatVal = 0.0f;
	GLfloat delta = 0.05;
    while (c.isOpen()) {
        c.sleep();
        // s1->setCenterX(sin(floatVal/90));
        // s1->setCenterY(sin(floatVal/90));
        // s1->setCenterZ(sin(floatVal/90));
        // s1->setYaw(floatVal);
        // s1->setPitch(floatVal);
        // s1->setRoll(floatVal);
        // s1->setRadius(sin(floatVal/90) + 3);
        // if (s1->getRadius() > 3 || s1->getRadius() < 1) {
        //     delta *= -1;
        //     // s1->setEdgeColor(ColorGLfloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), 1));
        // }
        // s1->changeRadiusBy(delta);
        // if (delta > 0) {
        //     s1->setColor(colors);
        // } else {
        //     s1->setColor(ColorGLfloat(1,0,0,1));
        // }
        floatVal += 1;
    }

	c.wait();

	delete[] colors;
	// delete s1; // not sure why this doesn't have to be deleted. But it doesn't.
	return 0;
}