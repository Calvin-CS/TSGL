/* testSpectrogram.cpp */

#include <tsgl.h>
using namespace tsgl;

/*!
 * \brief Draws various images on a Canvas.
 * \details Very basic test function showcasing image drawing capabilities.
 * - The first 6 images are drawn opaque.
 * - The 7th image is drawn across the entire Canvas with alpha transparency.
 * .
 * \param can Reference to the Canvas being drawn to.
 */
const int B = 16;  //Border
void spectrogramFunction(Canvas& can, Canvas& ocan, std::string fname) {
    const int ow = ocan.getWindowWidth();
    int count[NUM_COLORS] = {0}, maxCount = 0;
    can.drawImage(fname, 0, 0, can.getWindowWidth(), can.getWindowHeight());
    for (int j = 0; j < 600; ++j) {
      can.sleepFor(0.01f);
      for (int i = 0; i < 800; ++i) {
        ColorHSV hsv = can.getPoint(i,j);
        if (hsv.H == hsv.H) { //Check for NAN
          int ihue = (MAX_COLOR*hsv.H/6);
          if (++count[ihue] > maxCount) ++maxCount;
        }
      }
      ocan.clear();
      if (maxCount > 0)
        for (int k = 0; k < MAX_COLOR; ++k)
          ocan.drawLine(B,B+k,B+((ow-2*B)*count[k])/maxCount,B+k,ColorHSV((6.0f*k)/MAX_COLOR,1.0f,1.0f));
    }
}

int main(int argc, char* argv[]) {
    std::string fname = argc > 1 ? argv[1] : std::string("../assets/colorful_cars.jpg");
    Canvas c(-1, Canvas::getDisplayHeight()-600, 800, 600 ,"");
    c.start();
    Canvas c2(-1, 0, 800, MAX_COLOR + B*2 ,"");
    c2.start();
    spectrogramFunction(c,c2,fname);
    c.wait();
    c2.wait();
}
