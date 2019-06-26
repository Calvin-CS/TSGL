/*
 * testConcavePolygon.cpp
 *
 * Usage: ./testConcavePolygon <width> <height>
 */

#include <tsgl.h>
#include "Util.h"  //Constants

using namespace tsgl;

/**
 * \brief Draw Concave polygons, which have one or more interior angles > 180
 * \note See http://www.mathopenref.com/polygonconcave.html
 * \details
 * - Initialize a constant \b PSIZE.
 * - Have four arrays of integers \b x, \b y, \b xx, and \b yy and set them to have size \b PSIZE.
 * - Create an empty array of colors of size \b PSIZE and fill it with random colors.
 * - Fill the arrays of integers, \b x and \b y with specific values (which will then be used in the while loop to draw a Concave polygon).
 * - Fill the other arrays of integers, \b xx and \b yy, with specific values.
 * - While the Canvas is open:
 *   - Sleep the internal timer of the Canvas until the Canvas is ready to draw.
 *   - Draw a Concave polygon on the Canvas and pass \b PSIZE, the arrays \b x and \b y, and the array of colors as arguments.
 *   - Draw another Concave polygon on the Canvas and pass \b PSIZE, the arrays \b x and \b y, and the array of colors as arguments.
 *   .
 * .
 * \param can Reference to the Canvas being drawn to.
 */
