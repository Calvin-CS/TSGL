#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Shapes!", FRAME);
  c.start();
  c.drawCircle(250, 300, 50, 32, BLACK, false);
  c.drawRectangle(50, 100, 100, 200, BLACK, true);
  c.drawTriangle(150, 100, 250, 200, 150, 300, RED, false);
  c.wait();
}
