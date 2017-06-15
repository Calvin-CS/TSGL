/**
 * testArrows.cpp tests displaying the Line class's Arrow and Dotted attributes
 */
#include "../TSGL/tsgl.h"
#include "../TSGL/Line.h"
#include "../TSGL/Canvas.h"
using namespace tsgl;

int main() {
	Canvas c(-1, -1, -1, -1, "Line Tests", FRAME * 13);
	c.setBackgroundColor(WHITE);
	c.start();
	
	// test the single headed arrows
	c.drawLine(20, 20, 50, 60, BLACK, false, true);
	c.drawLine(10, 10, 40, 10, RED, false, true);
	c.drawLine(10, 10, 10, 60, BLUE, false, true);
	c.drawLine(50, 50, 30, 30, PURPLE, false, true);
	c.drawLine(20, 20, 120, 120, GREEN, false, true);
	c.drawLine(160, 160, 40, 40, CYAN, false, true);
	c.drawLine(200, 200, 250, 150, MAGENTA, false, true);
	c.drawLine(200, 200, 250, 250, MAGENTA, false, true);
	c.drawLine(200, 200, 150, 250, MAGENTA, false, true);
	c.drawLine(200, 200, 150, 150, MAGENTA, false, true);
	
	// draw Lines outlining a square centered at 300, 300
	for(int i = 250; i <= 350; i+= 10) {
		c.drawLine(300, 300, i, 250, PURPLE, false, true);
		c.drawLine(300, 300, 250, i, GREEN, false, true);
		c.drawLine(300, 300, 350, i, BLUE, false, true);
		c.drawLine(300, 300, i, 350, RED, false, true);
	}
	
	// test arrows with head at other end	
	// draw Lines outlining a square centered at 300, 500
	for(int i = -50; i <= 50; i+= 20) {
		c.drawLine(300, 500, 300+i, 450, PURPLE, false, false, true);
		c.drawLine(300, 500, 250,   500+i,  GREEN, false, false, true);
		c.drawLine(300, 500, 350,   500+i,   BLUE, false, false, true);
		c.drawLine(300, 500, 300+i, 550,    RED, false, false, true);
	}
	
		// test arrows with two heads at other end	
	// draw Lines outlining a square centered at 300, 500
	for(int i = -50; i <= 50; i+= 20) {
		c.drawLine(300, 600, 300+i, 550, PURPLE, false, true, true);
		c.drawLine(300, 600, 250,   600+i,  GREEN, false, true, true);
		c.drawLine(300, 600, 350,   600+i,   BLUE, false, true, true);
		c.drawLine(300, 600, 300+i, 650,    RED, false, true, true);
	}
	
		// test dashed lines
	// draw Lines outlining a square centered at 300, 500
	for(int i = -50; i <= 50; i+= 20) {
		c.drawLine(500, 500, 500+i, 450, PURPLE, true, false, false);
		c.drawLine(500, 500, 450,   500+i,  GREEN, true, false, false);
		c.drawLine(500, 500, 550,   500+i,   BLUE, true, false, false);
		c.drawLine(500, 500, 500+i, 550,    RED, true, false, false);
	}
	
	c.drawLine(300, 400, 600, 600, BLACK, true , true, true);

	c.wait();
	return 0;
}