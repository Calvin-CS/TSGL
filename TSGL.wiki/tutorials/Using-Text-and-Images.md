Quit texting and pay attention to this tutorial!

Placing text on a Canvas is pretty straight forward. It essentially requires a font file for the rendered text and creating Text objects.

***Linux/Mac users:*** Follow the steps from the previous tutorials. Name the folder "Tutorial3" and the file "text.cpp". Replace "program" in the "TARGET" line of the Makefile with "text".

***Windows users:*** Follow the steps from the previous tutorials. Name the Solution folder "Tutorial3" and the Visual Studio project "Text". After adding the Property sheet, name the .cpp file "text.cpp".

***All three platforms:*** Follow the steps in the [[Building Programs]] page on how to compile and run the program (Linux/Mac users, this is a single-file program).

Let's start by creating a Canvas object and initializing it:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "Texture example");
  c.start();
  c.wait();
}

~~~

Alright, now let's add some text!

What should we say? What message should we broadcast to the world?

How about "Hello, World!"?

Perfect. Now, let's set our font. For the purposes of this tutorial, we have a font file already inside of our folder, ```FreeMono.ttf```. The font file (along with a plethora of others) is located in the ```freefont``` folder which is in the ```assets``` folder located in the TSGL root directory (where the ```Makefile```) is. You can copy that font file (or another if you so desire) from the ```freefont``` folder into your folder (***Linux/Mac users***) or into your Solution folder in your Visual Studio project (***Windows users***).

TSGL is compatible with FreeType fonts, but it can also use other fonts. Make certain that you have that font file (or another one like it) in your project/Solution folder so the Canvas can render the text:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "Texture example");
  c.start();
  Text hello("Hello, World!", 150, 250, 5);
  c.add( &hello );
  c.wait();
}

~~~

```Text()``` creates a text item. It takes in these parameters:

* The text to render (```Hello, World!```).
* The x-coordinate of the left bound of the text to render (```150```).
* The y-coordinate of the base of the text to render (```250```).
* The size of the text (```30```).
* The color of the text (optional parameter; set to ```BLACK``` by default).
* The name of the font file. It can also be a directory path that leads to the font file, such as: ```assets/freetype/Name_Of_Font_File.ttf``` (optional parameter; defaults to FreeSans).

Once you are ready to render the Text object, it can be added to a Canvas the same way a Shape is, using Canvas's ```add()``` method.

That is essentially how you render text onto a Canvas!

You can also render images onto a Canvas as well. The process is the same for creating an Image object: ```Image()```. It takes in these parameters:

* The file name of the image (which can also be a directory path to the image file).
* The x-coordinate of the image's left bound.
* The y-coordinate of the image's left bound.
* The width of the image.
* The height of the image.
* The alpha value of the image (optional parameter; set to ```1.0f``` by default).

The alpha value determines whether or not the image should be transparent. We will take a closer look at alpha values in the next tutorial, Using Colors.

Let's look at an example where we first draw an image and then draw text on top of it. You can get pictures from the ```pics``` folder located in the ```assets``` folder which is located in the TSGL root directory. TSGL handles ```.jpg```, ```.png```, and ```.bmp``` picture files perfectly fine. Copy over whatever picture file you would like to draw text on (either in your project folder or in your Solution folder depending on what machine you are using) and then pass the full file name into the file name parameter for ```Image()```.

For the purposes of this tutorial, we used ```background.jpg```:

~~~{.cpp}
#include <tsgl.h>
using namespace tsgl;

int main() {
  Canvas c(0, 0, 500, 500, "Texture example");
  c.start();
  //We will explain what getWindowWidth()
  //and getWindowHeight() do in the next tutorial
  Image bkgd("background.jpg", 0, 0, c.getWindowWidth(), c.getWindowHeight());
  c.setFont("FreeMono.ttf");
  Text hello("Hello, World!", 150, 250, 30);
  c.add( &bkgd );
  c.add( &hello );
  c.wait();
}

~~~
Recompile and run. The image should now appear and the text "Hello, World!" should be drawn on top of it.

In sum, you can set the font using the ```setFont()``` method before rendering the text onto the Canvas. Then, you can create the actual text using ```Text()```.

Images can be created using ```Image()```.

That concludes this tutorial!

In the next tutorial, [[Using Colors]], you get to learn about colors!
