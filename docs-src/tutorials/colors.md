##Colors

To begin, there are different scales that contain different representations of a color. In the TSGL library these scales are used: RGBA and HSVA.

On both scales, a color has four components. However, the components represent entirely different things on the two scales.

On the RGBA scale, the four components represent a color's Red, Green, Blue, and Alpha values. The Red, Green, and Blue values combine to form the color that you want. The Alpha value determines whether or not a color should be transparent and is completely optional.

On the HSVA scale, the four components represent a color's Hue, Saturation, Value, and Alpha values. The Hue refers to the tint of the color, Saturation refers to the color's brightness, and the Value refers to the intensity of the color. These three components combine to form a color on the HSVA color scale. The Alpha value is an optional component that determines whether a color is transparent or not.

Given these two scales, you're probably wondering: "How in the heck are these two scales represented in the library?"

Well, given that we are coding in C++, there are structures in the language that are known affectionately as "structs". Structs can contain data and are perfect for representing these two scales.

In the TSGL library, there are two structs used to represent a color on the RGBA scale: ColorInt && ColorFloat. They are constructed in this way:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  //ColorInts
  ColorInt black(0, 0, 0, 0);
  ColorInt white(255, 255, 255, 0);

  //ColorFloats
  ColorFloat black2(0.0, 0.0, 0.0, 0.0);
  ColorFloat white2(1.0, 1.0, 1.0, 0.0);
}
~~~

A ColorInt takes in the Red, Green, Blue, and Alpha components as parameters. Each one can be in the range 0 to 255. The Alpha component can be omitted and will be set to 255 as default if it is omitted (the color will not be transparent if the Alpha component is omitted. An Alpha value of 0 will cause the color to be completely transparent. In between those extremes will cause the color to fade.).

White has 255 as its Red, Green, and Blue values, black has 0. In between those two extremes will be the other colors (red, orange, yellow, green, blue, indigo, violet, etc.).

Similarly for a ColorFloat, it also takes in the Red, Green, Blue, and Alpha components as parameters. It represents a color on the RGBA scale in the same way as a ColorInt struct does, except it takes in floating-point numbers as parameters. Also, White has 1.0 as its Red, Green, and Blue values and black has 0.0. The Alpha component can also be omitted when creating a ColorFloat struct and it will be set to 1.0f as default (the color will not be transparent at that point. An Alpha value of 0.0 will cause the color to be completely transparent and the color will fade if its Alpha value is in between those two extremes.).

For the HSVA color scale, we have the ColorHSV struct:

~~~{.cpp}
ColorHSV minColor(0.0, 0.0, 0.0);
ColorHSV maxColor(6.0, 1.0, 1.0);
~~~

The ColorHSV struct takes in the Hue, Saturation, Value, and Alpha components. It takes in floating-point values for the parameters. The range of values for the Hue parameter is: 0.0 to 6.0. For the Saturation, Value, and Alpha components: 0.0 to 1.0.

Now, given these three structs, the question remains: "How can I use these in drawing?"

Well, as seen in the previous tutorial with shapes, you can pass a color parameter that will draw a shape or line a different color. You can create a ColorInt/ColorFloat/ColorHSV struct that creates the color that you want and then pass that as the color parameter in the draw method. Example:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  ColorInt color(30, 50, 200);
  Canvas c(0, 0, 300, 300, "Color example", FRAME);
  c.start();
  c.drawLine(30, 50, 100, 200, color);
  c.wait();
}
~~~
The code will create a ColorInt struct and then create and initialize a Canvas object. Then, it will draw a line on the Canvas passing as the color parameter the constructed ColorInt struct.

Wait a minute...in the previous tutorial, a constant was used instead of a constructed ColorFloat/ColorInt/ColorHSV struct. You're probably thinking, "what's the big idea here? Which one should I use?"

Well, right out of the box, TSGL comes with a predefined set of color constants.
