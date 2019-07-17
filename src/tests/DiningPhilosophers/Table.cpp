#include "Table.h"

/*!
 * \brief Creates a new Table of dining philosophers.
 * \details Explicit constructor for a new Table object.
 *  \param can The Canvas on which the Table is to be drawn.
 *  \param p Integer denoting the number of Philosophers at the Table
 *  \param m PhilMethod denoting how the Philosophers should interact.
 */
Table::Table(Canvas& can, int p, PhilMethod m) {
  numPhils = p;
  myCan = &can;
  tabX = can.getWindowWidth()/2;
  tabY = can.getWindowHeight()/2;
  can.drawCircle(tabX,tabY,252,DARKGRAY);
  phils = new Philosopher[numPhils];
  forks = new Fork[numPhils];
  for (int i = 0; i < numPhils; ++i) {
    phils[i].setId(i,numPhils);
    forks[i].id = i;
    forks[i].setCanvas(myCan);
  }
  float delta = 2.0f / numPhils * PI;
  for(int i = 0; i < numPhils; i++) {
    myCan->drawImage("../assets/pics/spaghet.png", tabX-50+(200)*cos(i*delta), tabY-25+(215)*sin(i*delta), 100, 50, 1.0f);
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
  myCan2->drawText("Meals eaten: ",32,288,24,BROWN);
  myCan2->drawCircle(165,281,3,BROWN);
}

/*!
 * \brief Destructor for Table.
 */
Table::~Table() {
  if (myCan2->isOpen())
    myCan2->stop();
  else
    myCan2->wait();
  delete myCan2;
  delete spaghetti;
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
void Table::forfeitWhenBlockedMethod(int id) {
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
  * \note This is an example of Livelock amongst threads.
  */
void Table::waitWhenBlockedMethod(int id) {
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
void Table::nFrameReleaseMethod(int id) {
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
void Table::hierarchyMethod(int id) {
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
void Table::oddEvenMethod(int id) {
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
void Table::checkStep() {
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
      hierarchyMethod(i);
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
void Table::actStep() {
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
void Table::drawStep() {
  const int RAD = 300;
  int FORK_RAD = 250;
  const float ARC =2*PI/numPhils;
  const float CLOSE = 0.15f;
  const float BASEDIST = RAD+64;

  int i = omp_get_thread_num();
  float pangle = (i*2*PI)/numPhils;
  ColorFloat fcolor = BLACK;
  float fangle = (i+0.5f)*ARC;

  if( !phils[i].hasCircle() ) {
    phils[i].draw(*myCan,tabX+RAD*cos(pangle),tabY+RAD*sin(pangle));
  }

  phils[i].refreshColor();
  if( phils[i].state() == isFull ) {
      int j = phils[i].getMeals();
      float angle = pangle+(j/10)*2*PI/RAD; 
      float dist = BASEDIST+8*(j%10);
      myCan->drawCircle(tabX+dist*cos(angle), tabY+dist*sin(angle), 3,BROWN);
      phils[i].addMeal();
  }
  if (forks[i].user == i) {
    fangle = i*ARC + CLOSE;
    fcolor = (phils[i].state() == hasBoth) ? GREEN : PURPLE;
  }
  else if((forks[i].user == (i+1)%numPhils)) {
    fangle = ((i+1)*ARC) - CLOSE;
    fcolor = (phils[(i+1)%numPhils].state() == hasBoth) ? GREEN : ORANGE;
  } else {
    FORK_RAD = 220;
    fangle = pangle + PI / numPhils;
  }
  forks[i].draw(tabX+FORK_RAD*cos(fangle),tabY+FORK_RAD*sin(fangle),fangle,fcolor);
}