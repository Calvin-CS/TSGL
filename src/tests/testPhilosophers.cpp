/*
 * testPhilosophers.cpp runs the Dining Philosphers Problem animation using the TSGL library and OpenMP.
 * This file includes a main method, Philospher class, Fork class, and Table class.
 *
 * The program provides a visualization of the Dining Philosophers Problem 
 *  in which philosophers sit around a table, think for a random amount of time, and then want to eat.
 * In order to eat, each philosopher needs the forks to their right and left, shared with the other philosophers.
 * This visualization includes six different ways of resolving the conflicts.
 * See also: https://en.wikipedia.org/wiki/Dining_philosophers_problem.
 *
 * Usage: ./testPhilosophers <numPhilosophers> <speed> <resolutionMethodChar>
 * for <resolutionMethodChar> enter:
 *  'w' for waitWhenBlocked, which results in Deadlock
 *  'f' for forfeitWhenBlocked, which results in Livelock
 *  'n' for nFrameRelease, which does not lock and is mostly fair for N philosophers, N >= 5
 *  'r' for resourceHierarchy, which does not lock and is mostly fair for N philosophers, N >= 2
 *  'o' for oddEven, which does not lock and is perfectly fair for N philosophers, N >= 2 (default)
 */

#include <omp.h>
#include <tsgl.h>
#include <random>

using namespace tsgl;

/*! \brief Enum for valid states for the Dining Philosophers
 */
enum PhilState {
  hasNone, hasRight, hasLeft, hasBoth, isFull, thinking
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
  void draw(Canvas& can, int x, int y, double angle, ColorFloat c) {
    const int POINTS = 20; // number of vertices in polygon
    const int HEIGHT = 42; // 42 is preferred
    const int WIDTH = 12;  // 12 is preferred
    int xs[POINTS], ys[POINTS];
    
    // scales (out of 100) for the dimensions of the fork
    double xscale[POINTS] = {0, 19, 19, 27, 27, 46, 46, 54, 54, 73, 73, 81, 81, 100, 100, 65,  65,  35, 35,  0};
    double yscale[POINTS] = {0,  0, 20, 20,  0,  0, 20, 20,  0,  0, 20, 20,  0,   0,  30, 30, 100, 100, 30, 30};
    
    // create the fork points from the scale arrays
    for(int i = 0; i < POINTS; ++i) {
		  // scale the fork
      xs[i] = WIDTH  * xscale[i];
		  ys[i] = HEIGHT * yscale[i];
      xs[i] = xs[i]/100;
      ys[i] = ys[i]/100;
      
      // move so (0, 0) is center, not top left corner
      xs[i] -= WIDTH/2;
      ys[i] -= HEIGHT/2;
    } 
    
    ColorFloat colors[POINTS]; //Array of colors to draw points of Fork
    
    // rotate fork around 0,0 by angle
    angle -= PI/2; // rotate by PI/2 radians or 90 degrees for fork next to philosopher
      // if adding PI/2, then the forks point out, if subtracting PI/2 the forks point in to table
        // without this line, the forks are perpendicular to philosophers
    
    for(int i = 0; i < POINTS; ++i) {
      double a =  xs[i]*cos(angle) - ys[i]*sin(angle);
      double b =  xs[i]*sin(angle) + ys[i]*cos(angle);
      xs[i] = a+x; // transpose fork to be around table
      ys[i] = b+y;
      
      // set the color
      colors[i] = c;
    }

    can.drawConcavePolygon(POINTS, xs, ys, colors, true); // draw fork
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
      case hasLeft:  c=PURPLE; break;
      case hasBoth:  c=GREEN;  break;
      case isFull:   c=BLUE;   break;
      case thinking: c=BLUE;   break;
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
  void think() {
    if(rand()%3 == 0) {
      setState(hasNone);
      setAction(doNothing);
    }
  }
  int getMeals() { return meals; }
  void eat() { ++meals; myState = thinking; myAction = doNothing; }
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

    myCan2 = new Canvas(0,0,350,300,"Legend");
    myCan2->setBackgroundColor(WHITE);
    myCan2->start();
    myCan2->drawText("Method:",16,32,32,BLACK);
    myCan2->drawText("\"" + methodString + "\"",32,64,24,BLACK);
    myCan2->drawText("Legend:",16,96,24,BLACK);
    myCan2->drawText("Red: Hungry",32,128,24,RED);
    myCan2->drawText("Orange: Has Right Fork",32,160,24,ORANGE);
    myCan2->drawText("Purple: Has Left Fork",32,192,24,PURPLE);
    myCan2->drawText("Green: Eating",32,224,24,GREEN);
    myCan2->drawText("Blue: Thinking",32,256,24,BLUE);
    myCan2->drawText("Meals eaten",57,288,24,BROWN);
    myCan2->drawCircle(41,279,3,8,BROWN);
  }