void constructorFunction(Canvas& can) {
  // srand(time(NULL));
  const int PSIZE = 50;

  ColorFloat fillColor[PSIZE];
  ColorFloat outlineColor[PSIZE];
  for (unsigned i = 0; i < PSIZE; ++i) {
    fillColor[i] = Colors::randomColor(1.0f);
    outlineColor[i] = Colors::randomColor(1.0f);
  }

  can.drawRectangle(15, 5, 70, 95, fillColor[0], true);
  can.drawRectangle(115, 5, 70, 95, fillColor[0], false);
  can.drawRectangle(215, 5, 70, 95, fillColor, true);
  can.drawRectangle(315, 5, 70, 95, fillColor, false);

  can.drawRectangle(415, 5, 70, 95, fillColor[0], outlineColor[0]);
  can.drawRectangle(515, 5, 70, 95, fillColor, outlineColor[0]);
  can.drawRectangle(615, 5, 70, 95, fillColor[0], outlineColor);
  can.drawRectangle(715, 5, 70, 95, fillColor, outlineColor);

  can.drawSquare(5, 105, 90, fillColor[0], true);
  can.drawSquare(105, 105, 90, fillColor[0], false);
  can.drawSquare(205, 105, 90, fillColor, true);
  can.drawSquare(305, 105, 90, fillColor, false);

  can.drawSquare(405, 105, 90, fillColor[0], outlineColor[0]);
  can.drawSquare(505, 105, 90, fillColor, outlineColor[0]);
  can.drawSquare(605, 105, 90, fillColor[0], outlineColor);
  can.drawSquare(705, 105, 90, fillColor, outlineColor);

  can.drawStar(50, 250, 45, 6, fillColor[0], false, 0, true);
  can.drawStar(150, 250, 45, 6, fillColor[0], true, 0, false);
  can.drawStar(250, 250, 45, 6, fillColor, false, 0, true);
  can.drawStar(350, 250, 45, 6, fillColor, true, 0, false);

  can.drawStar(450, 250, 45, 6, fillColor[0], outlineColor[0], false, 0);
  can.drawStar(550, 250, 45, 6, fillColor, outlineColor[0], false, 0);
  can.drawStar(650, 250, 45, 6, fillColor[0], outlineColor, false, 0);  
  can.drawStar(750, 250, 45, 6, fillColor, outlineColor, false, 0);

  can.drawRegularPolygon(50, 350, 45, 6, fillColor[0], true);
  can.drawRegularPolygon(150, 350, 45, 6, fillColor[0], false);
  can.drawRegularPolygon(250, 350, 45, 6, fillColor, true);
  can.drawRegularPolygon(350, 350, 45, 6, fillColor, false);

  can.drawRegularPolygon(450, 350, 45, 6, fillColor[0], outlineColor[0]);
  can.drawRegularPolygon(550, 350, 45, 6, fillColor, outlineColor[0]);
  can.drawRegularPolygon(650, 350, 45, 6, fillColor[0], outlineColor);
  can.drawRegularPolygon(750, 350, 45, 6, fillColor, outlineColor);

  can.drawEllipse(50, 450, 35, 45, fillColor[0], true);
  can.drawEllipse(150, 450, 35, 45, fillColor[0], false);
  can.drawEllipse(250, 450, 35, 45, fillColor, true);
  can.drawEllipse(350, 450, 35, 45, fillColor, false);

  can.drawEllipse(450, 450, 35, 45, fillColor[0], outlineColor[0]);
  can.drawEllipse(550, 450, 35, 45, fillColor, outlineColor[0]);
  can.drawEllipse(650, 450, 35, 45, fillColor[0], outlineColor);
  can.drawEllipse(750, 450, 35, 45, fillColor, outlineColor);

  can.drawTriangle(50, 505, 5, 595, 95, 595, fillColor[0], true);
  can.drawTriangle(150, 505, 105, 595, 195, 595, fillColor[0], false);
  can.drawTriangle(250, 505, 205, 595, 295, 595, fillColor, true);
  can.drawTriangle(350, 505, 305, 595, 395, 595, fillColor, false);

  can.drawTriangle(450, 505, 405, 595, 495, 595, fillColor[0], outlineColor[0]);
  can.drawTriangle(550, 505, 505, 595, 595, 595, fillColor, outlineColor[0]);
  can.drawTriangle(650, 505, 605, 595, 695, 595, fillColor[0], outlineColor);
  can.drawTriangle(750, 505, 705, 595, 795, 595, fillColor, outlineColor);

  can.drawCircle(50, 650, 45, fillColor[0], true);
  can.drawCircle(150, 650, 45, fillColor[0], false);
  can.drawCircle(250, 650, 45, fillColor, true);
  can.drawCircle(350, 650, 45, fillColor, false);

  can.drawCircle(450, 650, 45, fillColor[0], outlineColor[0]);
  can.drawCircle(550, 650, 45, fillColor, outlineColor[0]);
  can.drawCircle(650, 650, 45, fillColor[0], outlineColor);
  can.drawCircle(750, 650, 45, fillColor, outlineColor);

  int x1[5], x2[5], x3[5], x4[5], x5[5], x6[5], x7[5], x8[5], y1[5], y2[5], y3[5], y4[5], y5[5], y6[5], y7[5], y8[5];

  x1[0] = x1[1] = 5;
  x1[2] = 50;
  x1[3] = x1[4] = 95;

  x2[0] = x2[1] = 105;
  x2[2] = 150;
  x2[3] = x2[4] = 195;

  x3[0] = x3[1] = 205;
  x3[2] = 250;
  x3[3] = x3[4] = 295;

  x4[0] = x4[1] = 305;
  x4[2] = 350;
  x4[3] = x4[4] = 395;

  x5[0] = x5[1] = 405;
  x5[2] = 450;
  x5[3] = x5[4] = 495;

  x6[0] = x6[1] = 505;
  x6[2] = 550;
  x6[3] = x6[4] = 595;

  x7[0] = x7[1] = 605;
  x7[2] = 650;
  x7[3] = x7[4] = 695;

  x8[0] = x8[1] = 705;
  x8[2] = 750;
  x8[3] = x8[4] = 795;

  y1[0] = y2[0] = y3[0] = y4[0] = y5[0] = y6[0] = y7[0] = y8[0] = 795;
  y1[1] = y2[1] = y3[1] = y4[1] = y5[1] = y6[1] = y7[1] = y8[1] = 750;
  y1[2] = y2[2] = y3[2] = y4[2] = y5[2] = y6[2] = y7[2] = y8[2] = 705;
  y1[3] = y2[3] = y3[3] = y4[3] = y5[3] = y6[3] = y7[3] = y8[3] = 750;
  y1[4] = y2[4] = y3[4] = y4[4] = y5[4] = y6[4] = y7[4] = y8[4] = 795;

  can.drawTriangleStrip(5, x1, y1, fillColor[0], true);
  can.drawTriangleStrip(5, x2, y2, fillColor[0], false);
  can.drawTriangleStrip(5, x3, y3, fillColor, true);
  can.drawTriangleStrip(5, x4, y4, fillColor, false);

  can.drawTriangleStrip(5, x5, y5, fillColor[0], outlineColor[0]);
  can.drawTriangleStrip(5, x6, y6, fillColor, outlineColor[0]);
  can.drawTriangleStrip(5, x7, y7, fillColor[0], outlineColor);
  can.drawTriangleStrip(5, x8, y8, fillColor, outlineColor);

  int x9[6], x10[6], x11[6], x12[6], x13[6], x14[6], x15[6], x16[6],
   y9[6], y10[6], y11[6], y12[6], y13[6], y14[6], y15[6], y16[6];

  x9[0] = x9[1] = 5;
  x9[2] = x9[5] = 50;
  x9[3] = x9[4] = 95;

  x10[0] = x10[1] = 105;
  x10[2] = x10[5] = 150;
  x10[3] = x10[4] = 195;

  x11[0] = x11[1] = 205;
  x11[2] = x11[5] = 250;
  x11[3] = x11[4] = 295;

  x12[0] = x12[1] = 305;
  x12[2] = x12[5] = 350;
  x12[3] = x12[4] = 395;

  x13[0] = x13[1] = 405;
  x13[2] = x13[5] = 450;
  x13[3] = x13[4] = 495;

  x14[0] = x14[1] = 505;
  x14[2] = x14[5] = 550;
  x14[3] = x14[4] = 595;

  x15[0] = x15[1] = 605;
  x15[2] = x15[5] = 650;
  x15[3] = x15[4] = 695;

  x16[0] = x16[1] = 705;
  x16[2] = x16[5] = 750;
  x16[3] = x16[4] = 795;

  y9[0] = y10[0] = y11[0] = y12[0] = y13[0] = y14[0] = y15[0] = y16[0] = 870;
  y9[1] = y10[1] = y11[1] = y12[1] = y13[1] = y14[1] = y15[1] = y16[1] = 840;
  y9[2] = y10[2] = y11[2] = y12[2] = y13[2] = y14[2] = y15[2] = y16[2] = 805;
  y9[3] = y10[3] = y11[3] = y12[3] = y13[3] = y14[3] = y15[3] = y16[3] = 830;
  y9[4] = y10[4] = y11[4] = y12[4] = y13[4] = y14[4] = y15[4] = y16[4] = 860;
  y9[5] = y10[5] = y11[5] = y12[5] = y13[5] = y14[5] = y15[5] = y16[5] = 895;

  can.drawConvexPolygon(6, x9, y9, fillColor[0], true);
  can.drawConvexPolygon(6, x10, y10, fillColor[0], false);
  can.drawConvexPolygon(6, x11, y11, fillColor, true);
  can.drawConvexPolygon(6, x12, y12, fillColor, false);

  can.drawConvexPolygon(6, x13, y13, fillColor[0], outlineColor[0]);
  can.drawConvexPolygon(6, x14, y14, fillColor[0], outlineColor);
  can.drawConvexPolygon(6, x15, y15, fillColor, outlineColor[0]);
  can.drawConvexPolygon(6, x16, y16, fillColor, outlineColor);

  int x17[6], x18[6], x19[6], x20[6], x21[6], x22[6], x23[6], x24[6],
   y17[6], y18[6], y19[6], y20[6], y21[6], y22[6], y23[6], y24[6];

  x17[0] = x17[1] = 5;
  x17[2] = x17[5] = 50;
  x17[3] = x17[4] = 95;

  x18[0] = x18[1] = 105;
  x18[2] = x18[5] = 150;
  x18[3] = x18[4] = 195;

  x19[0] = x19[1] = 205;
  x19[2] = x19[5] = 250;
  x19[3] = x19[4] = 295;

  x20[0] = x20[1] = 305;
  x20[2] = x20[5] = 350;
  x20[3] = x20[4] = 395;

  x21[0] = x21[1] = 405;
  x21[2] = x21[5] = 450;
  x21[3] = x21[4] = 495;

  x22[0] = x22[1] = 505;
  x22[2] = x22[5] = 550;
  x22[3] = x22[4] = 595;

  x23[0] = x23[1] = 605;
  x23[2] = x23[5] = 650;
  x23[3] = x23[4] = 695;

  x24[0] = x24[1] = 705;
  x24[2] = x24[5] = 750;
  x24[3] = x24[4] = 795;

  y17[0] = y18[0] = y19[0] = y20[0] = y21[0] = y22[0] = y23[0] = y24[0] = 995;
  y17[1] = y18[1] = y19[1] = y20[1] = y21[1] = y22[1] = y23[1] = y24[1] = 950;
  y17[2] = y18[2] = y19[2] = y20[2] = y21[2] = y22[2] = y23[2] = y24[2] = 905;
  y17[3] = y18[3] = y19[3] = y20[3] = y21[3] = y22[3] = y23[3] = y24[3] = 950;
  y17[4] = y18[4] = y19[4] = y20[4] = y21[4] = y22[4] = y23[4] = y24[4] = 995;
  y17[5] = y18[5] = y19[5] = y20[5] = y21[5] = y22[5] = y23[5] = y24[5] = 955;

  can.drawConcavePolygon(6, x17, y17, fillColor[0], true);
  can.drawConcavePolygon(6, x18, y18, fillColor[0], true);
  can.drawConcavePolygon(6, x19, y19, fillColor, true);
  can.drawConcavePolygon(6, x20, y20, fillColor, true);

  can.drawConcavePolygon(6, x21, y21, fillColor[0], outlineColor[0]);
  can.drawConcavePolygon(6, x22, y22, fillColor, outlineColor[0]);
  can.drawConcavePolygon(6, x23, y23, fillColor[0], outlineColor);
  can.drawConcavePolygon(6, x24, y24, fillColor, outlineColor);

  can.drawArrow(805, 5, 895, 95, fillColor[3], true);
  can.drawArrow(805, 105, 895, 195, fillColor, true);

  can.drawLine(805, 205, 895, 295, fillColor[3]);
  can.drawLine(805, 305, 895, 395, fillColor);

  int x25[5], x26[5], y25[5], y26[5];

  x25[0] = x26[0] = 820;
  x25[1] = x26[1] = 805;
  x25[2] = x26[2] = 880;
  x25[3] = x26[3] = 895;
  x25[4] = x26[4] = 870;

  y25[0] = 450;
  y25[1] = 405;
  y25[2] = 420;
  y25[3] = 460;
  y25[4] = 495;

  y26[0] = 550;
  y26[1] = 505;
  y26[2] = 520;
  y26[3] = 560;
  y26[4] = 595;

  can.drawPolyline(5, x25, y25, fillColor[3]);
  can.drawPolyline(5, x26, y26, outlineColor);

  while (can.isOpen()) {  // Checks to see if the window has been closed
    can.sleep();

  }
}

//Takes command-line arguments for the width and height of the screen
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
    int h = (argc > 2) ? atoi(argv[2]) : w;
    if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
      w = h = 960;              //If not, set the width and height to a default value
    Canvas c(-1, -1, w, h, "Constructors");
    c.setBackgroundColor(WHITE);
    c.run(constructorFunction);
}
