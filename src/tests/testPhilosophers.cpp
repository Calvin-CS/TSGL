/*
 * testPhilosophers.cpp
 *
 * Usage: ./testPhilosophers <numPhilosophers> <speed>
 */

#include <omp.h>
#include <tsgl.h>

using namespace tsgl;

/*! \brief Enum for valid states for the Dining Philosophers
 */
enum PhilState {
  hasNone, hasRight, hasLeft, hasBoth, isFull
};

/*! \brief Enum for valid actions for the Dining Philosophers
 */
enum PhilAction {
  doNothing, tryLeft, tryRight, tryBoth, releaseLeft, releaseRight, releaseBoth
};

/*! \brief Enum for resource collision resolution methods for the Dining Philosophers' problem
 */
enum PhilMethod {
  forfeitWhenBlocked, waitWhenBlocked, nFrameRelease, resourceHierarchy, oddEven
};

/*!
 * \struct Fork
 * \brief Small struct for the forks in the Dining Philosophers' problem
 */
struct Fork {
  int user, id;
  Fork() {
    user = -1; id = 0;
  }
  void draw(Canvas& can, int x, int y, ColorFloat c) {
    can.drawCircle(x,y,16,32,c,true);
  }
};

/*!
 * \class Philosopher
 * \brief Object representing a philosopher in the Dining Philosophers' problem
 * \details The Philosopher class contains variables and methods necessary for
 *   representing a philosopher at a table. Each Philosopher may acquire or release
 *   the fork to his left or to his right (or both), with his state changing
 *   accordingly.
 */
class Philosopher {
private:
  PhilState myState;
  PhilAction myAction;
  int id, myLeft, myRight, meals;
public:
  Philosopher() {
    setId(0,1); meals = 0; myState = hasNone; myAction = doNothing;
  }
  void draw(Canvas& can, int x, int y) {
    const int SIZE = 32;
    ColorFloat c;
    switch(myState) {
      case hasNone:  c=RED;    break;
      case hasRight: c=ORANGE; break;
      case hasLeft:  c=YELLOW; break;
      case hasBoth:  c=GREEN;  break;
      case isFull:   c=BLUE;   break;
    }
    can.drawCircle(x,y,SIZE,SIZE,c,true);
  }
  bool acquire(Fork& f) {
    if (f.user >= 0)
      return false;
    if (f.id == myLeft) {
      if (myState == hasNone)
        myState = hasLeft;
      else if (myState == hasRight)
        myState = hasBoth;
      else
        return false;
      f.user = id;
      return true;
    }
    if (f.id == myRight) {
      if (myState == hasNone)
        myState = hasRight;
      else if (myState == hasLeft)
        myState = hasBoth;
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
    if (myState != isFull)
      myState = (myState == ((f.id == myLeft) ? hasLeft : hasRight)) ? hasNone : isFull;
    f.user = -1;
    return true;
  }
  int getMeals() { return meals; }
  void eat() { ++meals; myState = hasNone; myAction = doNothing; }
  PhilState state() { return myState; }
  void setState(PhilState p) { myState = p; }
  PhilAction action() { return myAction; }
  void setAction(PhilAction a) { myAction = a; }
  int getId() { return id; }
  void setId(int i, int nphil) {id = myLeft = i; myRight = (id+nphil-1)%nphil; }
};

/*!
 * \class Table
 * \brief Object managing the forks and philosophers in the Dining Philosophers' problem.
 * \details The Table class keeps track of the forks and philosophers in the Dining
 *   Philosophers' problem; it additionally manages the actions of the philosophers.
 * \details Each step of the problem is broken up into two phases. In the checking phase,
 *   the philosophers look at the table around them and, without communicating with the
 *   other philosophers, determine an action to take based on their state and the states
 *   of their adjacent forks.
 * \details In the action phase, each philosopher attempts to execute the action previously
 *   determined in the checking phase. If unsuccessful, the philosopher does nothing;
 *   otherwise, the philosopher's state changes depending on the action taken.
 */
class Table {
private:
  int tabX, tabY, numPhils;
  PhilMethod myMethod;
  std::string methodString;
  Canvas *myCan, *myCan2;
  Philosopher *phils;
  Fork *forks;
public:
  Table(Canvas& can, int p, PhilMethod m) {
    numPhils = p;
    myCan = &can;
    tabX = can.getWindowWidth()/2;
    tabY = can.getWindowHeight()/2;
    phils = new Philosopher[numPhils];
    forks = new Fork[numPhils];
    for (int i = 0; i < numPhils; ++i) {
      phils[i].setId(i,numPhils);
      forks[i].id = i;
    }
    myMethod = m;
    switch(myMethod) {
      case forfeitWhenBlocked:
        methodString = "forfeit when blocked";
        break;
      case waitWhenBlocked:
        methodString = "wait when blocked";
        break;
      case nFrameRelease:
        methodString = "release on nth frame";
        break;
      case resourceHierarchy:
        methodString = "hierarchical resources";
        break;
      case oddEven:
        methodString = "odd-even check";
        break;
      default:
        break;
    }

    myCan2 = new Canvas(0,0,300,300,"Legend");
    myCan2->start();
    myCan2->drawText("Method:",16,32,32,BLACK);
    myCan2->drawText("\"" + methodString + "\"",32,64,24,BLACK);
    myCan2->drawText("Legend:",16,96,24,BLACK);
    myCan2->drawText("Red: Hungry",32,128,24,RED);
    myCan2->drawText("Orange: Has Right Fork",32,160,24,ORANGE);
    myCan2->drawText("Yellow: Has Left Fork",32,192,24,YELLOW);
    myCan2->drawText("Green: Eating",32,224,24,GREEN);
    myCan2->drawText("Blue: Thinking",32,256,24,BLUE);
  }

