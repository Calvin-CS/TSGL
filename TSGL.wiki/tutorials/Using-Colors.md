You don't have to color inside of the lines with this library.

***Linux/Mac users:*** Follow the steps from the previous tutorials. Name the folder "Tutorial4" and the file "colors.cpp". Replace "program" in the "TARGET" line of the Makefile with "colors".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial4" and the Visual Studio project "Color". After adding the Property sheet, name the .cpp file "colors.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

To begin, there are different scales that contain different representations of a color. In the TSGL library these scales are used: RGBA and HSVA.

On both scales, a color has four components. However, the components represent entirely different things on the two scales.

On the RGBA scale, the four components represent a color's Red, Green, Blue, and Alpha values. The Red, Green, and Blue values combine to form the color that you want. The Alpha value determines whether or not a color should be transparent and is completely optional.

On the HSVA scale, the four components represent a color's Hue, Saturation, Value, and Alpha values. The Hue refers to the tint of the color, Saturation refers to the color's brightness, and the Value refers to the intensity of the color. These three components combine to form a color on the HSVA color scale. The Alpha value is an optional component that determines whether the color is transparent or not.

Given these two scales, you're probably wondering: "How in the heck are these two scales represented in the library?"

Well, given that we are coding in C++, there are structures in the language that are known affectionately as ```structs```. Structs can contain data and are perfect for representing these two scales.

In the TSGL library, there are two structs used to represent a color on the RGBA scale: ```ColorInt``` and ```ColorFloat```. They are constructed in this way:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  //ColorInts
  ColorInt red(255, 0, 0);
  ColorInt blue(0, 0, 255);
  ColorInt green(0, 255, 0);
  ColorInt yellow(255, 255, 0);
  ColorInt magenta(255, 0, 255);
  ColorInt cyan(0, 255, 255);
  ColorInt black(0, 0, 0);
  //The above colors will not be transparent
  ColorInt white(255, 255, 255, 0);  //This color will be transparent
  ColorInt color(67, 28, 200, 50); //This color will be semi-transparent


  //ColorFloats
  ColorFloat red2(1.0f, 0.0f, 0.0f);
  ColorFloat blue2(0.0f, 0.0f, 1.0f);
  ColorFloat green2(0.0f, 1.0f, 0.0f);
  ColorFloat yellow2(1.0f, 1.0f, 0.0f);
  ColorFloat magenta2(1.0f, 0.0f, 1.0f);
  ColorFloat cyan2(0.0f, 1.0f, 1.0f);
  ColorFloat black2(0.0f, 0.0f, 0.0f);
  //The above colors will not be transparent
  ColorFloat white2(1.0f, 1.0f, 1.0f, 0.0f); //This color will be transparent
  ColorFloat color2(67.0f, 28.0f, 200.0f, 50.0f); //This color will be semi-transparent
}
~~~

The ```ColorInt``` constructor takes in these parameters:

* Red component of the color.
* Green component of the color.
* Blue component of the color.
* Alpha component of the color (optional parameter; set to ```255``` by default).

Each one can be in the range from ```0``` to ```255```.

The Alpha component can be omitted and will be set to ```255``` as default if it is omitted (the color will not be transparent if the Alpha component is omitted. An Alpha value of ```0``` will cause the color to be completely transparent. In between those extremes will cause the color to fade.).

White has ```255``` as its Red, Green, and Blue values, black has ```0```. In between those two extremes will be the other colors (red, orange, yellow, green, blue, indigo, violet, etc.).

Similarly for a ```ColorFloat```, the constructor takes in these parameters:

* Red component of the color.
* Green component of the color.
* Blue component of the color.
* Alpha component of the color (optional parameter; set to ```1.0f``` by default).

The range of values for each one is from ```0.0``` to ```1.0```.

It represents a color on the RGBA scale in the same way as a ```ColorInt``` struct does, except it takes in floating-point numbers as parameters. Also, White has ```1.0``` as its Red, Green, and Blue values and black has ```0.0```.

The Alpha component can also be omitted when creating a ```ColorFloat``` struct and it will be set to ```1.0f``` as default (the color will not be transparent at that point. An Alpha value of ```0.0``` will cause the color to be completely transparent and the color will fade if its Alpha value is in between those two extremes.).

For the HSVA color scale, we have the ```ColorHSV``` struct:

