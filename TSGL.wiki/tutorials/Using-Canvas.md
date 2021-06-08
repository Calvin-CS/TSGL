***BEFORE PROCEEDING, PLEASE SEE THE [[Building Programs]] PAGE FOR MORE INFORMATION ON HOW TO COMPILE AND RUN A PROGRAM WITH TSGL CODE!!!***

So, you've downloaded and installed TSGL. Congrats! Now what? Well, there's a *lot* you can do with TSGL. For now, let's start by making a simple Hello World program!

***Linux/Mac users***: Start off by creating a folder and name it "Tutorial1". Create a file inside of Tutorial1 and name it "hello.cpp". Then, navigate to the TSGL-master folder and into the genericMakefile folder. Copy the Makefile into Tutorial1 and change the "TARGET" line so that "program" is now "hello".

***Windows users***: Create a new Solution folder and call it "Tutorial1". Then, add a Visual Studio project to that folder and call it "Canvas". Then, go to the Property Manager section and right click on the Canvas project. Click "Add existing Property sheet", and navigate to the TSGL-master folder. Select "testProperties" and then go back to the Solution Explorer. Open up Tutorial1 and right click on Canvas. Add a .cpp file and name it "canvas.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

Now, we'll be writing in C++, so let's place our #include and using directives:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;
~~~

```tsgl.h``` contains #include directives for all of the necessary header files needed in order to use the TSGL library. This includes vital class header files such as those for the Canvas class and the Timer class. TSGL also has its own namespace which must be used when using the TSGL library.

Moving forward, let's add some code that will create and initialize a Canvas. A Canvas is essentially a screen that draws and displays whatever it is that you want to draw and display. There's a special kind of Canvas, the CartesianCanvas, that we will look at later on.

For now, let's focus on the normal Canvas:

~~~{.cpp}
int main() {
  Canvas c(0, 0, 600, 350, "Hello World!");
  c.start();
  c.wait();
}
~~~

This is essentially the skeleton code for any TSGL program. Let's break it down.  In the main method, a Canvas object is created positioned at ```(0, 0)``` (on your monitor) with a width of ```600``` and a height of ```350```. It has a title, ```Hello World!```.

The ```c.start()``` and ```c.wait()``` statements tell the Canvas to start drawing and then wait to close once it has completed drawing.

In between the ```c.start()``` and ```c.wait()``` statements is where the magic happens. We will place a new statement which will draw a line to the Canvas:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 600, 350, "Hello World!");
  c.setBackgroundColor(WHITE);
  c.start();
  //'H'
  Line h0(100, 50, 100, 300);
  Line h1(100, 150, 165, 150);
  Line h2(165, 50, 165, 300);

  //'E'
  Line e0(200, 50, 280, 50);
  Line e1(200, 50, 200, 300);
  Line e2(200, 150, 280, 150);
  Line e3(200, 300, 280, 300);

  //The two 'L's
  Line l0(300, 50, 300, 300);
  Line l1(300, 300, 320, 300);
  Line l2(350, 50, 350, 300);
  Line l3(350, 300, 370, 300);

  //'O'
  UnfilledEllipse o(400, 180, 30, 100);

  //Add 'HELLO' to the Canvas
  c.add(&h0); c.add(&h1); c.add(&h2);
  c.add(&e0); c.add(&e1); c.add(&e2); c.add(&e3);
  c.add(&l0); c.add(&l1); c.add(&l2); c.add(&l3);
  c.add(&o);

  c.wait();
}
~~~

We will examine the ```Line``` and ```UnfilledEllipse``` classes in the [[Using Shapes]] tutorial. For now, just know that ```Line()``` creates a Line, ```UnfilledEllipse()``` creates the outline of an ellipse, and ```add()``` draws any Drawable object to the Canvas.

Our code is now complete! Compile the code, and then once it has compiled correctly, run it. A window should pop up with "Hello" written on it. To close the window, press the ESC key or click the 'x' in one of the corners.

Please note that the ```c.start()``` and ```c.wait()``` methods *MUST* be used at least once when working with the Canvas class. Bad things will happen if you do not have these methods and decide to draw something on a Canvas.

There are also special types of Canvases for different situations. CartesianCanvas also draws lines and circles, but using methods like ```can.drawLine()``` and ```can.drawCircle()```, rather than the object oriented approach, and on a Cartesian coordinate system instead. RasterCanvas and CartesianRasterCanvas are used like their non-Raster equivalents, but they render each object once and then it cannot be removed with ```can.remove()```. RasterCanvas is particularly useful for drawing points. See the Mandelbrot example for a use of the RasterCanvas.

You've just made your first TSGL project, congratulations!

Next up, in [[Using Shapes]], we take a look at how to draw shapes onto a Canvas.
