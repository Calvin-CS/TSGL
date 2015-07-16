/* testSpectrogram.cpp */

#include <omp.h>

#include <tsgl.h>
using namespace tsgl;

/*!
 * \brief Displays a spectrogram.
 * \details Shows a spectrogram for the colors of a chosen photo.
 * - The window width and height are stored for ease of use.
 * - The image is drawn onto the Canvas.
 * - A Spectrogram object is created which will display the spectrogram.
 * - Sleep the Canvas for 1/10th of a second.
 * - Have a counter that keeps track of the number of pixels that we have checked (for their color).
 * - A parallel block is created and the process is forked.
 * - The thread id and the actual number of threads spawned are stored.
 * - Start and stopping points for the thread are calculated and stored.
 * - For the starting point to the ending point of rendering:
 *   - If the Canvas is open:
 *      - Sleep the internal timer until the next draw cycle.
 *      - For 0 to the width of the Canvas:
 *        - Get a hue color based off of the current pixel.
 *        - Check for a weird case where we can end up with a NaN error (Not a Number).
 *        - Draw the point onto the Canvas.
 *        .
 *      - Update the number of pixels checked in an parallel atomic block (to avoid conflicts).
 *      - Draw the results on the Spectrogram object.
 *     .
 *   .
 * - Have the Canvas sleep for: FRAME seconds.
 * - Draw the chosen image onto the Canvas.
 * - Close the Spectrogram object.
 * .
 * \param can Reference to the Canvas object to draw to.
 * \param fname The name of the file to get the image from.
 */
void spectrogramFunction(Canvas& can, std::string fname) {
    const int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
    can.drawImage(fname, 0, 0, cww, cwh);
    Spectrogram sp(VERTICAL,500);
    can.sleepFor(0.1f);
//    can.recordForNumFrames(FPS);
    unsigned numChecked = 0;
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
              sp.updateLocked(MAX_COLOR*hsv.H/6,1.0f,0.8f);
//              sp.updateCritical(MAX_COLOR*hsv.H/6,1.0f,0.8f);
            can.drawPoint(i,j,ColorHSV(0.0f,0.0f,hsv.V));
          }
          #pragma omp atomic
            ++numChecked;
          sp.draw((float)(1.0f*numChecked)/cwh);
        }
      }
    }
    can.sleepFor(FRAME);
    can.drawImage(fname, 0, 0, cww, cwh);
    sp.finish();
}

//Takes command-line arguments for the file name of the picture to use in the spectrogram function
int main(int argc, char* argv[]) {
    std::string fname = argc > 1 ? argv[1] : "../assets/pics/colorful_cars.jpg";
    int w, h;
    TextureHandler::getDimensions(fname,w,h);
    Canvas c(-1, Canvas::getDisplayHeight()-h, w, h ,"Spectrogram");
    c.start();
    spectrogramFunction(c,fname);
    c.wait();
}
