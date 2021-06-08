You can't spell "functions" without "fun"!

On a more serious note, TSGL has the capability to graph mathematical functions. 

We've been working with the standard Canvas class, but there's also another type of Canvas that is very useful for this purpose.

It's affectionately called the ***CartesianCanvas***.

Its a subclass of the Canvas class and so inherits and overrides many of the Canvas methods as well as adds some of its own. 

This type of Canvas has an adjustable Cartesian coordinate system. What that means is that instead of the point ```(0, 0)``` being in the top-left corner of the screen like in the standard Canvas, you can now specify where you want the center to be. 

***Linux/Mac users:*** Follow the steps from the previous tutorials. See the video on multi file programs in [Building Programs](https://github.com/Calvin-CS/TSGL/wiki/Building-Programs) page. Name the folder "Tutorial6" and the file "cartesian.cpp". Replace "program" in the "TARGET" line of the Makefile with "cartesian".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial6" and the Visual Studio project "Cartesian". After adding the Property sheet, name the .cpp file "cartesian.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a multi-file program). 

Let's start with a simple example:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  CartesianCanvas c(0, 0, 500, 500, -5.0, -5.0, 5.0, 50.0, "Cartesian Example");
  c.start();
  c.wait();
}
~~~

Compile the code and run it. A blank gray screen should pop up.

Looking at the code, it looks eerily similar to how a Canvas is constructed but with a few added parameters The constructor creates a CartesianCanvas whose upper left corner is at screen coordinate ```(0, 0)```, with a width of ```500``` and a height of ```500```. It then sets and scales the CartesianCanvas’s axes and internal coordinate system so that the left edge of the canvas is ```x = -5.0``` in the Cartesian coordinate system, the bottom edge is ```y = -5.0```, the right edge is ```x = +5.0```, and the top edge is ```y = +50.0```. It then gives the window a title, ```Cartesian Example```, and uses the default timer length of ```0.0``` (which you can set explicitly if you so desire, as described in our [[Animation Loops]] tutorial).

If we wanted to instead center the CartesianCanvas somewhere else, all we would have to do is change the first two parameters:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  CartesianCanvas c(300, 400, 500, 500, -5.0, -5.0, 5.0, 50.0, "Cartesian Example");
  c.start();
  c.wait();
}
~~~

That should create a 500x500 CartesianCanvas whose upper left corner is positioned at ```(300, 400)``` on your monitor.

Notice how the Canvas has integer coordinates whereas the CartesianCanvas has Decimal coordinates. 

Notice also how you can set the upper left corner of the CartesianCanvas with the first two arguments. These arguments can be used to position the canvas arbitrarily. You can set it pretty much anywhere you want (within reason of course. It has to be within the range of the size of the screen.). 

You can use the 4th, 5th, 6th, and 7th arguments to set the bounds of the coordinate system within the CartesianCanvas.

As mentioned before, the CartesianCanvas handles colors, shapes, and drawing loops in much the same way as a standard Canvas does. 

Now, how can we graph mathematical functions?

Well, let's first tell the CartesianCanvas to draw axes:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  CartesianCanvas c(0, 0, 500, 500, -5, -5, 5, 50, "Cartesian Example");
  c.start();
  c.setBackgroundColor(WHITE);  //Set the color to white to see the axes better
  c.drawAxes(0.0, 0.0, 1.0, 5.0);
  c.wait();
}
~~~

Recompile and run. The screen should pop up with a white background and axes drawn.

The ```drawAxes()``` method takes in these parameters:
* x-coordinate of the center of the CartesianCanvas (```0.0```).
* y-coordinate of the center of the CartesianCanvas (```0.0```).
* The spacing between the tick marks along the x-axis (```1.0```).
* The spacing between the tick marks along the y-axis (```5.0```).

Now, let's graph a function!

Wait...how do we do that?

It's rather simple really. TSGL comes with a built in ```Function``` class, and CartesianCanvas has a ```drawFunction()``` method that draws the graph of a given ```Function``` ***OR*** any subclass of ```Function```.

Before we continue, let us say a few notes that are platform dependent:

***Linux/Mac users:*** Create two new files: Cotangent.h and Cotangent.cpp. 

***Windows users:*** Add a Cotangent.cpp and Cotangent.h file to your project. 

To draw a specific mathematical function, we must define it as a subclass of the ```Function``` class:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

class Cotangent : public Function { 
//This is the .h file
};
~~~  

