/**
 * testStar.cpp tests displaying the Star class
 * Note: tests currently indicating that performance is awful on a Macbook Pro 2012.
 */
#include <tsgl.h>
using namespace tsgl;

void starFunction(Canvas& c) {
	ColorFloat * colors = new ColorFloat[400];
	colors[0] = ColorFloat(1,0,0,1);
	for(int i = 1; i < 10; i ++) {
		// colors[i] = ColorFloat(float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)), float(rand())/float((RAND_MAX)));
        colors[i] = ColorFloat(0,0,1,1);
	}

	Star * s1 = new Star(0, 0, 0, 200, 5, 0,0,0, colors, true);
    // s1->setColor(RED);
	c.add(s1);

    float floatVal = 0.0f;
	GLfloat delta = 5;
    while (c.isOpen()) {
        c.sleep();
        // s1->setCenterX(sin(floatVal/90) * 100);
        // s1->setCenterY(sin(floatVal/90) * 100);
        // s1->setCenterZ(sin(floatVal/90) * 100);
        // s1->setYaw(floatVal);
        // s1->setPitch(floatVal);
        // s1->setRoll(floatVal);
        // s1->setRadius(sin(floatVal/90) * 100 + 300);
        if (s1->getRadius() > 300 || s1->getRadius() < 100) {
            delta *= -1;
        }
        s1->changeRadiusBy(delta);
        if (delta > 0) {
            s1->setColor(colors);
        } else {
            s1->setColor(RED);
        }
        floatVal += 1;
    }

	delete[] colors;
	// delete s1; // not sure why this doesn't have to be deleted. But it doesn't.
	return 0;
}

int main(int argc, char* argv[]) {
  int w = 1000;
  int h = 1000;
  Canvas c(-1, -1, w, h, "Stars");
  c.run(starFunction);
}