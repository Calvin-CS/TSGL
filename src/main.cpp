#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "I/O Example");
  c.start();
  c.setBackgroundColor(WHITE);
  //This variable is in the scope of the Lambda function.
  //It can be passed in.
  bool leftMouseButtonPressed = false;

  //Bind the left mouse button so that when it's pressed
  //the boolean is set to true.
  c.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS,
                    [&leftMouseButtonPressed]() {
                          leftMouseButtonPressed = true;
                    }
                );

  //Bind the left mouse button again so that when it's released
  //the boolean is set to false.
  c.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE,
                    [&leftMouseButtonPressed]() {
                          leftMouseButtonPressed = false;
                    }
                );

  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    if(leftMouseButtonPressed) {
      c.drawRectangle(250, 250, 40, 30, GREEN);
    } else {
      c.drawRectangle(250, 250, 40, 30, RED);
    }
    c.clear();
  }
  c.wait();
}
