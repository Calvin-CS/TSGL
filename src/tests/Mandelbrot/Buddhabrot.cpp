/*
 * Buddhabrot.cpp
 */

#include "Buddhabrot.h"

#ifdef _WIN32 //Windows doesn't like numbers higher than RAND_MAX (32767)
  #define rand() (rand() | rand() << 16)
#endif

Buddhabrot::Buddhabrot(unsigned threads, unsigned depth = 1000) : Mandelbrot(threads, depth) {
  myThreads = threads;
  myDepth = depth;
  myRedraw = true;
  counter = nullptr;
  cww = cwh = 0;
}

Buddhabrot::~Buddhabrot() {
  for (int i = 0; i < cwh; ++i)
    delete [] counter[i];
  delete[] counter;
}

void Buddhabrot::draw(Cart& can) {
  cww = can.getWindowWidth(), cwh = can.getWindowHeight();
  const unsigned long MAXITS = cww*cwh*10;
  ColorFloat tcolor(1.0f,1.0f,1.0f,0.1f);
  const int RPREC = 100000;
  counter = new int*[cwh];
  for (int i = 0; i < cwh; ++i)
    counter[i] = new int[cww];
  while (myRedraw) {
    myRedraw = false;
    can.clear();
    for (int i = 0; i < cwh; ++i)
      for (int j = 0; j < cww; ++j)
        counter[i][j] = 0;
    const Decimal cph = can.getPixelHeight(), cpw = can.getPixelWidth(),
      cMinx = can.getMinX(), cMiny = can.getMinY(),
      cMaxx = cMinx+cpw*(cww-1), cMaxy = cMiny+cph*(cwh-1);
    unsigned long cycles = 0;
    #pragma omp parallel num_threads(myThreads)
    {
      unsigned tid = omp_get_thread_num(), threads = omp_get_num_threads();
      Decimal offset = cMiny+(cph*cwh*tid)/(Decimal)threads;
      const Decimal wscale = (cpw*cww)/(Decimal)RPREC;
      const Decimal hscale = (cph*cwh/threads)/(Decimal)RPREC;
      ColorFloat tc = Colors::highContrastColor(tid);
      ColorFloat tcolor(tc.R,tc.G,tc.B,0.1f);
      complex* znums = new complex[myDepth];
      Decimal col, row;
      for (unsigned long i = tid; i < MAXITS; i+= threads) {
        if (myRedraw) break;
        col = cMinx+wscale*(rand() % RPREC);    //Between cMinx and cMaxx
        row = offset+hscale*(rand() % RPREC);    //Between cMiny and cMaxy
        complex c(col,row);
        complex z = c;
        unsigned its = 0;
        while (std::abs(z) < 2.0l && its != myDepth) {
          z = z * z + c;
          znums[its] = z;
          ++its;
        }
        if (its < myDepth) {  //If we're not in the Mandelbrot set
          z = c;
          while (its > 0) {
            --its;
            if (znums[its].imag() < cMiny || znums[its].imag() > cMaxy ||
             znums[its].real() < cMinx || znums[its].real() > cMaxx)
              continue;
            int boxY = (znums[its].imag()-cMiny)/cph;
            int boxX = (znums[its].real()-cMinx)/cpw;
            #pragma omp atomic
              ++(counter[boxY][boxX]);
            can.Canvas::drawPixel(boxY, boxX, tcolor);
          }
        }
        #pragma omp atomic
          ++cycles;
        if (cycles % (MAXITS/100) == 0) {
          std::cout << (100*cycles)/MAXITS << "%" << std::endl;
          can.handleIO();
        }
        if (myRedraw || !can.getIsOpen())
          break;
      }
    delete [] znums;
    }
    if (!can.getIsOpen())
      return;
    int maxIts = 0;
    for (int i = 0; i < cwh; ++i)
      for (int j = 0; j < cww; ++j)
        if (counter[i][j] > maxIts)
          maxIts = counter[i][j];
    std::cout << maxIts << " max iterations" << std::endl;
    #pragma omp parallel num_threads(myThreads)
    {
      for (int i = omp_get_thread_num(); i < cwh; i += omp_get_num_threads())
        for (int j = 0; j < cww; ++j) {
          float normalize = sqrt((float)counter[i][j]/maxIts);
          can.Canvas::drawPixel(i, j, (ColorFloat)can.getPixel(i,j) * normalize);
        }
    }
    while (can.getIsOpen() && !myRedraw)
      can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
  }
}

