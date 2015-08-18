#include <tsgl.h>
using namespace tsgl;


void drawTutorial(Canvas & can, int numberOfThreads) {
  const unsigned int width = can.getWindowWidth(), height = can.getWindowHeight();
  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can] () {
    can.clear();
  });
#pragma omp parallel num_threads(numberOfThreads)
{
  float tid = omp_get_thread_num(), nthreads = omp_get_num_threads();
  ColorFloat color = Colors::highContrastColor(tid);
  unsigned bstart = tid*(width/nthreads);
  unsigned bend = (tid==nthreads) ? width-1 : bstart + width/nthreads;
  for (unsigned i = bstart; i <= bend; i++)
    for (unsigned int j = 0; j <= height; j++)
      can.drawPoint(i, j, color);
  }
}

int main(int argc, char * argv[]) {
  int width = (argc > 1) ? atoi(argv[1]) : 700;
  int height = (argc > 2) ? atoi(argv[2]) : 800;
  if (width <= 0 || height <= 0) {
    width = height = 900;
  }
  int threads = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  if(threads <= 0) {
    threads = omp_get_num_procs();
  }
  Canvas c(0, 0, width, height, "Threads", FRAME);
  c.start();
  drawTutorial(c, threads);
  c.wait();
}
