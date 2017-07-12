/*
 * testFunction.cpp
 *
 * Usage: ./testFunction <width> <height>
 */

#include <tsgl.h>

using namespace tsgl;

/*!
 * \brief Draws the outputs of some functions using CartesianCanvas.
 * \details
 * - Draw axes through the origin, with spacing 1.0 between x ticks and 5.0 between y ticks.
 * - Initialize a CosineFunction, and draw it using the currently rendered area of the CartesianCanvas.
 * - Initialize a PowerFunction with argument 2 (square), and draw it using
 *   the currently rendered area of the CartesianCanvas.
 * - Declare a new function that computes some bizarre polynomial.
 * - Initialize the new function, and draw it using the currently rendered area of the CartesianCanvas.
 * .
 * \param can Reference to the CartesianCanvas being drawn to.
 */
void functionFunction(CartesianCanvas& can) {
    can.drawAxes(0, 0, 1, 5);

    CosineFunction function1;
    can.drawFunction(function1,FRAME/5);

    PowerFunction function2(2);
    can.drawFunction(function2,FRAME/5);

    class myFunction : public Function {
     public:
        long double valueAt(long double x) const {
            return 5 * pow(x, 4) + 2 * pow(x, 3) + x + 15;
        }
    };

    myFunction function3;
    can.drawFunction(function3,FRAME/5);
}

//Takes command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1.2*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : 0.75*w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 1000;             //If not, set the width and height to a default value
    Cart c(-1, -1, w, h, -5,-5,5,50, "Function Plotting");
    c.setBackgroundColor(WHITE);
    c.run(functionFunction);
}
