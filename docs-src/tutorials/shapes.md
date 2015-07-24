##Shapes

So, now that you know how to create a Canvas and draw text, let's get to the real fun stuff.

TSGL has a wide assortment of shapes. Rectangles, triangles, concave and convex polygons are just some of the shapes that you can draw. Each shape has its own class that inherits from a generalized Shape class. To add, the Canvas class has draw methods that will create and draw a corresponding shape on a Canvas object as well as shapes that do not have classes of their own (circles, for example).

This means that you do not have to create a shape object and then draw it on the Canvas; the draw methods built into the Canvas class take care of that for you and draw the shape accordingly.

Let's look at some examples. Create a file named shapes.cpp, and place this code inside of it:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Shapes!", FRAME);
  c.start();
  c.drawCircle(250, 300, 50, 32, BLACK, false);
  c.wait();
}
~~~

Compile and run it. A window should appear with a circle drawn in the center. The c.drawCircle() method takes as parameters the x and y-coordinates for the center of the circle, the radius, the resolution of the circle (32 or 64. 64 makes it look nicer and smoother), the color, and a boolean which determines if the circle should be filled or not.

Rectangles are drawn in a similar fashion:

~~~{.cpp}
c.drawRectangle(50, 100, 100, 200, BLACK, true);
~~~

The method takes in the coordinates for the top left corner of the rectangle and the bottom right corner of the rectangle and draws it onto the Canvas in black. Since the boolean value for filling the shape is set to ```true``` the rectangle drawn on the Canvas will be filled black.

How about a triangle? Well:

~~~{.cpp}
c.drawTriangle(150, 100, 250, 200, 150, 300, RED, false);
~~~

The method takes in the coordinates for the three points of the triangle: (150, 100), (250, 200), and (150, 300). It then draws the triangle in red without filling it.

In essence, whenever you draw a shape, the first few parameters are for the x and y-coordinates of the points of the shape and the last two are for the color and whether or not it should be filled.

Putting all of this code together:
~~~{.cpp}
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
~~~

Compile and run it. A circle, red triangle, and black rectangle should appear on a gray screen.

You can also draw regular lines:

~~~{.cpp}
c.drawLine(10, 20, 30, 40, BLACK);
~~~

The method takes in the first x and y-coordinate for a points and the x and y-coordinate for a second point and draws a black line from the first to the second point.

Check out the documentation for the Canvas class in the "Doxygen" page to learn more about its various shape drawing methods. You can also look at the various classes for the shapes to learn more about how the shapes are actually drawn onto the Canvas.

That concludes shapes! To learn more about colors, check out "Tutorials: Colors". If you want to see mathematical functions being graphed on a Cartesian coordinate system, check out "Tutorials: Functions".
