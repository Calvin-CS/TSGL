##Getting Started

So, you've downloaded and installed TSGL. Congrats! Now what? Well, there's a *lot* you can do with TSGL. For now, let's start by making a simple Hello World program!

Start off by creating a file and call it hello.cpp.

We'll be writing in C++, so let's place our #include directives:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;
~~~

tsgl.h contains all of the necessary files needed in order to use the TSGL library. This includes vital class header files such as those for the Canvas class and the Timer class. TSGL also has its own namespace which must be used when using the TSGL library.

Moving forward, let's add some code that will create and initialize a Canvas:

~~~{.cpp}
int main() {
  Canvas c(0, 0, 200, 300, "Hello World!", FRAME);
  c.start();
  c.wait();
}
~~~

This is essentially the skeleton code for any TSGL program. Let's break it down. In the main method, a Canvas object is created centered at (0, 0) with a width of 200 and a height of 300. It has a title, "Hello World!" and its timer is set to expire once every FRAME seconds. What that means is that every FRAME of a second the Canvas will update itself. FRAME is a constant for 1/60. So, the timer will expire once every 1/60th of a second and the Canvas will update and draw something. The c.start() and c.wait() statements tell the Canvas to start drawing and then wait to close once it has completed drawing.

In between the c.start() and c.wait() statements is where the magic happens. We will place a new statement which will draw text to the Canvas:

~~~{.cpp}
int main() {
  Canvas c(0, 0, 200, 300, "Hello World!", FRAME);
  c.start();
  c.drawText("Hello World!", 50, 150, 20, BLACK);
  c.wait();
}
~~~

c.drawText() draws text to the Canvas with the top left part of the text starting at (50, 150) and then being drawn 20 pixels long with in black (BLACK is a TSGL color constant. There are multiple color constants available such as RED, BLUE, ORANGE, and so on. Please consult the documentation for TSGL to learn more about these constants.).

Our code is now complete! Compile the code, and then once it has compiled correctly, run it. A window should pop up with "Hello World!" located in the center.

Please note that the c.start() and c.wait() methods *MUST* be used at least once when working with the Canvas class. Bad things will happen if you do not have these methods and decide to draw something on a Canvas.

You've just made your first TSGL project, congratulations!

To see something a bit more interesting, check out the "Tutorials I: Shapes!" page.
