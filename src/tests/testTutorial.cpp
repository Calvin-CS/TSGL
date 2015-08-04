#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example", FRAME / 2);
  c.start();
  //Store the x and y-coordinate values in variables outside of the loop
  int x = 250, y = 300;
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    //Check to see if we are off screen
    if (x >= c.getWindowWidth()) {
       x = 250; //If so, reset the x-coordinate
    }
    //Let's draw a circle!
    //Pass the x and y coordinates
    c.drawCircle(x, y, 50, 32);
    //And change the x-coordinate once the circle has been drawn
    x += 5;
    c.clear(); //New statement
  }
  c.wait();
}
