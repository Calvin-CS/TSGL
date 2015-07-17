#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

int depthtest(int xmin, int ymin, int xmax, int ymax) {
  int xd = 0, yd = 0;
  for (float d = xmax-xmin; d > 1; d=ceil(d/2), ++xd);
  for (float d = ymax-ymin; d > 1; d=ceil(d/2), ++yd);
  return ((xd > yd) ? xd : yd);
}

bool blur(Canvas& can, int xmin, int ymin, int xmax, int ymax, int&numdrawn, int depth) {
  if (depth > 0) {
    int xmid = (xmin+xmax)/2, ymid = (ymin+ymax)/2;
    blur(can,xmin,  ymin,xmid,  ymid,numdrawn, depth-1);
    blur(can,xmid+1,ymin,xmax,  ymid,numdrawn, depth-1);
    blur(can,xmin,  ymid+1,xmid,ymax,numdrawn, depth-1);
    blur(can,xmid+1,ymid+1,xmax,ymax,numdrawn, depth-1);
    return false;
  }
  ColorFloat top = Colors::blend(can.getPoint(xmin,ymin),can.getPoint(xmax,ymin));
  ColorFloat bot = Colors::blend(can.getPoint(xmin,ymax),can.getPoint(xmax,ymax));
  for (int j = ymin; j <= ymax; ++j)
    for (int i = xmin; i <= xmax; ++i)
      can.drawPoint(i,j,Colors::blend(top,bot));
  return true;
}

void blurImageFunction(Canvas& can, std::string fpath, int threads) {
  int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
  can.drawImage(fpath, 0, 0, cww, cwh);
  can.sleepFor(0.5f);
  int side = sqrt(threads);  //Square root of the number of threads, rounded down
  threads = side * side;     //Make sure the actual number of threads is a square
  #pragma omp parallel num_threads (threads)
  {
    int tid = omp_get_thread_num();
    int ndrawn = 0, xblock = cww/side, yblock = cwh/side;
    int xmin = (tid%side)*xblock, ymin = (tid/side)*yblock;
    int xmax = xmin+xblock; clamp(xmax,0,cww-1);
    int ymax = ymin+yblock; clamp(ymax,0,cwh-1);
    int depth = depthtest(xmin, ymin, xmin+xblock, ymin+yblock);
    for (bool d = false; !d; d = blur(can, xmin, ymin, xmax, ymax, ndrawn, depth--));
  }
}

int main(int argc, char* argv[]) {
  int w, h, t = (argc > 1) ? atoi(argv[1]) : omp_get_num_procs();
  std::string fname = (argc > 2) ? argv[2] : "../assets/pics/colorful_cars.jpg";
  TextureHandler::getDimensions(fname,w,h);
  Canvas c(-1, -1, w, h, "Blurring using recursive splitting", FRAME);
  c.start();
  blurImageFunction(c,fname,t);
  c.wait();
}