  ~Table() {
    if (myCan2->getIsOpen())
      myCan2->stop();
    else
      myCan2->wait();
    delete myCan2;
    delete [] phils;
    delete [] forks;
  }

  /*!
   * \brief Method for determining which fork a Philosopher should get.
   * \details
   * - Store the id numbers for the left and the right Philsopher's state.
   * - Switch for the state of a Philosopher:
   *   - Philosopher has no fork:
   *     - If the right fork is free, try to get that fork.
   *     - Else, if the left fork is free, try to get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has right fork:
   *     - If the left fork is free, try to get that fork.
   *     .
   *   - Philosopher has the left fork:
   *     - If the right fork is free, try to get that fork.
   *     - Else, release the left fork.
   *     .
   *   - Philosopher has both forks:
   *     - Release both of them.
   *     .
   *   .
   * .
   * \param id The id number of the current Philosopher.
   * \note This is an example of Deadlock amongst threads.
   */
  void forfeitWhenBlockedMethod(int id) {
    int left = id, right = (id+numPhils-1)%numPhils;
    switch(phils[id].state()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case hasRight:
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

  /*!
   * \brief Method for determining which fork a Philosopher should get.
   * \details
   * - Store the states of the left and right Philosophers.
   * - Switch for the state of the current Philosopher:
   *   - Philosopher has no forks:
   *     - If the right fork is free, try to get that fork.
   *     - Else if the left fork is free, try to get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has right fork:
   *     - If the left fork is free, try to get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has the left fork:
   *     - If the right fork is free, try to get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has both forks:
   *     - Release both of them.
   *     .
   *   .
   * .
   * \param id The id number of the current Philosopher.
   * \note This is an example of Livelock amongst threads.
   */
  void waitWhenBlockedMethod(int id) {
    int left = id, right = (id+numPhils-1)%numPhils;
    switch(phils[id].state()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case hasRight:
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

  /*!
   * \brief Method for determining which fork a Philosopher should get.
   * \details
   * - Store the states of the left and right Philosophers.
   * - Switch statement for the current Philosopher:
   *   - Philosopher has no forks:
   *     - If the right fork is free, try to get that fork.
   *     - Else, if the left fork is free, try to get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has right fork:
   *     - If the left fork is free, try to get that fork.
   *     - Else, if the id of the current Philosopher is equal to the frame number of the Canvas
   *       modulo the number of Philosophers+1, then release the right fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has the left fork:
   *     - If the right fork is free, try and get that fork.
   *     - Else, if the id of the current Philosopher is equal to the frame number of the Canvas
   *       modulo the number of Philosophers+1, then release the left fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has both forks:
   *     - Release both of them.
   *     .
   *   .
   * .
   * \param id The id number of the current Philosopher.
   */
  void nFrameReleaseMethod(int id) {
    int left = id, right = (id+numPhils-1)%numPhils;
    switch(phils[id].state()) {
      case hasNone:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else
          phils[id].setAction(doNothing);
        break;
      case hasRight:
        if (forks[left].user == -1)
          phils[id].setAction(tryLeft);
        else {
          if (id == (myCan->getFrameNumber() % numPhils+1))
            phils[id].setAction(releaseRight);
          else
            phils[id].setAction(doNothing);
        }
        break;
      case hasLeft:
        if (forks[right].user == -1)
          phils[id].setAction(tryRight);
        else {
          if (id == (myCan->getFrameNumber() % numPhils+1))
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

  /*!
   * \brief Method for determining which fork a Philosopher should get.
   * \details
   * - Store the states for the left and right Philosophers.
   * - Switch statement for the state of the current Philosopher.
   *   - Philosopher has no forks:
   *     - If the right Philosopher's id is less than the left Philsopher's id:
   *       - If the right fork is free, try to get that fork.
   *       - Else, do nothing.
   *       .
   *     - Else, if the left fork is free then try and get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has the right fork:
   *     - If the left fork is free, try and get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has the left fork:
   *     - If the right fork is free, try and get that fork.
   *     - Else, do nothing.
   *     .
   *   - Philosopher has both forks:
   *     - Release both of them.
   *     .
   *   .
   * .
   * \param id The id number of the current Philosopher.
   */
  void hieararchyMethod(int id) {
    int left = id, right = (id+numPhils-1)%numPhils;
    switch(phils[id].state()) {
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
      case hasRight:
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

  /*!
   * \brief Method for determining which fork a Philosopher should get.
   * \details
   * - Switch statement for the current Philosopher:
   *   - Philosopher has no forks:
   *     - If the Philosopher's id is even
   *   - Philosopher has right fork (odd id):
   *     - If the Philsopher's id modulo 2 is equal to the Canvas' current frame number
   *       modulo 2, then try and get the left fork.
   *     - Else, release the right fork.
   *     .
   *   - Philosopher has left fork (even id):
   *     - If the Philsopher's id modulo 2 is equal to the Canvas' current frame number
   *       modulo 2, then try and get the right fork.
   *     - Else, release the left fork.
   *     .
   *   - Philosopher has both forks:
   *     - Release both of them.
   *     .
   *   .
   * .
   * \param id The id number of the current Philosopher.
   * \note This method is the one that works best.
   */
  void oddEvenMethod(int id) {
    switch(phils[id].state()) {
      case hasNone:
        if ((id % 2) == (myCan->getFrameNumber() % 2))
          phils[id].setAction(tryBoth);
        else
          phils[id].setAction(doNothing);
        break;
      case hasRight:
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

  void checkStep() {
    int i = omp_get_thread_num();
    if (phils[i].state() == isFull) {
      phils[i].eat();
      return;
    }
    switch(myMethod) {
      case forfeitWhenBlocked:
        forfeitWhenBlockedMethod(i);
        break;
      case waitWhenBlocked:
        waitWhenBlockedMethod(i);
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
  }

  void actStep() {
    int i = omp_get_thread_num();
    int left = i, right = (i+numPhils-1)%numPhils;
    switch(phils[i].action()) {
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
  }

  void drawStep() {
    const int RAD = 300;
    const float ARC =2*PI/numPhils;
    const float CLOSE = 0.15f;
    const float BASEDIST = RAD+64;

    myCan->drawCircle(tabX,tabY,RAD-48,RAD,DARKGRAY);
    int i = omp_get_thread_num();
//    for (int i = 0; i < numPhils; ++i) {
      float pangle = (i*2*PI)/numPhils;
      ColorFloat fcolor = BLACK;
      float fangle = (i+0.5f)*ARC;

      phils[i].draw(*myCan,tabX+RAD*cos(pangle),tabY+RAD*sin(pangle));
      for (int j = 0; j < phils[i].getMeals(); ++j) {
        float angle = pangle+(j/10)*2*PI/RAD, dist = BASEDIST+8*(j%10);
        myCan->drawCircle(tabX+dist*cos(angle), tabY+dist*sin(angle), 3,8,BROWN);
      }
      if (forks[i].user == i) {
        fangle = i*ARC + CLOSE;
        fcolor = (phils[i].state() == hasBoth) ? GREEN : YELLOW;
      }
      else if((forks[i].user == (i+1)%numPhils)) {
        fangle = ((i+1)*ARC) - CLOSE;
        fcolor = (phils[(i+1)%numPhils].state() == hasBoth) ? GREEN : ORANGE;
      }
      forks[i].draw(*myCan,tabX+RAD*cos(fangle),tabY+RAD*sin(fangle),fcolor);
//    }
  }
};

void philosopherFunction(Canvas& can,int philosophers) {
  //Uncomment exactly one of the below constructors to select a resolution method
//  Table t(can,philosophers,waitWhenBlocked);    //Deadlock
//  Table t(can,philosophers,forfeitWhenBlocked); //Livelock (when synchronized)
//  Table t(can,philosophers,nFrameRelease);      //No locking; mostly fair for N philosophers, N >= 5
//  Table t(can,philosophers,resourceHierarchy);  //No locking; mostly fair for N philosophers, N >= 2
  Table t(can,philosophers,oddEven);            //No locking; perfectly fair for N philosophers, N >= 2

  can.sleep();
  #pragma omp parallel num_threads(philosophers)
  {
    while(can.getIsOpen()) {
      t.checkStep();
      can.pauseDrawing();
//      #pragma omp barrier               //Barrier for optional synchronization
      t.actStep();
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
