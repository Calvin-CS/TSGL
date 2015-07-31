#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "I/O Example");
  c.start();
  //This variable is in the scope of the Lambda function.
  //It can be passed in.
  bool switch1 = false;

  //Bind the left mouse button so that when it's pressed
  //the boolean switch is set to true.
  c.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS,
                    [&switch1]() {
                          switch1 = true;
                    }
                );

  //Bind the left mouse button again so that when it's released
  //the boolean switch is set to false.
  c.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE,
                    [&switch1]() {
                          switch1 = false;
                    }
                );

  //Bind the spacebar so that when it's pressed
  //the Canvas is cleared.
  //(Yes, you can also pass in the Canvas as
  //a parameter to the Lambda function).
  //ANY variable that is in the scope of the
  //Lambda function can be passed as a parameter.
  c.bindToButton(TSGL_SPACE, TSGL_PRESS,
                    [&c]() {
                      c.clear();
                    }
                );

  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    int x = c.getMouseX(), y = c.getMouseY();  //Store the x and y-coordinates of the mouse
    c.setFont("assets/freefont/FreeMono.ttf");
    if(switch1) {
      c.drawText("YES", 270, 270, 30, GREEN);
      c.drawCircle(x, y, 20, 32, BLACK, true);
    } else {
      c.drawText("NO", 250, 250, 30, RED);
    }
  }
  c.wait();
}
