/*
 * testBlurImage.cpp
 *
 * Usage: ./testBlurImage <numThreads> <imagePath>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

int depthtest(int xmin, int ymin, int xmax, int ymax) {
  int xd = 0, yd = 0;
  for (float d = xmax-xmin; d > 1; d=ceil(d/2), ++xd);
  for (float d = ymax-ymin; d > 1; d=ceil(d/2), ++yd);
  return ((xd > yd) ? xd : yd);
}

bool blur(Background * bg, int xmin, int ymin, int xmax, int ymax, int&numdrawn, int depth) {
  if (xmin > xmax || ymin > ymax)
	  return false;
  if (depth > 0) {
    int xmid = (xmin+xmax)/2, ymid = (ymin+ymax)/2;
    blur(bg,xmin,  ymin,  xmid,ymid,numdrawn, depth-1);
    blur(bg,xmid+1,ymin,  xmax,ymid,numdrawn, depth-1);
    blur(bg,xmin,  ymid+1,xmid,ymax,numdrawn, depth-1);
    blur(bg,xmid+1,ymid+1,xmax,ymax,numdrawn, depth-1);
    return false;
  }
  ColorFloat top = Colors::blend(bg->getPixel(xmin,ymin),bg->getPixel(xmax,ymin));
  ColorFloat bot = Colors::blend(bg->getPixel(xmin,ymax),bg->getPixel(xmax,ymax));
  for (int j = ymin; j <= ymax; ++j)
    for (int i = xmin; i <= xmax; ++i)
      bg->drawPixel(i,j,Colors::blend(top,bot));
  return true;
}

void blurImageFunction(Canvas& can, std::string fpath, int threads) {
  Background * bg = can.getBackground();
  int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
  bg->drawImage(0,0,0,fpath,cww,cwh,0,0,0,1.0);
  int side = sqrt(threads);  //Square root of the number of threads, rounded down
  can.sleepFor(0.5f);
  #pragma omp parallel num_threads (side*side) //Make sure the actual number of threads is a square
  {
	side=sqrt(omp_get_num_threads());  //Verify we actually have a workable number of threads
    int tid = omp_get_thread_num();
    int ndrawn = 0, xblock = cww/side, yblock = cwh/side;
    int xmin = (tid%side)*xblock - cww/2, ymin = (tid/side)*yblock - cwh/2;
    int xmax = xmin+xblock; clamp(xmax,0,cww-1);
    int ymax = ymin+yblock; clamp(ymax,0,cwh-1);
    int depth = depthtest(xmin, ymin, xmin+xblock, ymin+yblock);
    for (bool d = false; !d && can.isOpen(); d = blur(bg, xmin, ymin, xmax, ymax, ndrawn, depth--));
  }
}

int main(int argc, char* argv[]) {
  int w, h, t = (argc > 1) ? atoi(argv[1]) : omp_get_num_procs();
  std::string fname = (argc > 2) ? argv[2] : "./assets/pics/HDR_landscape.jpg";
  Image::getFileResolution(fname,w,h);
  w*=0.6;
  h*=0.6;
  printf("%d, %d\n", w, h);
  Canvas c(-1, -1, w, h, "Blurring using recursive splitting");
  c.run(blurImageFunction,fname,t);
}
