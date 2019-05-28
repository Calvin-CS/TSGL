/**
 * testArrows.cpp tests displaying the Line class's Arrow and Dotted attributes
 */
#include <tsgl.h>
using namespace tsgl;

int main() {
	Canvas c(-1, -1, 1000, 1000, "Line Tests", FRAME * 13);
	c.start();

	int xs[4] = {250, 250, 750, 750};
	int ys[4] = {250, 750, 250, 750};

    float rotation = 0;

    c.drawStar(xs[0], ys[0], 100, 6, RED, true, PI/12);
    c.drawStar(xs[1], ys[1], 50, 7, BLUE, false, PI/12);
    c.drawStar(xs[2], ys[2], 125, 8, GREEN, true, PI/12);
    c.drawStar(xs[3], ys[3], 75, 9, ORANGE, false, PI/12);

	while( c.isOpen() ) {
		c.sleep();
	}

	c.wait();
	return 0;
}