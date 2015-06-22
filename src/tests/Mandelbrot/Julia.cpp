#include "Julia.h"

Julia::Julia(unsigned threads, unsigned depth) {
    myThreads = threads;
    myDepth = depth;
    myFirstX = myFirstY = mySecondX = mySecondY = 0.0;
    myRedraw = true;
}

Julia::~Julia() {
//Do nothing
}

void Julia::bindings(CartesianCanvas& can) {
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

void Julia::draw(CartesianCanvas& can) {
  const int CH = can.getWindowHeight();   //Height of our Mandelbrot canvas
  VisualQueue vq(CH);
  while(myRedraw) {
    setRedraw(false);
    can.reset();
    unsigned next = 0;
    vq.reset();
    #pragma omp parallel num_threads(myThreads)
    {
      int myNext;
      unsigned tid = omp_get_thread_num();
      unsigned nthreads = omp_get_num_threads();
      ColorFloat tcolor = Colors::highContrastColor(tid);
      while(true) {  // As long as we aren't trying to render off of the screen...
        #pragma omp critical
        {
          myNext = next++;
        }
        if (myNext >= can.getWindowHeight())
          break;
        vq.update(myNext,RUNNING);
        long double row = can.getMinY() + can.getPixelHeight() * myNext;
        for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
          complex z(-0.8f, 0.156f);
          complex c(col, row);
          unsigned iterations = 0;
          while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
            iterations++;
            c = c * c + z;
          }
          if(iterations == myDepth) { // If the point never escaped, draw it black
            can.drawPoint(col, row, BLACK);
          } else { // Otherwise, draw it with color based on how long it took
            float mult = iterations/(float)myDepth;
//            can.drawPoint(col, row, Colors::blendedColor(tcolor,WHITE,0.25f+0.5f*mult)*mult);
            can.drawPoint(col, row, Colors::blendedColor(BLACK,WHITE,0.25f+0.5f*mult)*mult);
          }
          if (!can.getIsOpen() || myRedraw) break;
        }
        vq.update(myNext,FINISHED);
        can.handleIO();
        if (myRedraw) break;
      }
    }
    std::cout << can.getTime() << std::endl;
    while (can.getIsOpen() && !myRedraw) {
      can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    }
  }
  vq.close();
}

  //mutator
void Julia::setRedraw(bool newValue) {
  myRedraw = newValue;
}
