/*
 * testText.cpp
 *
 * Usage: ./testSpectrum <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws some text on a Canvas.
 * \details
 * - We declare some colors to use for drawing.
 * - We changed it so that now a default text font is loaded if one is not specified.
 * - We draw a few lines of text in various colors using drawText().
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void textFunction(Canvas& can) {
    ColorFloat RED = ColorFloat(1.0, 0.0, 0.0, 1.0);
    ColorFloat GREEN = ColorFloat(0.0, 1.0, 0.0, 1.0);
    ColorFloat BLUE = ColorFloat(0.0, 0.0, 1.0, 1.0);

    can.setBackgroundColor(WHITE);

    int FONT_SIZE = 26;

    Text * t0 = new Text("Here's to the crazy ones. The misfits. The rebels. The troublemakers.", 16, 50, FONT_SIZE, BLACK);
    Text * t1 = new Text("The round pegs in the square holes. The ones who see things differently.", 16, 150, FONT_SIZE, RED);
    Text * t2 = new Text("They're not fond of rules. And they have no respect for the status quo.", 16, 250, FONT_SIZE, GREEN);
    Text * t3 = new Text("You can quote them, disagree with them, glorify or vilify them.", 16, 350, FONT_SIZE, BLUE);
    Text * t4 = new Text("About the only thing you can't do is ignore them. Because they change things.", 16, 450, FONT_SIZE, GRAY);
    Text * t5 = new Text("They push the human race forward. And while some may see them as the crazy ones,", 16, 550, FONT_SIZE, WHITE);
    Text * t6 = new Text("we see genius. Because the people who are crazy enough", 16, 650, FONT_SIZE, WHITE);
    Text * t7 = new Text("to think they can change the world, are the ones who do.", 16, 750, FONT_SIZE, PURPLE);
    can.add( t0 ); can.add( t1 ); can.add( t2 ); can.add( t3 ); can.add( t4 ); can.add( t5 ); can.add( t6 ); can.add( t7 );

    can.wait(); //Wait until Canvas is closed...
    //Then free memory from Text pointers
    delete t0; delete t1; delete t2;
    delete t3; delete t4; delete t5;
}

//Takes command-line arguments for the width and height of the screen
//as well as for the font
int main(int argc, char * argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1300;
    int h = (argc > 2) ? atoi(argv[2]) : 800;
    if(w <= 0 || h <= 0) {  //Check validity of width and height
      w = 1300;
      h = 800;
    }
    Canvas c(-1, -1, w, h, "Text on a Canvas");
    c.run(textFunction);
}
