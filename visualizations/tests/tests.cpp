#include <tsgl.h>
using namespace tsgl;

int main(int argc, char* argv[]) {
  Canvas c(0, 0, 500, 500, "Canvas");
  c.start();

  Star s(100, 100, 50, 5, BLUE, false);

  c.add( &s );
  c.sleepFor(0.5);
  s.rotate(PI/5);
  // for(int i = 0; i < 10; i++) {
  //   s.rotateAround(PI/5, 300, 300);
  //   c.sleepFor(0.5);
  // }

  c.wait();
}