  ~Table() {
    if (myCan2->isOpen())
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
   *     - Else, release the right fork.
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
   * \note This is an example of Livelock amongst threads.
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
      case thinking:
        phils[id].setState(hasNone);
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
   * \note This is an example of Deadlock amongst threads.
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
      case thinking:
        phils[id].think();
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
      case thinking:
        phils[id].think();
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
      case thinking:
        phils[id].think();
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
      case thinking:
        phils[id].think();
        break;
      default:
        break;
    }
  }

  /*!
   * \brief Method for determining which method of resolution the philosopher is using.
   */
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

  /*!
   * \brief Method for philosopher to act based on myAction.
   */
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

  /*!
   * \brief Method calculating angles calling draw methods of a philosopher and its fork or forks.
   */
  void drawStep() {
    const int RAD = 250;
    const float ARC =2*PI/numPhils;
    const float CLOSE = 0.175f;
    const float BASEDIST = RAD+64;

    myCan->drawCircle(tabX,tabY,RAD-48,RAD,DARKGRAY);
    int i = omp_get_thread_num();
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
        fcolor = (phils[i].state() == hasBoth) ? GREEN : PURPLE;
      }
      else if((forks[i].user == (i+1)%numPhils)) {
        fangle = ((i+1)*ARC) - CLOSE;
        fcolor = (phils[(i+1)%numPhils].state() == hasBoth) ? GREEN : ORANGE;
      }
      forks[i].draw(*myCan,tabX+RAD*cos(fangle),tabY+RAD*sin(fangle),fangle,fcolor);
  }
  
};

void philosopherFunction(Canvas& can,int philosophers, std::string RM) {

  PhilMethod method;
  //switch statement to create table with resolution method
  char resolutionMethod =  RM[0];
  switch(resolutionMethod) {
    case 'w':
      method = waitWhenBlocked;    //Deadlock
      break;
    case 'f':
      method = forfeitWhenBlocked; //Livelock (when synchronized)
      break;
    case 'n':
      method = nFrameRelease;      //No locking; mostly fair for N philosophers, N >= 5
      break;
    case 'r':
      method = resourceHierarchy;  //No locking; mostly fair for N philosophers, N >= 2
      break;
    case 'o':
      method = oddEven;           //No locking; perfectly fair for N philosophers, N >= 2
      break;
    default:
      method = oddEven;           //No locking; perfectly fair for N philosophers, N >= 2
      break;
  }
  
  Table t(can,philosophers,method);
  
  srand(time(NULL)); // seed the random number generator for thinking steps

  bool paused = false; // Flag that determines whether the animation is paused
  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&paused]() { // toggle pause when spacebar is pressed
		paused = !paused;
	});
  
  #pragma omp parallel num_threads(philosophers)
  {
    while(can.isOpen()) {
		  if (!paused) {
      		t.checkStep();
      		can.pauseDrawing();
        if(method == forfeitWhenBlocked || method == waitWhenBlocked) {
        #pragma omp barrier               //Barrier for optional synchronization
        }
      		t.actStep();
      		can.clear();
          can.sleep(); // ensures each fork is only drawn once per frame
          t.drawStep();
          can.resumeDrawing();
		  }
      can.sleep();
    }
  }
}

int main(int argc, char* argv[]) {
    int  nphil = (argc > 1) ? atoi(argv[1]) : 5;
    int  speed = (argc > 2) ? atoi(argv[2]) : 10;
    std::string resM  = (argc > 3) ? argv[3] : "o";
    Canvas c(-1, -1, -1, -1, "Dining Philosophers",1.0f/speed);
    c.setBackgroundColor(WHITE);
    c.run(philosopherFunction,nphil,resM);
}
