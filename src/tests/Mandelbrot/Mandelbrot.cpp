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
  int** canPoints = new int*[can.getWindowHeight()];
  for (int i = 0; i < can.getWindowHeight(); ++i) {
    canPoints[i] = new int[can.getWindowWidth()];
    for (int j = 0; j < can.getWindowWidth(); ++j) {
      ColorInt c = can.getPoint(j,i);
      canPoints[i][j] = ((c.R == c.G) && (c.G == c.B) && (c.B == 0)) ? 0 : -1;
    }
  }
  bool done = false;
  int loop, cwh = can.getWindowHeight(), cww = can.getWindowWidth();
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
      ColorFloat c = can.getPoint(j,i);
      can.Canvas::drawPoint(j,i,c*mult);
    }
  }

  for (int i = 0; i < can.getWindowHeight(); ++i)
    delete [] canPoints[i];
  delete [] canPoints;
  canPoints = NULL;
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
  const int PBWIDTH = 800;
  Canvas pCan(0, 0, PBWIDTH, 100, "Thread Workloads");        //Canvas for our progress bar
  pCan.start();
  ProgressBar pb(
    XBRD,YBRD,pCan.getWindowWidth()-XBRD*2,pCan.getWindowHeight()-YBRD*2,
    0,CH - (CH % myThreads),myThreads   //Make the max PB value a multiple of myThreads
  );
  for (int i = 0; i < myThreads; ++i) {
    pCan.drawText(to_string(i),pb.getSegX(i)+8,pb.getSegY()-8,32,BLACK);
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
      pCan.clear();
      pb.update(blockheight*tid);
      pCan.drawProgress(&pb);
      long double startrow = blocksize * tid + can.getMinY();
      for(unsigned int k = 0; k <= blockheight && can.isOpen(); k++) {  // As long as we aren't trying to render off of the screen...
        pb.update(k+(CH*tid)/nthreads);
        //Messy, but effective
//        pCan.drawRectangle(XBRD,YBRD,pCan.getWindowWidth()-XBRD,pCan.getWindowHeight()-YBRD,pCan.getBackgroundColor(),true);
        //Elegant, but flickery
        pCan.clear();
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
            can.drawPoint(col, row, Colors::blend(tcolor,WHITE,0.25f+0.5f*mult)*mult);
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
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
  }
  if (pCan.isOpen())
    pCan.close();
  pCan.wait();  //Close our progress bar if we're done
}
