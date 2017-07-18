/**
 * testArrows.cpp tests displaying the Line class's Arrow and Dotted attributes
 */
#include <tsgl.h>
using namespace tsgl;

int main() {
	Canvas c(-1, -1, -1, -1, "Line Tests", FRAME * 13);
	c.start();

	// test the single headed arrows
	Arrow * a0 = new Arrow(20, 20, 50, 60, BLACK);
	Arrow * a1 = new Arrow(10, 10, 40, 10, RED);
	Arrow * a2 = new Arrow(10, 10, 10, 60, BLUE);
	Arrow * a3 = new Arrow(50, 50, 30, 30, PURPLE);
	Arrow * a4 = new Arrow(20, 20, 120, 120, GREEN);
	Arrow * a5 = new Arrow(160, 160, 40, 40, CYAN);
	Arrow * a6 = new Arrow(200, 200, 250, 150, MAGENTA);
	Arrow * a7 = new Arrow(200, 200, 250, 250, MAGENTA);
	Arrow * a8 = new Arrow(200, 200, 150, 250, MAGENTA);
	Arrow * a9 = new Arrow(200, 200, 150, 150, MAGENTA);
  c.add( a0 ); c.add( a1 ); c.add( a2 ); c.add( a3 ); c.add( a4 );
  c.add( a5 ); c.add( a6 ); c.add( a7 ); c.add( a8 ); c.add( a9 );

	// draw Lines outlining a square centered at 300, 300
	for(int i = 250; i <= 350; i+= 10) {
		Arrow * a10 = new Arrow(300, 300, i, 250, PURPLE);
		Arrow * a11 = new Arrow(300, 300, 250, i, GREEN);
		Arrow * a12 = new Arrow(300, 300, 350, i, BLUE);
		Arrow * a13 = new Arrow(300, 300, i, 350, RED);
    c.add(a10); c.add(a11); c.add(a12); c.add(a13);
	}

	// test arrows with head at other end
	// draw Lines outlining a square centered at 300, 500
	for(int i = -50; i <= 50; i+= 20) {
		Arrow * a14 = new Arrow(300, 500, 300+i, 450, PURPLE);
		Arrow * a15 = new Arrow(300, 500, 250,   500+i,  GREEN);
		Arrow * a16 = new Arrow(300, 500, 350,   500+i,   BLUE);
		Arrow * a17 = new Arrow(300, 500, 300+i, 550,    RED);
    c.add(a14); c.add(a15); c.add(a16); c.add(a17);
	}

		// test arrows with two heads at other end
	// draw Lines outlining a square centered at 300, 500
	for(int i = -50; i <= 50; i+= 20) {
		Arrow * a18 = new Arrow(300, 600, 300+i, 550, PURPLE);
		Arrow * a19 = new Arrow(300, 600, 250,   600+i,  GREEN);
		Arrow * a20 = new Arrow(300, 600, 350,   600+i,   BLUE);
		Arrow * a21 = new Arrow(300, 600, 300+i, 650,    RED);
    c.add(a18); c.add(a19); c.add(a20); c.add(a21);
	}

		// test dashed lines
	// draw Lines outlining a square centered at 300, 500
	for(int i = -50; i <= 50; i+= 20) {
		Arrow * a22 = new Arrow(500, 500, 500+i, 450, PURPLE);
		Arrow * a23 = new Arrow(500, 500, 450,   500+i,  GREEN);
		Arrow * a24 = new Arrow(500, 500, 550,   500+i,   BLUE);
		Arrow * a25 = new Arrow(500, 500, 500+i, 550,    RED);
    c.add(a22); c.add(a23); c.add(a24); c.add(a25);
	}

	Arrow * a26 = new Arrow(300, 400, 600, 600, BLACK );
  c.add(a26);
  a26->moveHead(500, 500);
  c.sleep();
  a26->moveTail(300, 400);
  c.sleep();
  a26->moveTail(500, 600);
  c.sleep();
  for(int i = 0; i < 5; i++) {
    a26->moveHead(500, 500+50*i);
    c.sleep();
  }
  for(int i = 0; i < 5; i++) {
    a26->moveHead(500+50*i, 500);
    c.sleep();
  }

	c.wait();
	return 0;
}
