/* testSpectrogram.cpp */

#include <tsgl.h>
using namespace tsgl;

void spectrogramFunction(Canvas& can, std::string fname) {
    const int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
    can.drawImage(fname, 0, 0, cww, cwh);
    Spectrogram sp(HORIZONTAL,500);
    #pragma omp parallel for
    for (int j = 0; j < cwh; ++j) {
      if (can.getIsOpen()) {
        can.sleep();
        for (int i = 0; i < cww; ++i) {
          ColorHSV hsv = can.getPoint(i,j);
          if (hsv.H == hsv.H) //Check for NAN
            sp.update(MAX_COLOR*hsv.H/6,1.0f,0.8f);
        }
        sp.draw((float)j/cwh);
      }
    }
    sp.finish();
}

int main(int argc, char* argv[]) {
    std::string fname = argc > 1 ? argv[1] : "../assets/pics/colorful_cars.jpg";
    int w, h;
    TextureHandler::getDimensions(fname,w,h);
    Canvas c(-1, Canvas::getDisplayHeight()-h, w, h ,"");
    c.start();
    spectrogramFunction(c,fname);
    c.wait();
}
