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
	// std::vector<Arrow*> shapes;
	ColorFloat color[2];
	color[0] = BLUE;
	color[1] = YELLOW;

	for(int i = 0; i < 4; i++) {

		int x = xs[i], y = ys[i];

		// draw Arrows outlining a square centered at x, y
		for(int i = -100; i <= 100; i+= 20) {
			// Arrow * a0 = new Arrow(x, y, x+i, y-100, PURPLE);
			// Arrow * a1 = new Arrow(x, y, x-100, y+i,  GREEN);
			// Arrow * a2 = new Arrow(x, y, x+100, y+i,  color);
			// Arrow * a3 = new Arrow(x, y, x+i, y+100,    RED);
			c.drawArrow(x, y, x+i, y-100, PURPLE); 
			c.drawArrow(x, y, x-100, y+i,  GREEN); 
			c.drawArrow(x, y, x+100, y+i,  color); 
			c.drawArrow(x, y, x+i, y+100,    RED);
			// c.add(a0); c.add(a1); c.add(a2); c.add(a3);
			// shapes.push_back(a0); shapes.push_back(a1);
			// shapes.push_back(a2); shapes.push_back(a3);
		}
	}

	//Draw double headed arrow moving around Canvas
	Arrow* doubleArrow = new Arrow(500, 500, 250, 250, WHITE, true);
	c.add(doubleArrow);

	int x = 250, y = 250;
	int count = 0;

	while( c.isOpen() ) {
		c.sleep();
		//c.drawArrow(500, 500, x, y, WHITE, true);
		c.clear();
		x += 10; if(x > 1000) x = 250;
		y += 30; if(y > 1000) y = 250;
		doubleArrow->moveHead(x, y);
		count++;
		if (count > 50) {
			c.remove(doubleArrow);
		}
	}

	c.wait();

	// delete doubleArrow;
	// for(unsigned int i = 0; i < shapes.size(); i++) {
	// 	delete shapes[i];
	// }
	return 0;
}