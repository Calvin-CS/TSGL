End of the line, folks!

***Linux/Mac users:*** Follow the steps from the previous tutorials. Name the folder "Tutorial9" and the file "thread.cpp". Replace "program" in the "TARGET" line of the Makefile with "thread".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial9" and the Visual Studio project "Threads". After adding the Property sheet, name the .cpp file "thread.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

We've seen how TSGL draws shapes, handles colors, text, I/O events, and command-line arguments.

Now...how exactly does threading come into play in this library?

TSGL does stand for "Thread-Safe Graphics Library."

How can we utilize threading in our drawings?

Simple. Let's first write some skeleton code:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  RasterCanvas c(100, 100, 500, 500, "Threading", FRAME);
  c.start();
  c.wait();
}

~~~

Compile and run. A black screen should appear.

When your program includes the header file ```tsgl.h```, it also includes the header file (```omp.h```) needed in order to use OpenMP library functions such as ```omp_get_num_threads()``` and ```omp_get_thread_num()``` (which gets the number of threads spawned and the id number of a thread, respectively). It also allows us to utilize the ```#pragma omp``` directive, which enables us to write blocks of code that should be executed in parallel.

We're going to use the RasterCanvas because we'll be drawing points.

Now, we've seen drawing loops in the tutorial, [[Animation Loops]], but we aren't going to be adding one to this code. Instead, we are going to do something a little differently:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

void drawTutorial(RasterCanvas & can) {

}

int main() {
  RasterCanvas c(100, 100, 500, 500, "Threading", FRAME);
  c.start();

  double startTime = omp_get_wtime();
  drawTutorial(c);

  printf("\nTime to color pixels: %f\n", omp_get_wtime() - startTime);
  c.wait();
}

~~~

Let us explain what we did differently. We added a function stub ( ```drawTutorial()``` ) that takes in a reference to a Canvas object. We also added timing statements: ```double startTime = omp_get_wtime();```, ```printf("\nTime to color pixels: %f\n", omp_get_wtime() - startTime);```. These statements will keep track of how long our drawing took when we call our ```drawTutorial()``` function.

Next, let's define the function so that it actually draws something onto the passed Canvas:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

void drawTutorial(RasterCanvas & can) {
   ColorFloat color = RED;
   for(unsigned i = 0; i < can.getWindowWidth(); i++) {
      can.sleep();
      for(unsigned j = 0; j < can.getWindowHeight(); j++) {
         can.drawPoint(i, j, color);
      }
   }
}

int main() {
  RasterCanvas c(100, 100, 500, 500, "Threading", FRAME);
  c.start();

  double startTime = omp_get_wtime();
  drawTutorial(c);

  printf("\nTime to color pixels: %f\n", omp_get_wtime() - startTime);
  c.wait();
}

~~~

Re-compile and run. The screen should now be red. It took 8.319619 seconds to color all of the pixels on our machine. (Note that we are using the ```can.sleep()``` call to slow the computation down, so that you can see the main thread coloring the pixels.)

The main function passes our created RasterCanvas, ```c```, to the ```drawTutorial()``` function. That function colors each pixel of its ```can``` parameter red, and since ```can``` is a reference parameter for our argument ```c```, this colors each pixel of ```c``` red.

So where do threads come in?

As it stands, we just have the main thread creating and initializing a Canvas object, and then coloring it red in the ```drawTutorial()``` function. To spread the work of coloring the Canvas across multiple threads, we can add a parallel block and parallel ```for``` loop to our function:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

void drawTutorial(RasterCanvas & can) {
   ColorFloat color = RED;
   #pragma omp parallel
   {
     #pragma omp for
     for(unsigned i = 0; i < can.getWindowWidth(); i++) {
        can.sleep();
        for(unsigned j = 0; j < can.getWindowHeight(); j++) {
           can.drawPoint(i, j, color);
        }
     }
   }
}

int main() {
  omp_set_num_threads(4);  //Set the number of threads to use
  RasterCanvas c(100, 100, 500, 500, "Threading", FRAME);
  c.start();

  double startTime = omp_get_wtime();
  drawTutorial(c);

  printf("\nTime to color pixels: %f\n", omp_get_wtime() - startTime);
  c.wait();
}

~~~

Re-compile and run. The screen should still be red. It took 2.080817 seconds to color all of the pixels with four threads (about 1/4 of the original time) on our machine.

Let's take a look at the parallel block:

~~~{.cpp}
   ColorFloat color = RED;
   #pragma omp parallel
   {
     #pragma omp for
     for(unsigned i = 0; i < can.getWindowWidth(); i++) {
        can.sleep();
        for(unsigned j = 0; j < can.getWindowHeight(); j++) {
           can.drawPoint(i, j, color);
        }
     }
   }
~~~

```color``` is set to ```RED```, and a parallel block is made with ```4``` threads (see the ```omp_set_num_threads()``` function in the main method before we create a Canvas).

