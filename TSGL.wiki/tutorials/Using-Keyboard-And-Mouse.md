Let's get down to the "key" points here.

You can add I/O interaction to any animation that you draw on a Canvas/CartesianCanvas. That includes keyboard and mouse events only.

You may be asking us, "Is that done using magic??”

Well, TSGL makes it fairly easy.

***Linux/Mac users:*** Follow the steps from the previous tutorials. Name the folder "Tutorial7" and the file "interaction.cpp". Replace "program" in the "TARGET" line of the Makefile with "interaction".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial7" and the Visual Studio project "Interactive". After adding the Property sheet, name the .cpp file "interaction.cpp".
Let's begin by creating and initializing a Canvas object:

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "I/O Example");
  c.start();
  c.wait();
}
~~~

Compile and run. A blank screen should appear.

Now, if you've taken a look at the [TSGL API documentation](http://calvin-cs.github.io/TSGL/html/index.html) for Canvas, you may have seen that it has two methods: [bindToButton()](http://calvin-cs.github.io/TSGL/html/classtsgl_1_1_canvas.html#a26f2f1acf2b80eee95e42bc13dbc7600) and [bindToScroll()](http://calvin-cs.github.io/TSGL/html/classtsgl_1_1_canvas.html#aecd3d94790d2e660db380a5e951ae394).

These two methods are where the I/O magic happens.

They allow us to bind keys, mouse buttons, and the mouse scroll wheel to a Canvas which will allow us to have I/O capabilities in our animations.

But how do these methods work?

Well, before we answer that, allow us to explain a crucial aspect of these methods: Lambda functions.

Taken from http://en.cppreference.com/w/cpp/language/lambda, a Lambda function:
"Constructs a closure: an unnamed function object capable of capturing variables in scope."

What does that mean?

It is essentially an unnamed function that can access any variables that are in scope, and that can be passed as an argument to another function.

"Okay, great" you may be thinking "but how does this relate to the methods?"

In order to handle mouse and/or keyboard I/O, the ```bindToButton()``` and ```bindToScroll()``` methods each take a Lambda function as an argument. In the ```bindToButton()``` method, the Lambda function is associated with a keyboard key or mouse button and an action, which are also passed as arguments. In the ```bindToScroll()``` method, the Lambda function is the only argument.

Let's look at an example to make it clear:

~~~{.cpp}
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
  Rectangle rec(25, 250, 40, 30, RED);
  c.add( &rec );
  while(c.isOpen()) {
    c.sleep();
    if(leftMouseButtonPressed) {
      rec.setColor(GREEN);
    } else {
      rec.setColor(RED);
    }
  }
  c.wait();
}
~~~

Recompile and run. A rectangle should appear in red on the screen. Click the screen with the left mouse button. The color of the rectangle should change to green.

The ```bindToButton()``` method takes in three parameters:
* A key or button to bind to (e.g. ```TSGL_A``` for the 'A' key).
* An event associated with that key (```TSGL_PRESS``` for when it's pressed, ```TSGL_RELEASE``` when it is released).
* An unnamed (Lambda) function that will be passed to ```bindToButton()``` when that method is invoked.

Allow us to explain Lambda functions in a little more depth.

Variables inside the brackets are variables defined outside of the function that are in its scope, and that are being passed into the function. The ampersand makes the variable a reference parameter. In a nutshell, that means that whatever you do to that parameter in the function will affect the variable being passed as the parameter.  So, if we passed a boolean ```switch``` as the parameter and it has a value of ```false``` before the function runs and inside the function, we change the value of the parameter to ```true```, then when the function finishes, the variable ```switch``` will have ```true``` as its value instead of ```false```.

The variables *MUST* be passed as reference parameters.

Passing reference variables to the unnamed Lambda function is called "capturing" the parameters.

Now, in the definition of the Lambda function, you can alter the reference parameters so that whenever the Lambda function is invoked the reference parameters are changed to values you want to assign to them.

The mouse click and release event are bound to the Lambda functions that set a boolean variable to true and false (respectively).

"Bound" means that the corresponding Lambda function will be invoked once the action occurs:

* Once the mouse has been clicked, the Lambda function bound to the left mouse button click event will be invoked and the boolean ```leftMouseButtonPressed``` changes to true.
* Once it has been released, the Lambda function bound to the left mouse button click event will be invoked and the boolean ```leftMouseButtonPressed``` changes to false.

As you can see, whenever the left mouse button is clicked, the boolean variable ```leftMouseButtonPressed``` is set to true and when the left mouse button is released it is set to false.

Now, how do we get the Canvas to draw stuff once the mouse button is clicked?

In the animation loop shown below, if ```leftMouseButtonPressed``` has been set to ```true``` then we can have the Canvas draw a circle that is centered by the mouse's x and y-coordinates:

~~~{.cpp}
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
  Rectangle rec(25, 250, 40, 30, RED);
  Circle circle(0, 0, 20, BLACK);
  c.add( &rec ); c.add( &circle );
  while(c.isOpen()) {
    c.sleep();
    int x = c.getMouseX(), y = c.getMouseY();  //Store the x and y-coordinates of the mouse
    if(leftMouseButtonPressed) {
      rec.setColor(GREEN);
      circle.setCenter(x, y);
    } else {
      rec.setColor(RED);
    }
  }
  c.wait();
}
~~~

Recompile and run the code. Now, click on the left mouse button while the mouse is on the screen. The black, filled-in circle should appear below your mouse cursor!

You can even hold down the left mouse button and drag the mouse over the screen and the black circle will follow your mouse!

How about an example using the ```bindToScroll()``` method instead?

Let's take a look:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "I/O Example");
  c.start();
  c.setBackgroundColor(WHITE);
  //These variables are in the scope of the Lambda function.
  //They can be passed in.
  int circleX = 250, circleY = 250;

  c.bindToScroll([&circleY](double dx, double dy) {
    if(dy == 1.0) { //If you move the scroll wheel down...
      circleY -= 50;  //Decrement the circle's y-coordinate.
    } else { //Else...
      circleY += 50;  //Increment the coordinate.
    }
  });

  //Drawing loop
  Circle circle(circleX, circleY, 50);
  c.add( &circle );
  while(c.isOpen()) {
    c.sleep();
    circle.setCenter(circleX, circleY);
  }
  c.wait();
}
~~~

Recompile and run. A black circle should appear on the screen. Scroll up to make the circle go up, scroll down to make the circle go down.

Allow us to explain this example.

The bindToScroll() method takes in a Lambda function, which in turn, takes in two parameters:

* dx, which is an x parameter whose value is set when the mouse is scrolled.
* dy, which is a y parameter whose value is set when the mouse is scrolled.

The parameters dx and dy contain the x and y coordinates of the mouse when the event occurs — when the method is invoked — and can be used to access those coordinates within the body of the method.

The ```bindToScroll()``` method MUST take in those two parameters, even if one or both parameters are not used.

In sum, in order to do animations with I/O capabilities, you need to use ```bindToButton()``` for keyboard and mouse button events and ```bindToScroll()``` for mouse scroll wheel events.

See [Keynums.h](http://calvin-cs.github.io/TSGL/html/_keynums_8h_source.html) for constants that are mapped to specific keys on the keyboard as well as are mapped to the mouse buttons and scroll wheel.

The CartesianCanvas handles I/O in exactly the same way as the standard Canvas does (since it is a subclass of the Canvas class).

That completes this tutorial!

Next up, in [[Command-line arguments]], we take a look at using command-line arguments in our animations.
