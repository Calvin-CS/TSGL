TSGL has a wide assortment of shapes. Rectangles, triangles, concave and convex polygons are just some of the shapes that you can draw.  To draw a given shape, the Canvas class has draw methods that will create and draw a corresponding shape on a Canvas object.

Let’s look at some examples.

***Linux/Mac user:*** Create a new folder and name it "Tutorial2". Create a file inside of that folder and call it "shapes.cpp". Copy over the generic Makefile from the TSGL-master folder and change the "TARGET" line so that it now says "shapes" instead of "program".

***Windows users:*** Create a Solution folder and name it "Tutorial2". Add a Visual Studio project to that folder and name it "Shape". Go to the Property Manager and right click on that project. Add the testProperties Property sheet (located in the TSGL-master folder). Go back to the Solution Explorer and open up Tutorial2. Add a .cpp to Shape and name it "shapes.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Shapes!");
  c.start();
  c.setBackgroundColor(WHITE);
  Circle circle(250, 300, 50);
  c.add( &circle );
  c.wait();
}
~~~

Compile and run it. A window should appear with a circle drawn in the center.

The ```Circle()``` constructor takes these parameters:

* x-coordinate for the center of the circle (```250```).
* y-coordinate for the center of the circle (```300```).
* The radius of the circle (```50```).
* The color of the circle (optional parameter; set to ```BLACK``` by default).

The ```Circle()``` constructor lets you specify the optional color argument like so:

~~~{.cpp}
  Circle circle(250, 300, 50, RED);  //Color = RED
~~~

Rectangles are created in a similar fashion:

~~~{.cpp}
  Rectangle rec(50, 100, 50, 100);
~~~

```Rectangle()``` takes in these parameters:

* x-coordinate for the top left corner of the rectangle (```50```).
* y-coordinate for the top left corner of the rectangle (```100```).
* width of the rectangle (```50```).
* height of the rectangle (```100```).
* The color of the rectangle (optional parameter; set to ```BLACK``` by default).

How about a triangle? Well:

~~~{.cpp}
  Triangle tri(150, 100, 250, 200, 150, 300);
~~~

```Triangle()``` takes in these parameters:

* x-coordinate for the first point of the triangle (```150```).
* y-coordinate for the first point of the triangle (```100```).
* x-coordinate for the second point of the triangle (```250```).
* y-coordinate for the second point of the triangle (```200```).
* x-coordinate for the third point of the triangle (```150```).
* y-coordinate for the third point of the triangle (```300```).
* The color of the triangle (optional parameter; set to ```BLACK``` by default).

In essence, whenever you draw a shape, the first few parameters are for the x and y-coordinates of the points of the shape and the last one is optional for the color.

After creating any shape, it must be added to a Canvas before it is visible. Like so:
~~~{.cpp}
  c.add( &shape );
~~~
```add()``` takes a pointer to a Drawable as its parameter. Later, if you would like to remove an item from a Canvas, passing the pointer to ```remove()``` takes the Drawable off the Canvas.

Putting all of this code together:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Shapes!");
  c.start();
  c.setBackgroundColor(WHITE);
  Circle circle(250, 300, 50);  //Circle
  Rectangle rec(50, 100, 100, 200);  //Rectangle
  Triangle tri(150, 100, 250, 200, 150, 300);  //Triangle
  c.add( &circle ); c.add( &rec ); c.add( &tri );
  c.wait();
}
~~~

Compile and run it. A filled black circle, triangle, and rectangle should appear on a white screen.

You can also draw regular lines:

~~~{.cpp}
Line l(10, 20, 30, 40, PURPLE);
~~~

```Line()``` takes in these parameters:

* x-coordinate of the first point of the line (```10```).
* y-coordinate of the first point of the line (```20```).
* x-coordinate of the second point of the line (```30```).
* y-coordinate of the second point of the line (```40```).
* The color of the line (```PURPLE```) (optional parameter; set to ```BLACK``` by default.).

Check out the documentation in the [TSGL API](http://calvin-cs.github.io/TSGL/html/annotated.html) page to learn more about the variety of shapes.

That concludes this tutorial!

In the next tutorial, [[Using Text and Images]], you get to learn about text!
