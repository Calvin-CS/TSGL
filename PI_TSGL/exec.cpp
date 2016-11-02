#include "Canvas.h"

int main(){

	Canvas c(0,0, 800, 600, "HELLO WERLD");
	c.start();

	int x1 = 100, x2 = 150;
        while(c.isOpen()){
	  c.sleep();

	  c.drawRectangle(x1,100,x2,150,GREEN);
	  x1 += 2; x2 += 2;

	  c.clear();
	}
	
	c.wait();
	return 0;
}
