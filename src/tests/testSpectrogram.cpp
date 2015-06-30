/* testSpectrogram.cpp */

#include <omp.h>

#include <tsgl.h>
using namespace tsgl;

void spectrogramFunction(Canvas& can, std::string fname) {
    const int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
    can.drawImage(fname, 0, 0, cww, cwh);
    Spectrogram sp(HORIZONTAL,500);
    can.sleepFor(0.1f);
//    can.recordForNumFrames(FPS);
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      int tid = omp_get_thread_num(), nthreads = omp_get_num_threads();
      int blockSize = cwh / nthreads;
      int start = tid * blockSize;
      int end = (tid == (nthreads-1)) ? cwh : (tid+1) * blockSize;
      for (int j = start; j < end; ++j) {
        if (can.getIsOpen()) {
          can.sleep();
          for (int i = 0; i < cww; ++i) {
            ColorHSV hsv = can.getPoint(i,j);
            if (hsv.H == hsv.H) //Check for NAN
              sp.update(MAX_COLOR*hsv.H/6,1.0f,0.8f);
            can.drawPoint(i,j,ColorHSV(0.0f,0.0f,hsv.V));
          }
          sp.draw((float)(j-start)/blockSize);
        }
      }
    }
    can.drawImage(fname, 0, 0, cww, cwh);
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
