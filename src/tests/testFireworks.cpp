/*
 * testFireworks.cpp
 *
 * Usage: ./testFireworks <width> <height> <numThreads> <numFireworks> <speed>
 */

#include <tsgl.h>

using namespace tsgl;

const float HFLOAT = 6.0f/255.0f;

class Dot {
private:
  bool dead;
  Canvas* myCan;
  float myX, myY, mySpeed, myDir, myFric;
public:
  Dot(Canvas& can, float x, float y, float s, float d, float f) {
    myCan = &can;
    dead = false;
    myX = x; myY = y; mySpeed = s;
    myDir = d; myFric = f;
  }
  inline void step() {
    myX += mySpeed*cos(myDir);
    myY += mySpeed*sin(myDir);
    mySpeed *= myFric;
    if (!dead)
      myCan->drawPoint(myX,myY,WHITE);
    if (mySpeed < 0.5f)
      dead = true;
  }
  inline bool isDead() { return dead; }
};

class Firework {
private:
  bool dead;
  int myX, myY;
  Canvas* myCan;
  Dot* myDots[10];
public:
  Firework(Canvas& can, int x, int y) {
    dead = false;
    myCan = &can;
    myX = x;
    myY = y;
    for (int i = 0; i < 10; ++i)
      myDots[i] = new Dot(can, myX,myY,(rand() % 10000)/10000.0f,(rand() % 10000)/10000.0f * 2 * PI, 0.99f);
  }
  ~Firework() {
    for (int i = 0; i < 10; delete myDots[i++]);
  }
  inline void step() {
    if (!dead) {
      bool allDead = true;
      for (int i = 0; i < 10; ++i) {
        myDots[i]->step();
        if (!myDots[i]->isDead())
          allDead = false;
      }
      dead = allDead;
    }
  }
  inline bool isDead() { return dead; }
};

class Arc {
private:
  Canvas* myCan;
  int myLife;
  float myX, myY, myRad;
  float myAngle, myStepSize;
  ColorHSV myColor;
  Firework* f;
public:
  Arc(Canvas& can) {
    f = NULL;
    myLife = 0;
    myCan = &can;
    myX = rand() % myCan->getWindowWidth();
    myY = rand() % myCan->getWindowHeight();
    myAngle = ((rand() % 32000) / 32000.0f) * 2.0f*PI;
    myRad = 20 + rand() % 180;
    computeStepSize();
    myColor = ColorHSV(0.0f,1.0f,1.0f,1.0f);
  }
  Arc(Canvas* can, int x, int y, int rad, float angle) {
    myCan = can;
    myX = x; myY = y;
    myAngle = angle; myRad = rad;
    computeStepSize();
  }
  inline bool outOfBounds() {
    return (myX < 0 || myY < 0 || myX > myCan->getWindowWidth() || myY > myCan->getWindowHeight());
  }
  inline bool onBlackPixel() {
    const int LET = 14;
    ColorInt col = myCan->getPoint(myX,myY);
    return !(col.R<LET && col.G<LET && col.B<LET);
  }
  inline void computeStepSize() {
    myStepSize = 1.0f/myRad;
    if (rand() % 2 == 0) {
      myStepSize = -myStepSize;
      myAngle = -myAngle;
    }
    myColor.H = HFLOAT*(++myLife % 255);
  }
  inline void relocate() {
    myRad = 20 + rand() % 180;
    myAngle = ((rand() % 32000) / 32000.0f) * 2.0f*PI;
    while (onBlackPixel()) {
      myX = rand() % myCan->getWindowWidth();
      myY = rand() % myCan->getWindowHeight();
    }
    computeStepSize();
  }
  inline void step() {
    if (f != NULL)
      f->step();
    if (rand() % 100 < 2) {
      ++myRad;
      myStepSize = 1.0f/(myRad);
    }
    myAngle += myStepSize;
    myX += cos(myAngle);
    myY += sin(myAngle);
    if (outOfBounds() || onBlackPixel()) {
      if (f != NULL)
        delete f;
      f = new Firework(*myCan,myX,myY);
      relocate();
    }
    myCan->drawPoint(myX,myY,myColor);
  }
};

void fireworkFunction(Canvas& can, int threads, int numFireworks, int speed) {
  Arc* arcs[numFireworks];
  for (int i = 0; i < numFireworks; arcs[i++] = new Arc(can))
    arcs[i] = new Arc(can);
  ColorFloat col = can.getBackgroundColor();
  col.A = 0.04f;
  #pragma omp parallel num_threads(threads)
  {
    const int CWW = can.getWindowWidth(), CWH = can.getWindowHeight();
    int tid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    while(can.getIsOpen()) {
      for (int n = 0; n < speed; ++n) {
        for (int i = tid; i < numFireworks; i += nthreads)
          arcs[i]->step();
        if (tid == 0)
          can.drawRectangle(0,0,CWW,CWH,col);
        #pragma omp barrier
      }
      can.sleep();
    }
  }
  for (int i = 0; i < numFireworks; delete arcs[i++]);
}

int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : 0.9*Canvas::getDisplayHeight();
  int h = (argc > 2) ? atoi(argv[2]) : w;
  if (w <= 0 || h <= 0)     //Checked the passed width and height if they are valid
    w = h = 960;            //If not, set the width and height to a default value
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  int f = (argc > 4) ? atoi(argv[4]) : 50;
  int s = (argc > 5) ? atoi(argv[5]) : 10;
  Canvas c(-1, -1, w, h, "Fireworks!");
  c.setBackgroundColor(BLACK);
  c.start();
  fireworkFunction(c,t,f,s);
  c.wait();
}
