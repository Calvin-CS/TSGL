/**
 * testArrows.cpp tests displaying the Line class's Arrow and Dotted attributes
 */
#include <tsgl.h>
using namespace tsgl;

void arrowFunction(Canvas& c) {
	int xs[4] = {250, 250, 750, 750};
	int ys[4] = {250, 750, 250, 750};
	ColorFloat color[2];
	color[0] = BLUE;
	color[1] = YELLOW;

	//Draw double headed arrow moving around Canvas
	Arrow* doubleArrow = new Arrow(500, 500, 250, 250, WHITE, true);
	c.add(doubleArrow);

	for(int i = 0; i < 4; i++) {

		int x = xs[i], y = ys[i];

		// draw Arrows outlining a square centered at x, y
		for(int i = -100; i <= 100; i+= 20) {
			// Arrow * a0 = new Arrow(x, y, x+i, y-100, PURPLE);
			// Arrow * a1 = new Arrow(x, y, x-100, y+i,  GREEN);
			// Arrow * a2 = new Arrow(x, y, x+100, y+i,  color);
			// Arrow * a3 = new Arrow(x, y, x+i, y+100,    RED);
			// c.add(a0); c.add(a1); c.add(a2); c.add(a3);
			c.drawArrow(x, y, x+i, y-100, PURPLE); 
			c.drawArrow(x, y, x-100, y+i,  GREEN); 
			c.drawArrow(x, y, x+100, y+i,  color); 
			c.drawArrow(x, y, x+i, y+100,    RED);
		}
	}

	int x = 250, y = 250;
	int count = 0;

	while( c.isOpen() ) {
		c.sleep();
		//c.drawArrow(500, 500, x, y, WHITE, true);
		c.clear();
		x += 10; if(x > 1000) x = 250;
		y += 30; if(y > 1000) y = 250;
		doubleArrow->moveHead(x, y);
		// c.sleepFor(0.5f);
		if (count == 200) {
			c.remove(doubleArrow);
		} else {
			count++;
		}
	}
}

// void arrowFunction(Canvas& c) {
// 	int x = c.getWindowWidth()/2;
// 	int y = c.getWindowHeight()/2;
// 	c.drawRectangle(0, 0, x, y, PURPLE); 
// 	c.drawRectangle(0, y, x, y,  GREEN); 
// 	c.drawRectangle(x, 0, x, y,  BLUE); 
// 	c.drawRectangle(x, y, x, y,    RED);

// 	Rectangle * rec = new Rectangle(100, 100, 100, 100, YELLOW);
// 	c.add(rec);

// 	while(c.isOpen() ) {
// 		c.sleep();
// 		c.clear();
// 		rec->moveShapeBy(2, 2);

// 	}
// }

//Takes in command line arguments for the window width and height
int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 1000;
  int h = (argc > 2) ? atoi(argv[2]) : 1000;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 1000;            //If not, set the width and height to a default value
  Canvas c(-1, -1, w, h, "Line Tests");
  c.run(arrowFunction);
}