~~~{.cpp}
  ColorHSV minColor(0.0, 0.0, 0.0);
  ColorHSV maxColor(6.0, 1.0, 1.0);
~~~

The ```ColorHSV``` constructor takes in these parameters:

* Hue component of the color.
* Saturation component of the color.
* Value component of the color.
* Alpha component of the color (optional parameter; set to ```1.0f``` by default).

It takes in floating-point values for the parameters. The range of values for the Hue parameter is: ```0.0``` to ```6.0```. For the Saturation, Value, and Alpha components: ```0.0``` to ```1.0```.

Now, given these three structs, the question remains: "How can I use these in drawing?"

Well, as seen in the previous tutorial with shapes, you can draw a shape or line in a non-default color by passing a color parameter. You can create a ```ColorInt/ColorFloat/ColorHSV``` struct that creates the color that you want and then pass that as the color parameter in the draw method.

Let's draw the flag of Norway as an example:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  ColorInt red(255, 0, 45);
  //This uses the blend() method from the Colors class
  ColorInt darkBlue = Colors::blend(BLUE, BLACK);
  ColorInt white(255, 255, 255);

  Canvas c(0, 0, 800, 600, "Color Example");
  c.start();
  c.setBackgroundColor(red);
  //White rectangles
  Rectangle whiteRec1(0, 200, 800, 350, white);
  Rectangle whiteRec2(250, 0, 400, 600, white);
  c.add( &whiteRec1 ); c.add( &whiteRec2 );
  //Blue rectangles
  Rectangle blueRec1(285, 0, 365, 600, darkBlue);
  Rectangle blueRec2(0, 240, 800, 310, darkBlue);
  c.add( &blueRec1 ); c.add( &blueRec2 );
  c.wait();
}
~~~
Compile and run. The flag of Norway will be drawn!

The code will create ```ColorInt``` structs: ```red```, ```darkBlue```, and ```white``` and then create and initialize a Canvas object. It will then use the Canvas' ```setBackgroundColor()``` method and set the background color to ```red```. Then, it will use ```darkBlue``` and ```white``` and ```Rectangle()``` to draw the rectangles of the flag.

```setBackgroundColor()``` takes in only one parameter, a color to use for the background.

You may have also noticed that another method, ```blend()```, is being used to blend ```BLUE``` and ```BLACK``` together in order to create ```darkBlue```. That method is located in the ```Colors``` class and takes in these parameters:

* The first color to blend (```BLUE```, can also be a constructed ```ColorFloat```).
* The second color to blend (```BLACK```, can also be a constructed ```ColorFloat```).
* The bias to use in blending (optional parameter; set to ```0.5f``` by default).

You can construct and pass ```ColorFloat``` and ```ColorHSV``` structs in the same manner.

But wait...in the previous tutorial, a constant was used instead of a constructed ```ColorFloat/ColorInt/ColorHSV``` struct. You're probably thinking, "what's the big idea here? Which one should I use?"

Well, right out of the box, TSGL also comes with a predefined set of color constants. These constants are RGBA ```ColorFloat``` structs that already have values that are defined for certain colors. They are:

* ```BLACK```
* ```DARKGRAY```
* ```LIGHTGRAY```
* ```GRAY```
* ```WHITE```
* ```RED```
* ```ORANGE```
* ```YELLOW```
* ```GREEN```
* ```BLUE```
* ```PURPLE```
* ```MAGENTA```
* ```LIME```
* ```CYAN```
* ```BROWN```

You can pass a predefined color constant for the color parameter, or if you need a different color, you can construct a ```ColorInt/ColorFloat/ColorHSF``` struct for that color and pass that. Whichever is easier for you.

Now, there are also methods and operators defined in the structs that manipulate and convert between the three structs (which in turn convert between the different color scales and types of structs).

There are also methods in a class, ```Colors```, that can blend the colors of two ```ColorInt/ColorFloat/ColorHSV``` structs as well as other useful utility methods.

See the [TSGL API](http://calvin-cs.github.io/TSGL/html/annotated.html) page on [Colors.h](http://calvin-cs.github.io/TSGL/html/classtsgl_1_1_colors.html) for more information on ```blend()``` and on the predefined color constants as well as for other useful utility methods.

As we've mentioned before, there is a special kind of Canvas: The CartesianCanvas. How does that handle colors? In the exact same way as the normal Canvas does.

That concludes this tutorial!

In the next tutorial, [[Animation Loops]], you get to learn about drawing loops and animations!