Because of the ```omp_set_num_threads(4);``` statement in the ```main()``` function, the ```#pragma omp parallel``` directive causes the main thread to fork 3 new threads.  Each of the four threads will then perform the statements in the block that follows the ```pragma```. The ```#pragma omp for``` directive then divides up the iterations of the ```for``` loop that follows it among those four threads.  Since the inner loop colors the pixels ```RED```, all of the threads end up coloring their pixels the same color.

It also speeds up the coloring of the pixels in proportion to the number of threads being used; in this case, four threads were used, so the time to color all of the pixels red was about 1/4 of the original time.

This is nice and all....but we can't really visualize the four threads coloring different pixels. We know they are coloring them ```RED```, but which ones?

Let's add some more code to better visualize this:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

void drawTutorial(RasterCanvas & can) {
  //Parallel block
  #pragma omp parallel
  {
    //Current thread id & number of threads
    float tid = omp_get_thread_num();

    //Give it a color
    ColorFloat color = Colors::highContrastColor(tid);

    //Color the pixels
    #pragma omp for
    for(unsigned i = 0; i < can.getWindowWidth(); i++) {
        can.sleep();
        for(unsigned j = 0; j < can.getWindowHeight(); j++) {
            can.drawPoint(i, j, color);
          }
    }
  }
}

int main() {
  omp_set_num_threads(4);  //Set the number of threads to use
  RasterCanvas c(100, 100, 500, 500, "Threading", FRAME);
  c.start();

  double startTime = omp_get_wtime();
  drawTutorial(c);

  printf("\nTime to color pixels: %f\n", omp_get_wtime() - startTime);
  c.wait();
}

~~~
Recompile and run. There should now be four differently colored sections! (It took 2.079545 seconds to color the the screen four different colors on our machine!)

Since the changes were made only to ```drawTutorial()```, let's take a closer look at that function:

~~~{.cpp}
void drawTutorial(RasterCanvas & can) {
  //Parallel block
  #pragma omp parallel
  {
    //Current thread id & number of threads
    float tid = omp_get_thread_num();

    //Give it a color
    ColorFloat color = Colors::highContrastColor(tid);

    //Color the pixels
    #pragma omp for
    for(unsigned i = 0; i < can.getWindowWidth(); i++) {
        can.sleep();
        for(unsigned j = 0; j < can.getWindowHeight(); j++) {
            can.drawPoint(i, j, color);
          }
    }
  }
}
~~~

This function allows us to see the threads coloring different sections of the Canvas and thereby make it ***MUCH*** easier to visualize.

Next, let's add support for command-line arguments so that we can control the number of threads being used:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

void drawTutorial(RasterCanvas & can) {
  //Parallel block
  #pragma omp parallel
  {
    //Current thread id & number of threads
    float tid = omp_get_thread_num();

    //Give it a color
    ColorFloat color = Colors::highContrastColor(tid);

    //Color the pixels
    #pragma omp for
    for(unsigned i = 0; i < can.getWindowWidth(); i++) {
        can.sleep();
        for(unsigned j = 0; j < can.getWindowHeight(); j++) {
            can.drawPoint(i, j, color);
          }
    }
  }
}

//Adding command-line argument support...
int main(int argc, char * argv[]) {
  int width = (argc > 1) ? atoi(argv[1]) : 700;
  int height = (argc > 2) ? atoi(argv[2]) : 800;
  if(width <= 0 || height <= 0) {
    width = height = 900;
  }
  //Threads...
  int threads = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  if(threads <= 0) {
    //omp_get_num_procs() gets the number of available processors on your system
    threads = omp_get_num_procs();
  }
  omp_set_num_threads(threads); //Use the number of threads passed via command-line
  RasterCanvas c(100, 100, width, height, "Threading", FRAME);
  c.start();

  double startTime = omp_get_wtime();
  drawTutorial(c);

  printf("\nTime to color pixels: %f\n", omp_get_wtime() - startTime);
  c.wait();
}

~~~

We just added support for three command-line arguments: the width and height of the Canvas, and the number of threads to use in rendering. We added checks to make sure that they were valid arguments, and then we used the first two as the width and height of the Canvas.

We are then passing the third argument, the number of threads to use, into ```omp_set_num_threads()``` so that we can use that number in rendering.

Before continuing, note that we did not use an animation loop because our ```drawTutorial()``` function is not animating anything — it is just creating an inanimate drawing on the Canvas.  We only need to use animation loops when we are animating something.

Compile and run the program, passing these command-line arguments: 700, 700, 10. There should now be 10 differently colored sections of the Canvas! (It took 0.830523 seconds to color the 10 different sections on our machine!)

Since we used 10 threads, the time should be about 1/10 of the original time. Using n threads, it should take about 1/n seconds of the original time (with one thread) to draw, provided (a) we have enough work for the threads to do, (b) the work of each thread is independent of the others, and (c) there are enough cores available for them to run independently.

In sum, OpenMP makes it easy to add multithreading to speed up a sequential program, and TSGL can be used to visualize what each thread is doing. You can use command-line arguments to control the number of threads; you can also a function that draws a particular drawing (with or without multithreading).

That concludes this tutorial!

We hope that these tutorial pages help you get started with TSGL, and that you make some amazing creations with this library!
