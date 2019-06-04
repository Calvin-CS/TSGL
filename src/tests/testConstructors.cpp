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
  const int PSIZE = 50;

  ColorFloat fillColor[PSIZE];
  ColorFloat outlineColor[PSIZE];
  for (unsigned i = 0; i < PSIZE; ++i) {
    fillColor[i] = Colors::randomColor(1.0f);
    outlineColor[i] = Colors::randomColor(1.0f);
  }

  can.drawArrow(5, 205, 95, 295, fillColor[3], true);
  can.drawArrow(105, 205, 195, 295, fillColor, true);

  can.drawRectangle(5, 5, 95, 95, fillColor[0], true, true);
  can.drawRectangle(5, 105, 95, 195, fillColor[0], true, false);
  can.drawRectangle(5, 205, 95, 295, fillColor[0], false, true);
  can.drawRectangle(5, 305, 95, 395, fillColor[0], false, false);

  can.drawRectangle(105, 5, 195, 95, fillColor, true, true);
  can.drawRectangle(105, 105, 195, 195, fillColor, true, false);
  can.drawRectangle(105, 205, 195, 295, fillColor, false, true);
  can.drawRectangle(105, 305, 195, 395, fillColor, false, false);

  can.drawRectangle(205, 5, 295, 95, fillColor[0], outlineColor[0], true, true);
  can.drawRectangle(205, 105, 295, 195, fillColor[0], outlineColor[0], true, false);
  can.drawRectangle(205, 205, 295, 295, fillColor[0], outlineColor[0], false, true);
  can.drawRectangle(205, 305, 295, 395, fillColor[0], outlineColor[0], false, false);

  can.drawRectangle(305, 5, 395, 95, fillColor, outlineColor[0], true, true);
  can.drawRectangle(305, 105, 395, 195, fillColor, outlineColor[0], true, false);
  can.drawRectangle(305, 205, 395, 295, fillColor, outlineColor[0], false, true);
  can.drawRectangle(305, 305, 395, 395, fillColor, outlineColor[0], false, false);

  can.drawRectangle(405, 5, 495, 95, fillColor[0], outlineColor, true, true);
  can.drawRectangle(405, 105, 495, 195, fillColor[0], outlineColor, true, false);
  can.drawRectangle(405, 205, 495, 295, fillColor[0], outlineColor, false, true);
  can.drawRectangle(405, 305, 495, 395, fillColor[0], outlineColor, false, false);

  can.drawRectangle(505, 5, 595, 95, fillColor, outlineColor, true, true);
  can.drawRectangle(505, 105, 595, 195, fillColor, outlineColor, true, false);
  can.drawRectangle(505, 205, 595, 295, fillColor, outlineColor, false, true);
  can.drawRectangle(505, 305, 595, 395, fillColor, outlineColor, false, false);

  can.drawStar(50, 350, 45, 6, fillColor[0], false, 0, true, true);
  can.drawStar(50, 450, 45, 6, fillColor[0], true, 0, true, false);
  can.drawStar(50, 550, 45, 6, fillColor[0], false, 0, false, true);
  can.drawStar(50, 650, 45, 6, fillColor[0], true, 0, false, false);

  can.drawStar(150, 350, 45, 6, fillColor, false, 0, true, true);
  can.drawStar(150, 450, 45, 6, fillColor, true, 0, true, false);
  can.drawStar(150, 550, 45, 6, fillColor, false, 0, false, true);
  can.drawStar(150, 650, 45, 6, fillColor, true, 0, false, false);

  can.drawStar(250, 350, 45, 6, fillColor[0], outlineColor[0], false, 0, true, true);
  can.drawStar(250, 450, 45, 6, fillColor[0], outlineColor[0], true, 0, true, false);
  can.drawStar(250, 550, 45, 6, fillColor[0], outlineColor[0], false, 0, false, true);
  can.drawStar(250, 650, 45, 6, fillColor[0], outlineColor[0], true, 0, false, false);

  can.drawStar(350, 350, 45, 6, fillColor, outlineColor[0], false, 0, true, true);
  can.drawStar(350, 450, 45, 6, fillColor, outlineColor[0], true, 0, true, false);
  can.drawStar(350, 550, 45, 6, fillColor, outlineColor[0], false, 0, false, true);
  can.drawStar(350, 650, 45, 6, fillColor, outlineColor[0], true, 0, false, false);

  can.drawStar(450, 350, 45, 6, fillColor[0], outlineColor, false, 0, true, true);
  can.drawStar(450, 450, 45, 6, fillColor[0], outlineColor, true, 0, true, false);
  can.drawStar(450, 550, 45, 6, fillColor[0], outlineColor, false, 0, false, true);
  can.drawStar(450, 650, 45, 6, fillColor[0], outlineColor, true, 0, false, false);
  
  can.drawStar(550, 350, 45, 6, fillColor, outlineColor, false, 0, true, true);
  can.drawStar(550, 450, 45, 6, fillColor, outlineColor, true, 0, true, false);
  can.drawStar(550, 550, 45, 6, fillColor, outlineColor, false, 0, false, true);
  can.drawStar(550, 650, 45, 6, fillColor, outlineColor, true, 0, false, false);

  can.drawRegularPolygon(50, 650, 45, 6, fillColor[0], true, true);
  can.drawRegularPolygon(50, 750, 45, 6, fillColor[0], true, false);
  can.drawRegularPolygon(50, 850, 45, 6, fillColor[0], false, true);
  can.drawRegularPolygon(50, 950, 45, 6, fillColor[0], false, false);

  can.drawRegularPolygon(150, 650, 45, 6, fillColor, true, true);
  can.drawRegularPolygon(150, 750, 45, 6, fillColor, true, false);
  can.drawRegularPolygon(150, 850, 45, 6, fillColor, false, true);
  can.drawRegularPolygon(150, 950, 45, 6, fillColor, false, false);

  can.drawRegularPolygon(250, 650, 45, 6, fillColor[0], outlineColor[0], true, true);
  can.drawRegularPolygon(250, 750, 45, 6, fillColor[0], outlineColor[0], true, false);
  can.drawRegularPolygon(250, 850, 45, 6, fillColor[0], outlineColor[0], false, true);
  can.drawRegularPolygon(250, 950, 45, 6, fillColor[0], outlineColor[0], false, false);

  can.drawRegularPolygon(350, 650, 45, 6, fillColor, outlineColor[0], true, true);
  can.drawRegularPolygon(350, 750, 45, 6, fillColor, outlineColor[0], true, false);
  can.drawRegularPolygon(350, 850, 45, 6, fillColor, outlineColor[0], false, true);
  can.drawRegularPolygon(350, 950, 45, 6, fillColor, outlineColor[0], false, false);

  can.drawRegularPolygon(450, 650, 45, 6, fillColor[0], outlineColor, true, true);
  can.drawRegularPolygon(450, 750, 45, 6, fillColor[0], outlineColor, true, false);
  can.drawRegularPolygon(450, 850, 45, 6, fillColor[0], outlineColor, false, true);
  can.drawRegularPolygon(450, 950, 45, 6, fillColor[0], outlineColor, false, false);

  can.drawRegularPolygon(550, 650, 45, 6, fillColor, outlineColor, true, true);
  can.drawRegularPolygon(550, 750, 45, 6, fillColor, outlineColor, true, false);
  can.drawRegularPolygon(550, 850, 45, 6, fillColor, outlineColor, false, true);
  can.drawRegularPolygon(550, 950, 45, 6, fillColor, outlineColor, false, false);

  can.drawTriangle(50, 905, 5, 995, 95, 995, fillColor[0], true, true);
  can.drawTriangle(50, 1005, 5, 1095, 95, 1095, fillColor[0], true, false);
  can.drawTriangle(50, 1105, 5, 1195, 95, 1195, fillColor[0], false, true);
  can.drawTriangle(50, 1205, 5, 1295, 95, 1295, fillColor[0], false, false);

  can.drawTriangle(150, 905, 105, 995, 195, 995, fillColor, true, true);
  can.drawTriangle(150, 1005, 105, 1095, 195, 1095, fillColor, true, false);
  can.drawTriangle(150, 1105, 105, 1195, 195, 1195, fillColor, false, true);
  can.drawTriangle(150, 1205, 105, 1295, 195, 1295, fillColor, false, false);

  can.drawTriangle(250, 905, 205, 995, 295, 995, fillColor[0], outlineColor[0], true, true);
  can.drawTriangle(250, 1005, 205, 1095, 295, 1095, fillColor[0], outlineColor[0], true, false);
  can.drawTriangle(250, 1105, 205, 1195, 295, 1195, fillColor[0], outlineColor[0], false, true);
  can.drawTriangle(250, 1205, 205, 1295, 295, 1295, fillColor[0], outlineColor[0], false, false);

  can.drawTriangle(350, 905, 305, 995, 395, 995, fillColor, outlineColor[0], true, true);
  can.drawTriangle(350, 1005, 305, 1095, 395, 1095, fillColor, outlineColor[0], true, false);
  can.drawTriangle(350, 1105, 305, 1195, 395, 1195, fillColor, outlineColor[0], false, true);
  can.drawTriangle(350, 1205, 305, 1295, 395, 1295, fillColor, outlineColor[0], false, false);

  can.drawTriangle(450, 905, 405, 995, 495, 995, fillColor[0], outlineColor, true, true);
  can.drawTriangle(450, 1005, 405, 1095, 495, 1095, fillColor[0], outlineColor, true, false);
  can.drawTriangle(450, 1105, 405, 1195, 495, 1195, fillColor[0], outlineColor, false, true);
  can.drawTriangle(450, 1205, 405, 1295, 495, 1295, fillColor[0], outlineColor, false, false);

  can.drawTriangle(550, 905, 505, 995, 595, 995, fillColor, outlineColor, true, true);
  can.drawTriangle(550, 1005, 505, 1095, 595, 1095, fillColor, outlineColor, true, false);
  can.drawTriangle(550, 1105, 505, 1195, 595, 1195, fillColor, outlineColor, false, true);
  can.drawTriangle(550, 1205, 505, 1295, 595, 1295, fillColor, outlineColor, false, false);

  int x1[5], x2[5], x3[5], x4[5], x5[5], x6[5], x7[5], x8[5], x9[5], x10[5], x11[5], x12[5],
   x13[5], x14[5], x15[5], x16[5], x17[5], x18[5], x19[5], x20[5], x21[5], x22[5], x23[5], x24[5],
   y1[5], y2[5], y3[5], y4[5], y5[5], y6[5], y7[5], y8[5], y9[5], y10[5], y11[5], y12[5],
   y13[5], y14[5], y15[5], y16[5], y17[5], y18[5], y19[5], y20[5], y21[5], y22[5], y23[5], y24[5];

  x1[0] = x2[0] = x3[0] = x4[0] = x1[1] = x2[1] = x3[1] = x4[1] = 605;
  x5[0] = x6[0] = x7[0] = x8[0] = x5[1] = x6[1] = x7[1] = x8[1] = 705;
  x9[0] = x10[0] = x11[0] = x12[0] = x9[1] = x10[1] = x11[1] = x12[1] = 805;
  x13[0] = x14[0] = x15[0] = x16[0] = x13[1] = x14[1] = x15[1] = x16[1] = 905;
  x17[0] = x18[0] = x19[0] = x20[0] = x17[1] = x18[1] = x19[1] = x20[1] = 1005;
  x21[0] = x22[0] = x23[0] = x24[0] = x21[1] = x22[1] = x23[1] = x24[1] = 1105;

  x1[2] = x2[2] = x3[2] = x4[2] = 650;
  x5[2] = x6[2] = x7[2] = x8[2] = 750;
  x9[2] = x10[2] = x11[2] = x12[2] = 850;
  x13[2] = x14[2] = x15[2] = x16[2] = 950;
  x17[2] = x18[2] = x19[2] = x20[2] = 1050;
  x21[2] = x22[2] = x23[2] = x24[2] = 1150;

  x1[3] = x2[3] = x3[3] = x4[3] = x1[4] = x2[4] = x3[4] = x4[4] = 695;
  x5[3] = x6[3] = x7[3] = x8[3] = x5[4] = x6[4] = x7[4] = x8[4] = 795;
  x9[3] = x10[3] = x11[3] = x12[3] = x9[4] = x10[4] = x11[4] = x12[4] = 895;
  x13[3] = x14[3] = x15[3] = x16[3] = x13[4] = x14[4] = x15[4] = x16[4] = 995;
  x17[3] = x18[3] = x19[3] = x20[3] = x17[4] = x18[4] = x19[4] = x20[4] = 1095;
  x21[3] = x22[3] = x23[3] = x24[3] = x21[4] = x22[4] = x23[4] = x24[4] = 1195;

  y1[0] = y5[0] = y9[0] = y13[0] = y17[0] = y21[0] = y1[4] = y5[4] = y9[4] = y13[4] = y17[4] = y21[4] = 95;
  y2[0] = y6[0] = y10[0] = y14[0] = y18[0] = y22[0] = y2[4] = y6[4] = y10[4] = y14[4] = y18[4] = y22[4] = 195;
  y3[0] = y7[0] = y11[0] = y15[0] = y19[0] = y23[0] = y3[4] = y7[4] = y11[4] = y15[4] = y19[4] = y23[4] = 295;
  y4[0] = y8[0] = y12[0] = y16[0] = y20[0] = y24[0] = y4[4] = y8[4] = y12[4] = y16[4] = y20[4] = y24[4] = 395;

  y1[1] = y5[1] = y9[1] = y13[1] = y17[1] = y21[1] = y1[3] = y5[3] = y9[3] = y13[3] = y17[3] = y21[3] = 50;
  y2[1] = y6[1] = y10[1] = y14[1] = y18[1] = y22[1] = y2[3] = y6[3] = y10[3] = y14[3] = y18[3] = y22[3] = 150;
  y3[1] = y7[1] = y11[1] = y15[1] = y19[1] = y23[1] = y3[3] = y7[3] = y11[3] = y15[3] = y19[3] = y23[3] = 250;
  y4[1] = y8[1] = y12[1] = y16[1] = y20[1] = y24[1] = y4[3] = y8[3] = y12[3] = y16[3] = y20[3] = y24[3] = 350;


  y1[2] = y5[2] = y9[2] = y13[2] = y17[2] = y21[2]  = 5;
  y2[2] = y6[2] = y10[2] = y14[2] = y18[2] = y22[2] = 105;
  y3[2] = y7[2] = y11[2] = y15[2] = y19[2] = y23[2] = 205;
  y4[2] = y8[2] = y12[2] = y16[2] = y20[2] = y24[2] = 305;

  can.drawTriangleStrip(5, x1, y1, fillColor[0], true, true);
  can.drawTriangleStrip(5, x2, y2, fillColor[0], true, false);
  can.drawTriangleStrip(5, x3, y3, fillColor[0], false, true);
  can.drawTriangleStrip(5, x4, y4, fillColor[0], false, false);

  can.drawTriangleStrip(5, x5, y5, fillColor, true, true);
  can.drawTriangleStrip(5, x6, y6, fillColor, true, false);
  can.drawTriangleStrip(5, x7, y7, fillColor, false, true);
  can.drawTriangleStrip(5, x8, y8, fillColor, false, false);

  can.drawTriangleStrip(5, x9, y9, fillColor[0], outlineColor[0], true, true);
  can.drawTriangleStrip(5, x10, y10, fillColor[0], outlineColor[0], true, false);
  can.drawTriangleStrip(5, x11, y11, fillColor[0], outlineColor[0], false, true);
  can.drawTriangleStrip(5, x12, y12, fillColor[0], outlineColor[0], false, false);

  can.drawTriangleStrip(5, x13, y13, fillColor, outlineColor[0], true, true);
  can.drawTriangleStrip(5, x14, y14, fillColor, outlineColor[0], true, false);
  can.drawTriangleStrip(5, x15, y15, fillColor, outlineColor[0], false, true);
  can.drawTriangleStrip(5, x16, y16, fillColor, outlineColor[0], false, false);

  can.drawTriangleStrip(5, x17, y17, fillColor[0], outlineColor, true, true);
  can.drawTriangleStrip(5, x18, y18, fillColor[0], outlineColor, true, false);
  can.drawTriangleStrip(5, x19, y19, fillColor[0], outlineColor, false, true);
  can.drawTriangleStrip(5, x20, y20, fillColor[0], outlineColor, false, false);

  can.drawTriangleStrip(5, x21, y21, fillColor, outlineColor, true, true);
  can.drawTriangleStrip(5, x22, y22, fillColor, outlineColor, true, false);
  can.drawTriangleStrip(5, x23, y23, fillColor, outlineColor, false, true);
  can.drawTriangleStrip(5, x24, y24, fillColor, outlineColor, false, false);

  can.drawEllipse(650, 350, 35, 45, fillColor[0], true, true);
  can.drawEllipse(650, 450, 35, 45, fillColor[0], true, false);
  can.drawEllipse(650, 550, 35, 45, fillColor[0], false, true);
  can.drawEllipse(650, 650, 35, 45, fillColor[0], false, false);

  can.drawEllipse(750, 350, 35, 45, fillColor, true, true);
  can.drawEllipse(750, 450, 35, 45, fillColor, true, false);
  can.drawEllipse(750, 550, 35, 45, fillColor, false, true);
  can.drawEllipse(750, 650, 35, 45, fillColor, false, false);

  can.drawEllipse(850, 350, 35, 45, fillColor[0], outlineColor[0], true, true);
  can.drawEllipse(850, 450, 35, 45, fillColor[0], outlineColor[0], true, false);
  can.drawEllipse(850, 550, 35, 45, fillColor[0], outlineColor[0], false, true);
  can.drawEllipse(850, 650, 35, 45, fillColor[0], outlineColor[0], false, false);

  can.drawEllipse(950, 350, 35, 45, fillColor, outlineColor[0], true, true);
  can.drawEllipse(950, 450, 35, 45, fillColor, outlineColor[0], true, false);
  can.drawEllipse(950, 550, 35, 45, fillColor, outlineColor[0], false, true);
  can.drawEllipse(950, 650, 35, 45, fillColor, outlineColor[0], false, false);

  can.drawEllipse(1050, 350, 35, 45, fillColor[0], outlineColor, true, true);
  can.drawEllipse(1050, 450, 35, 45, fillColor[0], outlineColor, true, false);
  can.drawEllipse(1050, 550, 35, 45, fillColor[0], outlineColor, false, true);
  can.drawEllipse(1050, 650, 35, 45, fillColor[0], outlineColor, false, false);

  can.drawEllipse(1150, 350, 35, 45, fillColor, outlineColor, true, true);
  can.drawEllipse(1150, 450, 35, 45, fillColor, outlineColor, true, false);
  can.drawEllipse(1150, 550, 35, 45, fillColor, outlineColor, false, true);
  can.drawEllipse(1150, 650, 35, 45, fillColor, outlineColor, false, false);

  can.drawCircle(650, 650, 45, 45, fillColor[0], true, true);
  can.drawCircle(650, 750, 45, 45, fillColor[0], true, false);
  can.drawCircle(650, 850, 45, 45, fillColor[0], false, true);
  can.drawCircle(650, 950, 45, 45, fillColor[0], false, false);

  can.drawCircle(750, 650, 45, 45, fillColor, true, true);
  can.drawCircle(750, 750, 45, 45, fillColor, true, false);
  can.drawCircle(750, 850, 45, 45, fillColor, false, true);
  can.drawCircle(750, 950, 45, 45, fillColor, false, false);

  can.drawCircle(850, 650, 45, 45, fillColor[0], outlineColor[0], true, true);
  can.drawCircle(850, 750, 45, 45, fillColor[0], outlineColor[0], true, false);
  can.drawCircle(850, 850, 45, 45, fillColor[0], outlineColor[0], false, true);
  can.drawCircle(850, 950, 45, 45, fillColor[0], outlineColor[0], false, false);

  can.drawCircle(950, 650, 45, 45, fillColor, outlineColor[0], true, true);
  can.drawCircle(950, 750, 45, 45, fillColor, outlineColor[0], true, false);
  can.drawCircle(950, 850, 45, 45, fillColor, outlineColor[0], false, true);
  can.drawCircle(950, 950, 45, 45, fillColor, outlineColor[0], false, false);

  can.drawCircle(1050, 650, 45, 45, fillColor[0], outlineColor, true, true);
  can.drawCircle(1050, 750, 45, 45, fillColor[0], outlineColor, true, false);
  can.drawCircle(1050, 850, 45, 45, fillColor[0], outlineColor, false, true);
  can.drawCircle(1050, 950, 45, 45, fillColor[0], outlineColor, false, false);

  can.drawCircle(1150, 650, 45, 45, fillColor, outlineColor, true, true);
  can.drawCircle(1150, 750, 45, 45, fillColor, outlineColor, true, false);
  can.drawCircle(1150, 850, 45, 45, fillColor, outlineColor, false, true);
  can.drawCircle(1150, 950, 45, 45, fillColor, outlineColor, false, false);

  int x25[6], x26[6], x27[6], x28[6], x29[6], x30[6], x31[6], x32[6], x33[6], x34[6], x35[6], x36[6],
   x37[6], x38[6], x39[6], x40[6], x41[6], x42[6], x43[6], x44[6], x45[6], x46[6], x47[6], x48[6],
   y25[6], y26[6], y27[6], y28[6], y29[6], y30[6], y31[6], y32[6], y33[6], y34[6], y35[6], y36[6],
   y37[6], y38[6], y39[6], y40[6], y41[6], y42[6], y43[6], y44[6], y45[6], y46[6], y47[6], y48[6];

  x25[0] = x26[0] = x27[0] = x28[0] = x25[1] = x26[1] = x27[1] = x28[1] = 605;
  x29[0] = x30[0] = x31[0] = x32[0] = x29[1] = x30[1] = x31[1] = x32[1] = 705;
  x33[0] = x34[0] = x35[0] = x36[0] = x33[1] = x34[1] = x35[1] = x36[1] = 805;
  x37[0] = x38[0] = x39[0] = x40[0] = x37[1] = x38[1] = x39[1] = x40[1] = 905;
  x41[0] = x42[0] = x43[0] = x44[0] = x41[1] = x42[1] = x43[1] = x44[1] = 1005;
  x45[0] = x46[0] = x47[0] = x48[0] = x45[1] = x46[1] = x47[1] = x48[1] = 1105;

  x25[2] = x26[2] = x27[2] = x28[2] = x25[5] = x26[5] = x27[5] = x28[5] = 650;
  x29[2] = x30[2] = x31[2] = x32[2] = x29[5] = x30[5] = x31[5] = x32[5] = 750;
  x33[2] = x34[2] = x35[2] = x36[2] = x33[5] = x34[5] = x35[5] = x36[5] = 850;
  x37[2] = x38[2] = x39[2] = x40[2] = x37[5] = x38[5] = x39[5] = x40[5] = 950;
  x41[2] = x42[2] = x43[2] = x44[2] = x41[5] = x42[5] = x43[5] = x44[5] = 1050;
  x45[2] = x46[2] = x47[2] = x48[2] = x45[5] = x46[5] = x47[5] = x48[5] = 1150;

  x25[3] = x26[3] = x27[3] = x28[3] = x25[4] = x26[4] = x27[4] = x28[4] = 695;
  x29[3] = x30[3] = x31[3] = x32[3] = x29[4] = x30[4] = x31[4] = x32[4] = 795;
  x33[3] = x34[3] = x35[3] = x36[3] = x33[4] = x34[4] = x35[4] = x36[4] = 895;
  x37[3] = x38[3] = x39[3] = x40[3] = x37[4] = x38[4] = x39[4] = x40[4] = 995;
  x41[3] = x42[3] = x43[3] = x44[3] = x41[4] = x42[4] = x43[4] = x44[4] = 1095;
  x45[3] = x46[3] = x47[3] = x48[3] = x45[4] = x46[4] = x47[4] = x48[4] = 1195;

  y25[0] = y29[0] = y33[0] = y37[0] = y41[0] = y45[0] = y25[4] = y29[4] = y33[4] = y37[4] = y41[4] = y45[4] = 995;
  y26[0] = y30[0] = y34[0] = y38[0] = y42[0] = y46[0] = y26[4] = y30[4] = y34[4] = y38[4] = y42[4] = y46[4] = 1095;
  y27[0] = y31[0] = y35[0] = y39[0] = y43[0] = y47[0] = y27[4] = y31[4] = y35[4] = y39[4] = y43[4] = y47[4] = 1195;
  y28[0] = y32[0] = y36[0] = y40[0] = y44[0] = y48[0] = y28[4] = y32[4] = y36[4] = y40[4] = y44[4] = y48[4] = 1295;

  y25[1] = y29[1] = y33[1] = y37[1] = y41[1] = y45[1] = y25[3] = y29[3] = y33[3] = y37[3] = y41[3] = y45[3] = 945;
  y26[1] = y30[1] = y34[1] = y38[1] = y42[1] = y46[1] = y26[3] = y30[3] = y34[3] = y38[3] = y42[3] = y46[3] = 1045;
  y27[1] = y31[1] = y35[1] = y39[1] = y43[1] = y47[1] = y27[3] = y31[3] = y35[3] = y39[3] = y43[3] = y47[3] = 1145;
  y28[1] = y32[1] = y36[1] = y40[1] = y44[1] = y48[1] = y28[3] = y32[3] = y36[3] = y40[3] = y44[3] = y48[3] = 1245;

  y25[2] = y29[2] = y33[2] = y37[2] = y41[2] = y45[2] = 910;
  y26[2] = y30[2] = y34[2] = y38[2] = y42[2] = y46[2] = 1010;
  y27[2] = y31[2] = y35[2] = y39[2] = y43[2] = y47[2] = 1110;
  y28[2] = y32[2] = y36[2] = y40[2] = y44[2] = y48[2] = 1210;

  y25[5] = y29[5] = y33[5] = y37[5] = y41[5] = y45[5] = 965;
  y26[5] = y30[5] = y34[5] = y38[5] = y42[5] = y46[5] = 1065;
  y27[5] = y31[5] = y35[5] = y39[5] = y43[5] = y47[5] = 1165;
  y28[5] = y32[5] = y36[5] = y40[5] = y44[5] = y48[5] = 1265;

  can.drawConcavePolygon(6, x25, y25, fillColor[0], true, true);
  can.drawConcavePolygon(6, x26, y26, fillColor[0], true, false);
  can.drawConcavePolygon(6, x27, y27, fillColor[0], false, true);
  can.drawConcavePolygon(6, x28, y28, fillColor[0], false, false);

  can.drawConcavePolygon(6, x29, y29, fillColor, true, true);
  can.drawConcavePolygon(6, x30, y30, fillColor, true, false);
  can.drawConcavePolygon(6, x31, y31, fillColor, false, true);
  can.drawConcavePolygon(6, x32, y32, fillColor, false, false);

  can.drawConcavePolygon(6, x33, y33, fillColor[0], outlineColor[0], true, true);
  can.drawConcavePolygon(6, x34, y34, fillColor[0], outlineColor[0], true, false);
  can.drawConcavePolygon(6, x35, y35, fillColor[0], outlineColor[0], false, true);
  can.drawConcavePolygon(6, x36, y36, fillColor[0], outlineColor[0], false, false);

  can.drawConcavePolygon(6, x37, y37, fillColor[0], outlineColor, true, true);
  can.drawConcavePolygon(6, x38, y38, fillColor[0], outlineColor, true, false);
  can.drawConcavePolygon(6, x39, y39, fillColor[0], outlineColor, false, true);
  can.drawConcavePolygon(6, x40, y40, fillColor[0], outlineColor, false, false);

  can.drawConcavePolygon(6, x41, y41, fillColor, outlineColor[0], true, true);
  can.drawConcavePolygon(6, x42, y42, fillColor, outlineColor[0], true, false);
  can.drawConcavePolygon(6, x43, y43, fillColor, outlineColor[0], false, true);
  can.drawConcavePolygon(6, x44, y44, fillColor, outlineColor[0], false, false);

  can.drawConcavePolygon(6, x45, y45, fillColor, outlineColor, true, true);
  can.drawConcavePolygon(6, x46, y46, fillColor, outlineColor, true, false);
  can.drawConcavePolygon(6, x47, y47, fillColor, outlineColor, false, true);
  can.drawConcavePolygon(6, x48, y48, fillColor, outlineColor, false, false);

  int x49[6], x50[6], x51[6], x52[6], x53[6], x54[6], x55[6], x56[6], x57[6], x58[6], x59[6], x60[6],
   x61[6], x62[6], x63[6], x64[6], x65[6], x66[6], x67[6], x68[6], x69[6], x70[6], x71[6], x72[6],
   y49[6], y50[6], y51[6], y52[6], y53[6], y54[6], y55[6], y56[6], y57[6], y58[6], y59[6], y60[6],
   y61[6], y62[6], y63[6], y64[6], y65[6], y66[6], y67[6], y68[6], y69[6], y70[6], y71[6], y72[6];

  x49[0] = x49[1] = 5;
  x49[2] = x49[5] = 50;
  x49[3] = x49[4] = 95;

  x50[0] = x50[1] = 105;
  x50[2] = x50[5] = 150;
  x50[3] = x50[4] = 195;

  x51[0] = x51[1] = 205;
  x51[2] = x51[5] = 250;
  x51[3] = x51[4] = 295;

  x52[0] = x52[1] = 305;
  x52[2] = x52[5] = 350;
  x52[3] = x52[4] = 395;

  x53[0] = x53[1] = 405;
  x53[2] = x53[5] = 450;
  x53[3] = x53[4] = 495;

  x54[0] = x54[1] = 505;
  x54[2] = x54[5] = 550;
  x54[3] = x54[4] = 595;

  x55[0] = x55[1] = 605;
  x55[2] = x55[5] = 650;
  x55[3] = x55[4] = 695;

  x56[0] = x56[1] = 705;
  x56[2] = x56[5] = 750;
  x56[3] = x56[4] = 795;

  x57[0] = x57[1] = 805;
  x57[2] = x57[5] = 850;
  x57[3] = x57[4] = 895;

  x58[0] = x58[1] = 905;
  x58[2] = x58[5] = 950;
  x58[3] = x58[4] = 995;

  x59[0] = x59[1] = 1005;  y65[0] = y65[4] = 460;
  y65[1] = y65[3] = 430;
  y65[2] = 405;
  y65[5] = 495;
  x59[2] = x59[5] = 1050;
  x59[3] = x59[4] = 1095;

  x60[0] = x60[1] = 1105;
  x60[2] = x60[5] = 1150;
  x60[3] = x60[4] = 1195;

  y49[0] = y50[0] = y51[0] = y52[0] = y53[0] = y54[0] = y55[0] = y56[0] = y57[0] = y58[0] = y59[0] = y60[0] = 1260;
  y49[1] = y50[1] = y51[1] = y52[1] = y53[1] = y54[1] = y55[1] = y56[1] = y57[1] = y58[1] = y59[1] = y60[1] = 1230;
  y49[2] = y50[2] = y51[2] = y52[2] = y53[2] = y54[2] = y55[2] = y56[2] = y57[2] = y58[2] = y59[2] = y60[2] = 1205;
  y49[3] = y50[3] = y51[3] = y52[3] = y53[3] = y54[3] = y55[3] = y56[3] = y57[3] = y58[3] = y59[3] = y60[3] = 1230;
  y49[4] = y50[4] = y51[4] = y52[4] = y53[4] = y54[4] = y55[4] = y56[4] = y57[4] = y58[4] = y59[4] = y60[4] = 1260;
  y49[5] = y50[5] = y51[5] = y52[5] = y53[5] = y54[5] = y55[5] = y56[5] = y57[5] = y58[5] = y59[5] = y60[5] = 1295;

  x61[0] = x62[0] = x63[0] = x64[0] = x65[0] = x66[0] = x67[0] = x68[0] = x69[0] = x70[0] = x71[0] = x72[0] = 1205;
  x61[1] = x62[1] = x63[1] = x64[1] = x65[1] = x66[1] = x67[1] = x68[1] = x69[1] = x70[1] = x71[1] = x72[1] = 1205;
  x61[2] = x62[2] = x63[2] = x64[2] = x65[2] = x66[2] = x67[2] = x68[2] = x69[2] = x70[2] = x71[2] = x72[2] = 1250;
  x61[3] = x62[3] = x63[3] = x64[3] = x65[3] = x66[3] = x67[3] = x68[3] = x69[3] = x70[3] = x71[3] = x72[3] = 1295;
  x61[4] = x62[4] = x63[4] = x64[4] = x65[4] = x66[4] = x67[4] = x68[4] = x69[4] = x70[4] = x71[4] = x72[4] = 1295;
  x61[5] = x62[5] = x63[5] = x64[5] = x65[5] = x66[5] = x67[5] = x68[5] = x69[5] = x70[5] = x71[5] = x72[5] = 1250;

  y61[0] = y61[4] = 60;
  y61[1] = y61[3] = 30;
  y61[2] = 5;
  y61[5] = 95;

  y62[0] = y62[4] = 160;
  y62[1] = y62[3] = 130;
  y62[2] = 105;
  y62[5] = 195;

  y63[0] = y63[4] = 260;
  y63[1] = y63[3] = 230;
  y63[2] = 205;
  y63[5] = 295;

  y64[0] = y64[4] = 360;
  y64[1] = y64[3] = 330;
  y64[2] = 305;
  y64[5] = 395;
