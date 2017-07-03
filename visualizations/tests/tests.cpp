#include <tsgl.h>
#include "Fork.h"
using namespace tsgl;

int main(int argc, char* argv[]) {
  Canvas c(0, 0, 500, 500, "Canvas");
  c.start();

  Star s(150, 100, 50, 4, BLUE, true);

  c.add( &s );
  c.sleepFor(0.5);
  s.rotate(PI/5);
  // for(int i = 0; i < 3; i++) {
  //   s.rotateAround(PI/16, 300, 300);
  //   c.sleepFor(0.5);
  // }
  s.rotateAround(-PI/8, 300, 300);
  c.sleepFor(0.5);
  s.rotateAround(PI/8, 300, 300);
  c.sleepFor(0.5);

  Fork f;
  f.draw(c, 200, 200, 0, GREEN);
  c.sleepFor(0.5);
  f.draw(c, 200, 200, PI/4, GREEN);
  c.sleepFor(0.5);
  f.draw(c, 200, 200, PI/2, GREEN);

  Circle circle(100, 100, 30, 30, RED);
  c.add( &circle );
  c.sleepFor(0.5);
  circle.rotateAround(PI/2, 250, 250);
  c.sleepFor(0.5);
  circle.rotateAround(PI/2, 250, 250);
  c.sleepFor(0.5);
  circle.rotateAround(PI/2, 250, 250);
  c.sleepFor(0.5);
  circle.rotateAround(PI/2, 250, 250);

  c.wait();
}
