Let's make a loop-da-loop!

Now that you know how to draw simple shapes and use colors, it's time to learn how to make animations!

***Linux/Mac users:*** Follow the steps from the previous tutorials. Name the folder "Tutorial5" and the file "animation.cpp". Replace "program" in the "TARGET" line of the Makefile with "animation".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial5" and the Visual Studio project "Animation". After adding the Property sheet, name the .cpp file "animation.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

Before we begin, allow us to explain drawing loops.

Drawing loops are what allow us to make animations.

They are essentially while loops that execute code as long as the Canvas remains open.

Let's look at an example:

~~~{.cpp}
while(canvasName.isOpen()) {
   canvasName.sleep();
  //Code to execute as long as the Canvas is open...
}
~~~

This is the skeleton code for a drawing loop. The while loop executes its code as long as the accessor method, ```isOpen()```, returns ```true```. This method essentially checks if the Canvas window is still open and returns ```true``` if it is.

The ```sleep()``` method tells the Canvas to wait until something is ready to be drawn and then draw it once its ready.

Drawing loops are key to making a Canvas continually draw something until its closed. Let's make one!

First, let's start by making and initializing a Canvas:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example");
  c.setBackgroundColor(WHITE);
  c.start();
  c.wait();
}
~~~

Now, let's introduce our drawing loop:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example");
  c.start();
  c.setBackgroundColor(WHITE);
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
  }
  c.wait();
}
~~~

Now, let's have the Canvas draw something:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example");
  c.start();
  c.setBackgroundColor(WHITE);
  //Let's draw a circle!
  Circle circle(250, 300, 50);
  c.add( &circle );
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
  }
  c.wait();
}
~~~

Compile and run the code. A black circle should appear in the center. We create the circle outside the drawing loop because we only want to create and add it one time. Once it has been added, it will stay until we tell the Canvas to remove it.

But wait...it's not doing anything. It's just sitting there!

That's because we haven't told the Canvas to move it in our drawing loop.

In order to make it move, we can set its location and change the x and y-coordinates so that the Canvas moves it every frame, like so:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example");
  c.start();
  c.setBackgroundColor(WHITE);
  //Store the x and y-coordinate values in variables outside of the loop
  int x = 250, y = 300;
  //Let's draw a circle!
  Circle circle(x, y, 50);
  c.add(&circle);
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    //Pass the x and y coordinates
    circle.setCenter(x, y);
    //And change the x-coordinate once the circle has been drawn
    x += 2;
  }
  c.wait();
}
~~~
Recompile and run the code. The circle should move from left to right.

The circle just moves off screen though...is there anyway to make it so that once it goes off screen it gets redrawn back onto the screen?

Certainly:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example");
  c.start();
  c.setBackgroundColor(WHITE);
  //Store the x and y-coordinate values in variables outside of the loop
  int x = 250, y = 300;
  //Let's draw a circle!
  Circle circle(x, y, 50);
  c.add(&circle);
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    //Check to see if we are off screen
    if (x >= c.getWindowWidth()) {
      x = 250; //If so, reset the x-coordinate
    }
    //Pass the x and y coordinates
    circle.setCenter(x, y);
    //And change the x-coordinate once the circle has been drawn
    x += 2;
  }
  c.wait();
}
~~~

```c.getWindowWidth()``` is the accessor for the width of the current Canvas. If the x-coordinate value is greater than or equal to that value, then it gets reset back to 250 and so appears to be animated!

There is also another accessor method, ```getWindowHeight()```, which gets the height of the current Canvas.

In sum, in order to have an animation, follow these steps:

1). Create and initialize a Canvas object.

2). Call the ```start()``` and ```wait()``` methods.

3). In between the calls to the ```start()``` and ```wait()``` methods, place the drawing loop:

~~~{.cpp}
while(canvasName.isOpen()) {
  canvasName.sleep();
  //Code to change something....
}
~~~

4). Done!

There is an optional parameter in the Canvas constructor that controls the speed at which the circle moves, and that is the ```timerLength``` parameter. It essentially sets the length of time that the ```c.sleep()``` call will cause the Canvas to sleep each iteration of the animation loop.

TSGL has a constant for the standard length of time that the timer should keep: ```FRAME```. When we pass this constant as the timer length parameter, the internal timer will expire every 1/60ths of a second and the Canvas will update itself. The standard frame rate of the TSGL library is 60 frames a second.

Let's see what happens when we pass that constant as a parameter:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example", FRAME);
  c.start();
  c.setBackgroundColor(WHITE);
  //Store the x and y-coordinate values in variables outside of the loop
  int x = 250, y = 300;
  //Let's draw a circle!
  Circle circle(x, y, 50);
  c.add(&circle);
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    //Check to see if we are off screen
    if (x >= c.getWindowWidth()) {
      x = 250; //If so, reset the x-coordinate
    }
    //Pass the x and y coordinates
    circle.setCenter(x, y);
    //And change the x-coordinate once the circle has been drawn
    x += 2;
  }
  c.wait();
}
~~~

Recompile and run. The circle will still move at the same speed.

However, if we alter the timer length parameter, we can change the speed of the animation!

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 600, "Animation Loop Example", FRAME / 2);
  c.start();
  c.setBackgroundColor(WHITE);
  //Store the x and y-coordinate values in variables outside of the loop
  int x = 250, y = 300;
  //Let's draw a circle!
  Circle circle(x, y, 50);
  c.add(&circle);
  //Drawing loop
  while(c.isOpen()) {
    c.sleep();
    //Check to see if we are off screen
    if (x >= c.getWindowWidth()) {
      x = 250; //If so, reset the x-coordinate
    }
    //Pass the x and y coordinates
    circle.setCenter(x, y);
    //And change the x-coordinate once the circle has been drawn
    x += 2;
  }
  c.wait();
}
~~~

Recompile and run. Dividing the ```FRAME``` constant by 2 will speed up the animation!

We encourage you to try other divisors and see how they affect the animation rate!

As mentioned, there is a special kind of Canvas: The CartesianCanvas. Drawing loops and animations are handled in the same way (only with a Cartesian coordinate system).

That completes this tutorial!

Next up, in [[Using Functions]], we explain the special Canvas: The CartesianCanvas!