//
  y65[0] = y65[4] = 460;
  y65[1] = y65[3] = 430;
  y65[2] = 405;
  y65[5] = 495;

  y66[0] = y66[4] = 560;
  y66[1] = y66[3] = 530;
  y66[2] = 505;
  y66[5] = 595;

  y67[0] = y67[4] = 660;
  y67[1] = y67[3] = 630;
  y67[2] = 605;
  y67[5] = 695;

  y68[0] = y68[4] = 760;
  y68[1] = y68[3] = 730;
  y68[2] = 705;
  y68[5] = 795;
 //
  y69[0] = y69[4] = 860;
  y69[1] = y69[3] = 830;
  y69[2] = 805;
  y69[5] = 895;

  y70[0] = y70[4] = 960;
  y70[1] = y70[3] = 930;
  y70[2] = 905;
  y70[5] = 995;

  y71[0] = y71[4] = 1060;
  y71[1] = y71[3] = 1030;
  y71[2] = 1005;
  y71[5] = 1095;

  y72[0] = y72[4] = 1160;
  y72[1] = y72[3] = 1130;
  y72[2] = 1105;
  y72[5] = 1195;

  can.drawConvexPolygon(6, x49, y49, fillColor[0], true, true);
  can.drawConvexPolygon(6, x50, y50, fillColor[0], true, false);
  can.drawConvexPolygon(6, x51, y51, fillColor[0], false, true);
  can.drawConvexPolygon(6, x52, y52, fillColor[0], false, false);

  can.drawConvexPolygon(6, x53, y53, fillColor, true, true);
  can.drawConvexPolygon(6, x54, y54, fillColor, true, false);
  can.drawConvexPolygon(6, x55, y55, fillColor, false, true);
  can.drawConvexPolygon(6, x56, y56, fillColor, false, false);

  can.drawConvexPolygon(6, x57, y57, fillColor[0], outlineColor[0], true, true);
  can.drawConvexPolygon(6, x58, y58, fillColor[0], outlineColor[0], true, false);
  can.drawConvexPolygon(6, x59, y59, fillColor[0], outlineColor[0], false, true);
  can.drawConvexPolygon(6, x60, y60, fillColor[0], outlineColor[0], false, false);

  can.drawConvexPolygon(6, x61, y61, fillColor, outlineColor[0], true, true);
  can.drawConvexPolygon(6, x62, y62, fillColor, outlineColor[0], true, false);
  can.drawConvexPolygon(6, x63, y63, fillColor, outlineColor[0], false, true);
  can.drawConvexPolygon(6, x64, y64, fillColor, outlineColor[0], false, false);

  can.drawConvexPolygon(6, x65, y65, fillColor[0], outlineColor, true, true);
  can.drawConvexPolygon(6, x66, y66, fillColor[0], outlineColor, true, false);
  can.drawConvexPolygon(6, x67, y67, fillColor[0], outlineColor, false, true);
  can.drawConvexPolygon(6, x68, y68, fillColor[0], outlineColor, false, false);

  can.drawConvexPolygon(6, x69, y69, fillColor, outlineColor, true, true);
  can.drawConvexPolygon(6, x70, y70, fillColor, outlineColor, true, false);
  can.drawConvexPolygon(6, x71, y71, fillColor, outlineColor, false, true);
  can.drawConvexPolygon(6, x72, y72, fillColor, outlineColor, false, false); 



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