In order for ```drawFunction()``` to draw it, the subclass, ```Cotangent```, must supply a definition for the ```valueAt()``` method that is declared in the ```Function``` class.  

Here is the declaration from class ```Function```:

~~~{.cpp}
  virtual Decimal valueAt(Decimal x) const = 0;
~~~

This method is called an "abstract method” or “pure virtual function”. It can't be used on its own because there is no definition for it in the ```Function``` class.

However, any subclass that inherits from the ```Function``` class can (and *MUST*) provide a definition for this method.

That's exactly what we're going to do in the ```Cotangent``` class:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

class Cotangent : public Function {
 //This is the .h file
 public:
    virtual Decimal valueAt(Decimal x) const; 
};
~~~

~~~{.cpp}
#include "Cotangent.h"

//This is the .cpp file
Decimal Cotangent::valueAt(Decimal x) const {
    return cos(x) / sin(x);
}
~~~

As you can see, we define the ```valueAt(x)``` function to return the cotangent of x.

TSGL predefines the ```Power```, ```Square Root```, ```Sine```, ```Cosine```, ```Tangent```, ```Absolute Value```, ```Exponential```, ```Natural Log```, ```Common Log```, ```Ceiling```, ```Floor```, and ```Round``` functions in a similar fashion. 

It does not predefine the ```Cotangent``` function, which is why we had to define it ourselves.

Continuing forward, let's add a change to our cartesian.cpp file:

~~~{.cpp}
#include "Cotangent.h" //Take out the #include <tsgl.h> directive
                       //As well as the using namespace tsgl directive

int main() {
  CartesianCanvas c(0, 0, 500, 500, -5, -5, 5, 50, "Cartesian Example");
  c.start();
  c.setBackgroundColor(WHITE); //Set the color to white to see the axes better
  c.drawAxes(0.0, 0.0, 1.0, 5.0);
  c.wait();
}
~~~

Now we are ready to graph the ```Cotangent``` function!

To do so, simply create an instance of the ```Cotangent``` class and then pass it to the ```drawFunction()``` method from the CartesianCanvas object:

~~~{.cpp}
#include "Cotangent.h" //Take out the #include <tsgl.h> directive
                       //As well as the using namespace tsgl directive

//Main program
int main() {
  CartesianCanvas c(0, 0, 500, 500, -5, -5, 5, 50, "Cartesian Example");
  c.start();
  c.setBackgroundColor(WHITE);  //Set the color to white to see the axes better
  c.drawAxes(0.0, 0.0, 1.0, 5.0);
  Cotangent cot;  //Create an instance of the Cotangent class
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    c.drawFunction(cot); //Draw it onto the CartesianCanvas
  }
  c.wait();
}
~~~

Recompile and run. The graph of the ```Cotangent``` function should be drawn!

We didn't see it being drawn though...

That's because we omitted a second parameter, which is the length of time that we should have the main thread sleep for so that it can be in sync with the Canvas.

Omitting this parameter just draws the entire function “instantly" and so therefore, we can't see it.

We can change this, however, by not omitting the second parameter:

~~~{.cpp}
  c.drawFunction(cot, FRAME);
~~~

By placing ```FRAME``` as the second parameter, we tell the main thread to sleep for ```FRAME``` seconds (1/60th of a second) and then that syncs with the Canvas so that we can see the drawing actually occurring.

It's a little slow though, how can we speed it up?

To make it go faster, simply change the second parameter:

~~~{.cpp}
  c.drawFunction(cot, FRAME/2);
~~~

This makes it so that the thread sleeps for every ```FRAME/2``` of a second (or 1/120th of a second) and then a point is drawn on the Canvas.

There's also a third optional parameter which is the color of the function (set to ```BLACK``` by default).

See [Function.h](http://calvin-cs.github.io/TSGL/html/classtsgl_1_1_function.html) for more information on the predefined functions in TSGL.

If you ever want to graph a mathematical function that is not predefined, you will have to define it yourself in the same way as we did for the ```Cotangent``` function. 

Please note: The standard Canvas is intended for drawing shapes and animation loops; the CartesianCanvas is a specialized kind of Canvas for drawing mathematical functions.

See [CartesianCanvas](http://calvin-cs.github.io/TSGL/html/classtsgl_1_1_cartesian_canvas.html) in the TSGL API for more information on the special kind of Canvas. 

That completes this tutorial!

In the next tutorial, [[Using Keyboard && Mouse]], we take a look at how to handle keyboard and mouse events!