Love giving commands? This is the page for you!

To begin, in the ```tests``` subfolder of the ```src``` folder of the TSGL source code there are a plethora of tests that you can launch individually.

Some of these tests take in command-line arguments. Command-line arguments are values which you pass whenever you execute a test file from the command-line (for Linux: ./testName argument1 argument2 ...). 

These arguments can change something within the code; this ranges from the width and height of the Canvas screen to the number of threads to use when drawing something.

How do we give command-line capabilities to new animations though?

We're about to find out.

***Linux/Mac users:*** Follow the steps from the previous tutorials. Name the folder "Tutorial8" and the file "command.cpp". Replace "program" in the "TARGET" line of the Makefile with "command".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial8" and the Visual Studio project "Commands". After adding the Property sheet, name the .cpp file "command.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program). 

Let's start with skeleton code:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "Command-line Example", FRAME);
  c.start();
  c.wait();
}
~~~

Compile and run. A blank gray screen should appear.

Now, how can we add the ability to take in command-line arguments? Sounds complicated.

Its actually fairly simple:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main(int argc, char * argv[]) {
  Canvas c(0, 0, 500, 500, "Command-line Example", FRAME);
  c.start();
  c.wait();
}
~~~

Notice how we add parameters to the main method? This is the standard way to receive arguments from the command-line. The ```argc``` parameter is the number of arguments passed and the ```argv``` parameter is the array containing the arguments (with ```argv[0]``` being the name of the program). 

Here's a way to get the arguments passed from the command-line and into the code:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main(int argc, char * argv[]) {
  int width = atoi(argv[1]), height = atoi(argv[2]);  //Get the width and height from the command-line
  Canvas c(0, 0, width, height, "Command-line Example", FRAME);  //Pass them as parameters
  c.start();
  c.wait();
}
~~~

Since the types of elements in ```argv``` are char pointers, we need to convert them into integers in order for this example to use them as the width and the height of the Canvas.

```atoi()``` converts alphabet characters into integers. 

Compile the code. Because of the way that we have added command-line argument capabilities to our code, we MUST pass two of them, or a segmentation fault (a run-time error) will occur. (We will fix this shortly.)

Attempting to run the code without command-line arguments now will trigger a segmentation fault (an error). 

To run a program with command-line arguments depends on your system's OS. There are videos that can show you how to run an individual test file in the TSGL bin folder from the command-line:

* Linux/Mac: https://www.youtube.com/watch?v=ASMtIoJFJVI
* Windows: https://www.youtube.com/watch?v=9aXBKVQ4n4I

This code is being run on a Linux machine, so the tutorial will continue using this format.

When you run the code with command-line arguments, the screen should appear but in a different size.

But wait...what happens when someone accidentally forgets to put in a command-line argument? The code will seg fault regardless!

To avert this, we need to place a few checks in the code:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main(int argc, char * argv[]) {
  int width = (argc > 1) ? atoi(argv[1]) : 600; //Checks for command-line arguments
  int height = (argc > 2) ? atoi(argv[2]) : 800;
  Canvas c(0, 0, width, height, "Command-line Example", FRAME);  //Pass them as parameters
  c.start();
  c.wait();
}
~~~

Now compile and run the code without command-line arguments. The window should still appear without causing a seg fault.

The checks are essentially conditional operators that are laid out as follows:

~~~{.cpp}

(condition) ? expression_if_true : expression_if_false;

~~~

When execution reaches a conditional expression, its ```condition``` (usually a boolean expression) gets evaluated. If it evaluates to ```true``` then ```expression_if_true``` is the value produced by the expression. If it evaluates to ```false``` then ```expression_if_false``` is the value produced by the expression. So when this appears on the right side of an assignment statement:

~~~{.cpp}

variable = (condition) ? expression_if_true : expression_if_false;

~~~

the value assigned to ```variable``` varies depending on the value of the ```condition```, which is why it is called a "conditional expression".

In a nutshell, think of it as a one-line if-else statement.

Now, what about invalid widths and heights? We can't have a negative value or 0!

To avert this, we need one more check:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main(int argc, char * argv[]) {
  int width = (argc > 1) ? atoi(argv[1]) : 600;  //Checks for command-line arguments
  int height = (argc > 2) ? atoi(argv[2]) : 800;
  if(width <= 0 || height <= 0) {  //Check for negative or zero values
    width = height = 800;  //Set defaults if invalid widths or heights
  }
  Canvas c(0, 0, width, height, "Command-line Example", FRAME);  //Pass them as parameters
  c.start();
  c.wait();
}
~~~

Compile and run the code with a negative value or 0. The window should still appear without causing any problems.

In sum, to use command-line arguments to control your application, you *MUST* edit the ```main()``` method signature so that it looks like this:

~~~{.cpp}
int main(int argc, char * argv[]) {

}
~~~

Then, you can use the first parameter, ```argc```, to determine whether any arguments have been given on the command-line. If so, you can use the second parameter, ```argv```, to retrieve those arguments from the command-line.

You must then include any checks to make sure there have been command-line arguments passed and then make sure those arguments are valid for you animation.

That concludes this tutorial!

In the final one, [[Bringing It All Together]], we recap everything we've seen into an example with threading!