#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(unsigned threads, unsigned depth) {
    myThreads = threads;
    myDepth = depth;
    myFirstX = myFirstY = mySecondX = mySecondY = 0.0;
    myRedraw = true;
}

void Mandelbrot::bindings(Cart& can) {
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, this]() {
      can.clear();
      this->myRedraw = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, this]() {
      can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), this->myFirstX, this->myFirstY);
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, this]() {
      can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), this->mySecondX, this->mySecondY);
      if (!(this->myFirstX == this->mySecondX || this->myFirstY == this->mySecondY)) {
        can.zoom(this->myFirstX, this->myFirstY, this->mySecondX, this->mySecondY);
        this->myRedraw = true;
      }
    });
    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, this]() {
      Decimal x, y;
      can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
      can.zoom(x, y, 1.5);
      this->myRedraw = true;
    });
    can.bindToScroll([&can, this](double dx, double dy) {
      Decimal x, y;
      can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
      Decimal scale;
      if (dy == 1) scale = .5;
      else scale = 1.5;
      can.zoom(x, y, scale);
      this->myRedraw = true;
    });
  }

void Mandelbrot::draw(Cart& can) {
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  const int XBRD = 10;                    //Border for out progress bar
  const int YBRD = 40;                    //Border for out progress bar
  Canvas pCan(0, 0, 800, 100, "");        //Canvas for our progress bar
  pCan.start();
  ProgressBar pb(
    XBRD,YBRD,pCan.getWindowWidth()-XBRD*2,pCan.getWindowHeight()-YBRD*2,
    0,CH - (CH % myThreads),myThreads   //Make the max PB value a multiple of myThreads
  );
  while(myRedraw) {
    setRedraw(false);
    can.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      unsigned tid = omp_get_thread_num();
      unsigned nthreads = omp_get_num_threads();
      ColorFloat tcolor = Colors::highContrastColor(tid);
      double blocksize = can.getCartHeight() / nthreads;
      double blockheight = CH / nthreads;
      pCan.clear();
      pb.update(blockheight*tid);
      pCan.drawProgress(&pb);
      long double startrow = blocksize * tid + can.getMinY();
      for(unsigned int k = 0; k <= blockheight && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
        pb.update(k+(CH*tid)/nthreads);
        pCan.drawRectangle(XBRD,YBRD,pCan.getWindowWidth()-XBRD,pCan.getWindowHeight()-YBRD,pCan.getBackgroundColor(),true);
        pCan.drawText(to_string(tid), 100 + tid*200, 32, 32, BLACK);
        pCan.drawProgress(&pb);
        long double row = startrow + can.getPixelHeight() * k;
        for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
          complex originalComplex(col, row);
          complex c(col, row);
          unsigned iterations = 0;
          while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            c = c * c + originalComplex;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
            can.drawPoint(col, row, Colors::blendedColor(tcolor,WHITE,0.25f+0.5f*mult)*mult);
          }
          if (myRedraw) break;
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
    std::cout << can.getTime() << std::endl;
    while (can.getIsOpen() && !myRedraw) {
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
  }
  if (pCan.getIsOpen())
    pCan.close();
  pCan.wait();  //Close our progress bar if we're done
}

  //mutator
void Mandelbrot::setRedraw(bool newValue) {
  myRedraw = newValue;
}
