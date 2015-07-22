/*
 * testPhilosophers.cpp
 *
 * Usage: ./testPhilosophers <numPhilosophers> <speed>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

enum PhilState {
  hasNone, nasRight, hasLeft, hasBoth, isFull
};

enum PhilAction {
  doNothing, tryLeft, tryRight, tryBoth, releaseLeft, releaseRight, releaseBoth
};

enum PhilMethod {
  forfeitWhenBlocked, waitWhenBlocked, nFrameRelease, resourceHierarchy, oddEven
};

struct Fork {
  int user = -1, id = 0;
  void draw(Canvas& can, int x, int y, ColorFloat c) {
    can.drawCircle(x,y,16,32,c,true);
  }
};

class Philosopher {
private:
  PhilState state;
  PhilAction action;
  int id, myLeft, myRight, meals;
public:
  Philosopher() {
    setId(0,1); meals = 0; state = hasNone; action = doNothing;
  }
  void draw(Canvas& can, int x, int y) {
    const int SIZE = 32;
    switch(state) {
      case hasNone:
        can.drawCircle(x,y,SIZE,SIZE,RED,true); break;
      case nasRight:
        can.drawCircle(x,y,SIZE,SIZE,ORANGE,true); break;
      case hasLeft:
        can.drawCircle(x,y,SIZE,SIZE,YELLOW,true); break;
      case hasBoth:
        can.drawCircle(x,y,SIZE,SIZE,GREEN,true); break;
      case isFull:
        can.drawCircle(x,y,SIZE,SIZE,BLUE,true); break;
    }
  }
  bool acquire(Fork& f) {
    if (f.user >= 0)
      return false;
    if (f.id == myLeft) {
      if (state == hasNone)
        state = hasLeft;
      else if (state == nasRight) {
        state = hasBoth;
      }
      else
        return false;
      f.user = id;
      return true;
    }
    if (f.id == myRight) {
      if (state == hasNone)
        state = nasRight;
      else if (state == hasLeft) {
        state = hasBoth;
      }
      else
        return false;
      f.user = id;
      return true;
    }
    return false;
  }
  bool release(Fork& f) {
    if (f.user != id)
      return false;
    if (state != isFull) {
      if (f.id == myLeft)
        state = (state == hasLeft) ? hasNone: isFull;
      else
        state = (state == nasRight) ? hasNone : isFull;
    }
    f.user = -1;
    return true;
  }
  void eat() {
    ++meals;
    state = hasNone;
    action = doNothing;
  }
  PhilState getState() { return state; }
  void setState(PhilState p) { state = p; }
  PhilAction getAction() { return action; }
  void setAction(PhilAction a) { action = a; }
  int getId() { return id; }
  void setId(int i, int nphil) {id = myLeft = i; myRight = (id+nphil-1)%nphil; }
  int getMeals() { return meals; }
};

class Table {
private:
  int tabX, tabY, numPhilosophers;
  Canvas *myCan;
  Philosopher *phils;
  Fork *forks;
public:
  Table(Canvas& can, int p) {
    numPhilosophers = p;
    myCan = &can;
    tabX = can.getWindowWidth()/2;
    tabY = can.getWindowHeight()/2;
    phils = new Philosopher[numPhilosophers];
    forks = new Fork[numPhilosophers];
    for (int i = 0; i < numPhilosophers; ++i) {
      phils[i].setId(i,numPhilosophers);
      forks[i].id = i;
    }
  }

  ~Table() {
    delete [] phils;
    delete [] forks;
  }

  void forfeitWhenBlockedMethod(int id) {
    int left = id, right = (id+numPhilosophers-1)%numPhilosophers;
    switch(phils[id].getState()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case nasRight:
        if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(releaseRight);
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else
          phils[id].setAction(releaseLeft);
        break;
      case hasBoth:
        phils[id].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void waitWhenBlockedBlockedMethod(int id) {
    int left = id, right = (id+numPhilosophers-1)%numPhilosophers;
    switch(phils[id].getState()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case nasRight:
        if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else
          phils[id].setAction(doNothing);
        break;
      case hasBoth:
        phils[id].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void nFrameReleaseMethod(int id) {
    int left = id, right = (id+numPhilosophers-1)%numPhilosophers;
    switch(phils[id].getState()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case nasRight:
        if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else {
          if (id == (myCan->getFrameNumber() % numPhilosophers+1))
            phils[id].setAction(releaseRight);
          else
            phils[id].setAction(doNothing);
        }
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else {
          if (id == (myCan->getFrameNumber() % numPhilosophers+1))
            phils[id].setAction(releaseLeft);
          else
            phils[id].setAction(doNothing);
        }
        break;
      case hasBoth:
        phils[id].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void hieararchyMethod(int id) {
    int left = id, right = (id+numPhilosophers-1)%numPhilosophers;
    switch(phils[id].getState()) {
      case hasNone:
        if (right < left) {
          if (forks[right].user == -1)
            phils[id].setAction(tryRight);
          else
            phils[id].setAction(doNothing);
        } else {
          if (forks[left].user == -1)
            phils[id].setAction(tryLeft);
          else
            phils[id].setAction(doNothing);
        }
        break;
      case nasRight:
        if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else {
          phils[id].setAction(doNothing);
        }
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else
          phils[id].setAction(doNothing);
        break;
      case hasBoth:
        phils[id].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void oddEvenMethod(int id) {
    int left = id, right = (id+numPhilosophers-1)%numPhilosophers;
    switch(phils[id].getState()) {
      case hasNone:
        if ((id % 2) == (myCan->getFrameNumber() % 2))
          phils[id].setAction(tryBoth);
        else
          phils[id].setAction(doNothing);
        break;
      case nasRight:
        if ((id % 2) == (myCan->getFrameNumber() % 2))
          phils[id].setAction(tryLeft);
        else {
          phils[id].setAction(releaseRight);
        }
        break;
      case hasLeft:
        if ((id % 2) == (myCan->getFrameNumber() % 2))
          phils[id].setAction(tryRight);
        else
          phils[id].setAction(releaseLeft);
        break;
      case hasBoth:
        phils[id].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void checkStep(PhilMethod p) {
    // std::cout << " State: ";
    int i = omp_get_thread_num();
    // for (int i = 0; i < numPhilosophers; ++i) {
    // std::cout << phils[i].getState();
    if (phils[i].getState() == isFull) {
      phils[i].eat();
      // continue;
      return;
    }
    switch(p) {
      case forfeitWhenBlocked:
        forfeitWhenBlockedMethod(i);
        break;
      case waitWhenBlocked:
        waitWhenBlockedBlockedMethod(i);
        break;
      case nFrameRelease:
        nFrameReleaseMethod(i);
        break;
      case resourceHierarchy:
        hieararchyMethod(i);
        break;
      case oddEven:
        oddEvenMethod(i);
        break;
      default:
        break;
    }
    // }
    // std::cout << std::endl;
  }

  void actStep() {
    // std::cout << " Action:";
    // for (int i = 0; i < numPhilosophers; ++i) {
    int i = omp_get_thread_num();
    int left = i, right = (i+numPhilosophers-1)%numPhilosophers;
    // std::cout << phils[i].getAction();
    switch(phils[i].getAction()) {
      case tryLeft:
        phils[i].acquire(forks[left]);
        break;
      case tryRight:
        phils[i].acquire(forks[right]);
        break;
      case tryBoth:
        phils[i].acquire(forks[left]);
        phils[i].acquire(forks[right]);
        break;
      case releaseLeft:
        phils[i].release(forks[left]);
        break;
      case releaseRight:
        phils[i].release(forks[right]);
        break;
      case releaseBoth:
        phils[i].release(forks[left]);
        phils[i].release(forks[right]);
        break;
      default:
        break;
    }
    // }
    // std::cout << std::endl;
  }

  void drawStep() {
    const int RAD = 300;
    const float ARC =2*PI/numPhilosophers;
    const float CLOSE = 0.15f;
    const float BASEDIST = RAD+64;
    myCan->drawCircle(tabX,tabY,RAD-48,RAD,DARKGRAY);
    for (int i = 0; i < numPhilosophers; ++i) {
      float pangle = (i*2*PI)/numPhilosophers;
      phils[i].draw(*myCan,tabX+RAD*cos(pangle),tabY+RAD*sin(pangle));
      for (int j = 0; j < phils[i].getMeals(); ++j) {
        float angle = pangle+(j/10)*2*PI/RAD, dist = BASEDIST+8*(j%10);
        myCan->drawCircle(tabX+dist*cos(angle), tabY+dist*sin(angle), 3,8,BROWN);
      }
      ColorFloat fcolor = BLACK;
      float fangle = (i+0.5f)*ARC;;
      if (forks[i].user == i) {
        fangle = i*ARC + CLOSE;
        if (phils[i].getState() == hasBoth)
          fcolor = GREEN;
        else
          fcolor = YELLOW;
      }
      else if((forks[i].user == (i+1)%numPhilosophers)) {
        fangle = ((i+1)*ARC) - CLOSE;
        if (phils[(i+1)%numPhilosophers].getState() == hasBoth)
          fcolor = GREEN;
        else
          fcolor = ORANGE;
      }
      forks[i].draw(*myCan,tabX+RAD*cos(fangle),tabY+RAD*sin(fangle),fcolor);
    }
  }
};

void philosopherFunction(Canvas& can,int philosophers) {
  Table t(can,philosophers);
  can.sleep();
  #pragma omp parallel num_threads(philosophers)
  {
    while(can.getIsOpen()) {
      // std::cout << can.getFrameNumber() << std::endl;
      // t.checkStep(waitWhenBlocked);     //Deadlock
      // t.checkStep(forfeitWhenBlocked);  //Livelock
      t.checkStep(nFrameRelease);       //No locking; mostly fair for N philosophers, N >= 5
      // t.checkStep(resourceHierarchy);   //No locking; mostly fair for N philosophers, N >= 2
      // t.checkStep(oddEven);             //No locking; perfectly fair for N philosophers, N >= 2
      // #pragma omp barrier
      t.actStep();
      can.pauseDrawing();
      can.clear();
      t.drawStep();
      can.resumeDrawing();
      can.sleep();
    }
  }
}

int main(int argc, char* argv[]) {
    int nphil = (argc > 1) ? atoi(argv[1]) : 5;
    int speed = (argc > 2) ? atoi(argv[2]) : 10;
    Canvas c(-1, -1, -1, -1, "Philosophers",1.0f/speed);
    c.run(philosopherFunction,nphil);
}
