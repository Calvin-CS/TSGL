#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 200, 300, "Hello World!", FRAME);
  c.start();
//  c.setFont("assets/freefont/FreeMono.ttf");
  c.drawText("Hello World!", 50, 150, 20, BLACK);
  c.wait();
}
