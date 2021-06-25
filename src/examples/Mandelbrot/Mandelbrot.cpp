/*
 * Mandelbrot.cpp
 */

#include "Mandelbrot.h"

Mandelbrot::Mandelbrot(unsigned threads, unsigned depth) {
    myThreads = threads;
    myDepth = depth;
    myFirstX = myFirstY = mySecondX = mySecondY = 0.0;
    myRedraw = true;
}

void Mandelbrot::manhattanShading(CartesianCanvas& can) {
  int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
  CartesianBackground * bg = can.getBackground();
  int** canPoints = new int*[cwh];
  for (int i = 0; i < cwh; ++i) {
    canPoints[i] = new int[cww];
    for (int j = 0; j < cww; ++j) {
      ColorInt c = bg->Background::getPixel(j - cww/2,i - cwh/2);
      canPoints[i][j] = ((c.R == c.G) && (c.G == c.B) && (c.B == 0)) ? 0 : -1;
    }
  }
  bool done = false;
  int loop;
  for (loop = 0; !done; ++loop) {
    done = true;
    #pragma omp parallel for
    for (int i = 0; i < cwh; ++i) {
      for (int j = 0; j < cww; ++j) {
        if (canPoints[i][j] != loop)
          continue;
        if (i > 0 && canPoints[i-1][j] == -1)
          canPoints[i-1][j] = loop+1, done = false;
        if (j > 0 && canPoints[i][j-1] == -1)
          canPoints[i][j-1] = loop+1, done = false;
        if (i < cwh-1 && canPoints[i+1][j] == -1)
          canPoints[i+1][j] = loop+1, done = false;
        if (j < cww-1 && canPoints[i][j+1] == -1)
          canPoints[i][j+1] = loop+1, done = false;
      }
    }
  }

  int sum = 0;
  for (int i = 0; i < cwh; ++i)
    for (int j = 0; j < cww; ++j)
      sum += canPoints[i][j];
  float avg = (((float)sum)/cww)/cwh;

  #pragma omp parallel for
  for (int i = 0; i < cwh; ++i) {
    for (int j = 0; j < cww; ++j) {
      float mult = sqrt(avg*((float)canPoints[i][j])/loop);
      ColorFloat c = bg->Background::getPixel(j - cww/2,i - cwh/2);
      bg->Background::drawPixel(j - cww/2,i - cwh/2,c*mult);
    }
  }

  for (int i = 0; i < can.getWindowHeight(); ++i)
    delete [] canPoints[i];
  delete [] canPoints;
  canPoints = NULL;
}

void Mandelbrot::bindings(Cart& can) {
    can.bindToButton(TSGL_ENTER, TSGL_PRESS, [&can, this]() {
      can.zoom(can.getMouseX(), can.getMouseY(), 0.5);
      can.getBackground()->clear();
      this->myRedraw = true;
    });
    can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, this]() {
      can.getBackground()->clear();
      this->myRedraw = true;
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, this]() {
      this->myFirstX = can.getMouseX();
      this->myFirstY = can.getMouseY();
    });
    can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, this]() {
      this->mySecondX = can.getMouseX();
      this->mySecondY = can.getMouseY();
      if (!(this->myFirstX == this->mySecondX || this->myFirstY == this->mySecondY)) {
        can.zoom(this->myFirstX, this->myFirstY, this->mySecondX, this->mySecondY);
        can.getBackground()->clear();
        this->myRedraw = true;
      }
    });
    can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, this]() {
      Decimal x, y;
      x = can.getMouseX();
      y = can.getMouseY();
      can.zoom(x, y, 1.5);
      can.getBackground()->clear();
      this->myRedraw = true;
    });
    can.bindToScroll([&can, this](double dx, double dy) {
      Decimal x, y;
      x = can.getMouseX();
      y = can.getMouseY();
      Decimal scale;
      if (dy == 1) scale = .5;
      else scale = 1.5;
      can.zoom(x, y, scale);
      can.getBackground()->clear();
      this->myRedraw = true;
    });
  }

void Mandelbrot::draw(Cart& can) {
  CartesianBackground * bg = can.getBackground();
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  const int XBRD = 10;                    //Border for out progress bar
  const int YBRD = 40;                    //Border for out progress bar
  const int PBWIDTH = 800;
  Canvas pCan(0, 0, PBWIDTH, 100, "Thread Workloads");        //Canvas for our progress bar
  Background * pBack = pCan.getBackground();
  pCan.start();
  ProgressBar * pb = new ProgressBar(
    0,0,0,
    pCan.getWindowWidth()-XBRD*2,pCan.getWindowHeight()-YBRD*2,
    0,CH - (CH % myThreads),myThreads, 0,0,0   //Make the max PB value a multiple of myThreads
  );
  pCan.add(pb);

  for (int i = 0; i < myThreads; ++i) {
    float w = pb->getWidth();
    pBack->drawText(-w/2 + i*w/myThreads + 10, pb->getHeight()+8, 0, std::to_wstring(i), "/usr/local/include/TSGL/assets/freefont/FreeSerif.ttf", 32, 0,0,0, BLACK);
  }
  while(myRedraw) {
    myRedraw = false;
    can.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      unsigned tid = omp_get_thread_num();
      unsigned nthreads = omp_get_num_threads();
      ColorFloat tcolor = Colors::highContrastColor(tid);
      double blocksize = can.getCartHeight() / nthreads;
      double blockheight = CH / nthreads;
      pb->update(blockheight*tid);
      long double startrow = blocksize * tid + can.getMinY();
      for(unsigned int k = 0; k <= blockheight && can.isOpen(); k++) {  // As long as we aren't trying to render off of the screen...
        pb->update(k+(CH*tid)/nthreads);
        long double y = startrow + can.getPixelHeight() * k;
        for(long double x = bg->getMinX(); x <= bg->getMaxX(); x += can.getPixelWidth()) {
          complex originalComplex(x, y);
          complex c(x, y);
          unsigned iterations = 0;
          while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            c = c * c + originalComplex;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            if (y < can.getMaxY())
              bg->drawPixel(x, y, ColorFloat(0,0,0,1));
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
            if (y < can.getMaxY())
              bg->drawPixel(x, y, Colors::blend(tcolor,WHITE,0.25f+0.5f*mult)*mult);
          }
          if (myRedraw) break;
        }
        can.handleIO();
        if (myRedraw) break;
      }
    }
//    shadeCanvas(can);  Optional shading
    std::cout << can.getTime() << std::endl;
    while (can.isOpen() && !myRedraw) {
      pCan.sleep();
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
  }
  if (pCan.isOpen())
    pCan.close();
  pCan.wait();  //Close our progress bar if we're done
  delete pb;
}
