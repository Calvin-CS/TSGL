#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

const int PHIL = 5;  //5 Philosophers

enum PhilState {
  hasNone, nasRight, hasLeft, hasBoth, isFull
};

enum PhilAction {
  doNothing, tryLeft, tryRight, tryBoth, releaseLeft, releaseRight, releaseBoth
};

enum PhilMethod {
  forfeitWhenBlocked, waitWhenBlocked, nFrameRelease
};

struct Fork {
  int user = -1, id = 0;
  void draw(Canvas& can, int x, int y) {
    can.drawCircle(x,y,16,32,(user == -1) ? BLACK : RED,true);
  }
};

class Philosopher {
private:
  PhilState state;
  PhilAction action;
  int id, myLeft, myRight, meals;
public:
  Philosopher() {
    setId(0); meals = 0; state = hasNone; action = doNothing;
  }
  void draw(Canvas& can, int x, int y) {
    switch(state) {
      case hasNone:
        can.drawCircle(x,y,32,32,WHITE,true); break;
      case nasRight:
      case hasLeft:
        can.drawCircle(x,y,32,32,BLUE,true); break;
      case hasBoth:
      case isFull:
        can.drawCircle(x,y,32,32,GREEN,true); break;
    }
  }
  bool acquire(Fork& f) {
    if (f.user >= 0)
      return false;
    //Same id is the one on the left; lower id is the one on the right
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
  void eat() { ++meals; }
  PhilState getState() { return state; }
  void setState(PhilState p) { state = p; }
  PhilAction getAction() { return action; }
  void setAction(PhilAction a) { action = a; }
  int getId() { return id; }
  void setId(int i) {id = myLeft = i; myRight = (id+PHIL-1)%PHIL; }
  int getMeals() { return meals; }
};

class Table {
private:
  int centerX, centerY;
  Canvas* myCan;
  Philosopher phils[PHIL];
  Fork forks[PHIL];
public:
  Table(Canvas& can) {
    myCan = &can;
    centerX = can.getWindowWidth()/2;
    centerY = can.getWindowHeight()/2;
    for (int i = 0; i < PHIL; ++i) {
      phils[i].setId(i);
      forks[i].id = i;
    }
  }

  void forfeitWhenBlockedMethod(int i) {
    int left = i, right = (i+PHIL-1)%PHIL;
    switch(phils[i].getState()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[i].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[i].setAction(tryLeft);
        else
          phils[i].setAction(doNothing);
        break;
      case nasRight:
        if (forks[left].user == -1)
          phils[i].setAction(tryLeft);
        else
          phils[i].setAction(releaseRight);
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[i].setAction(tryRight);
        else
          phils[i].setAction(releaseLeft);
        break;
      case hasBoth:
        phils[i].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void waitWhenBlockedBlockedMethod(int i) {
    int left = i, right = (i+PHIL-1)%PHIL;
    switch(phils[i].getState()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[i].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[i].setAction(tryLeft);
        else
          phils[i].setAction(doNothing);
        break;
      case nasRight:
        if (forks[left].user == -1)
          phils[i].setAction(tryLeft);
        else
          phils[i].setAction(doNothing);
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[i].setAction(tryRight);
        else
          phils[i].setAction(doNothing);
        break;
      case hasBoth:
        phils[i].setAction(releaseBoth);
        break;
      default:
        break;
    }
  }

  void nFrameReleaseMethod(int id) {
    int left = id, right = (id+PHIL-1)%PHIL;
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
          if (id == (myCan->getFrameNumber() % PHIL+1))
            phils[id].setAction(releaseRight);
          else
            phils[id].setAction(doNothing);
        }
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else {
          if (id == (myCan->getFrameNumber() % PHIL+1))
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

  void checkStep(PhilMethod p) {
    std::cout << " State: ";
    for (int i = 0; i < PHIL; ++i) {
      std::cout << phils[i].getState();
      if (phils[i].getState() == isFull) {
        phils[i].eat();
        phils[i].setState(hasNone);
        phils[i].setAction(doNothing);
        continue;
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
        default:
          break;
      }
    }
    std::cout << std::endl;
  }

  void actStep() {
    std::cout << " Action:";
    for (int i = 0; i < PHIL; ++i) {
      int left = i, right = (i+PHIL-1)%PHIL;
      std::cout << phils[i].getAction();
      switch(phils[i].getAction()) {
        case tryLeft:
          phils[i].acquire(forks[left]);
          break;
        case tryRight:
          phils[i].acquire(forks[right]);
          break;
        case tryBoth: //Not allowed, strictly speaking
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
    }
    std::cout << std::endl;
  }

  void drawStep() {
    const int RAD = 300;
    const float ARC =2*PI/PHIL;
    myCan->drawCircle(centerX,centerY,RAD-48,256,DARKGRAY);
    for (int i = 0; i < PHIL; ++i) {
      float pangle = (i*2*PI)/PHIL;
      phils[i].draw(*myCan,centerX+RAD*cos(pangle),centerY+RAD*sin(pangle));
      for (int j = 0; j < phils[i].getMeals(); ++j) {

        myCan->drawCircle(centerX+(RAD+64+8*(j%10))*cos(pangle+(j/10)*2*PI/RAD),
                          centerY+(RAD+64+8*(j%10))*sin(pangle+(j/10)*2*PI/RAD),
                          3,8,BROWN);
      }
      float fangle;
      if (forks[i].user == i)
        fangle = (i+0.25f)*ARC;
      else if((forks[i].user == (i+1)%PHIL))
        fangle = (i+0.75f)*ARC;
      else
        fangle = (i+0.5f)*ARC;
      forks[i].draw(*myCan,centerX+RAD*cos(fangle),centerY+RAD*sin(fangle));
    }
  }
};

void philosopherFunction(Canvas& can) {
  Table t(can);
  can.sleep();
  while(can.getIsOpen()) {
    std::cout << can.getFrameNumber() << std::endl;
//    t.checkStep(waitWhenBlocked);     //Deadlock
//    t.checkStep(forfeitWhenBlocked);  //Livelock
    t.checkStep(nFrameRelease);       //No locking; fair for N philosophers, N >= 5
    t.actStep();
    can.pauseDrawing();
    can.clear();
    t.drawStep();
    can.resumeDrawing();
    can.sleep();
  }
}

int main() {
    Canvas c(-1, -1, -1, -1, "Philosophers",0.1f);
    c.run(philosopherFunction);
}
