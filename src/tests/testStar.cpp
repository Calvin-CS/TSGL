/**
 * testStar.cpp tests displaying the Star class
 */
#include <tsgl.h>
using namespace tsgl;

int main() {
	Canvas c(-1, -1, 1000, 1000, "Line Tests", FRAME * 13);
	c.start();

	int xs[4] = {250, 250, 750, 750};
	int ys[4] = {250, 750, 250, 750};

	ColorFloat * colors = new ColorFloat[16];
	colors[0] = YELLOW;
	for(int i = 1; i < 16; i ++) {
		colors[i] = GREEN;
	}

    float rotation = 0;
	Star * s1 = new Star(xs[2], ys[2], 125, 8, colors, colors, true);
	Star * s2 = new Star(xs[3], ys[3], 75, 9, PURPLE, BLUE, false);
	Star * s3 = new Star(xs[0], ys[0], 100, 6, RED, true, true);
	Star * s4 = new Star(xs[1], ys[1], 50, 7, BLUE, false, false);
	c.add(s1); 
	c.add(s2); 
	c.add(s3); 
	c.add(s4); 

	while( c.isOpen() ) {
		c.sleep();
		c.pauseDrawing();
		rotation += PI / 3;
		s1->setRotation(rotation);
		c.resumeDrawing();
		c.sleepFor(1);
	}

	c.wait();

	delete[] colors;
	delete s1;
	delete s2;
	delete s3;
	delete s4;
	return 0;
}