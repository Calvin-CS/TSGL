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
  Background * bg = can.getBackground();
  bg->drawCircle(0,0,0,175,0,0,0,ColorFloat(0.5,0.5,0.5,1));
  phils = new Philosopher[numPhils];
  forks = new Fork[numPhils];
  for (int i = 0; i < numPhils; ++i) {
    phils[i].setId(i,numPhils);
    forks[i].id = i;
    forks[i].setCanvas(myCan);
  }
  float delta = 2.0f / numPhils * PI;
  for(int i = 0; i < numPhils; i++) {
    bg->drawImage(120 * cos(i*delta), 140 * sin(i*delta), 0, "./assets/pics/spaghet.png", 100, 50, 0,0,0);
  }
  myMethod = m;
  switch(myMethod) {
    case forfeitWhenBlocked:
      methodString = L"forfeit when blocked";
      break;
    case waitWhenBlocked:
      methodString = L"wait when blocked";
      break;
    case nFrameRelease:
      methodString = L"release on nth frame";
      break;
    case resourceHierarchy:
      methodString = L"hierarchical resources";
      break;
    case oddEven:
      methodString = L"odd-even check";
      break;
    default:
      break;
  }

  myCan2 = new Canvas(0,0,300,300,"Legend", GRAY);
  myCan2->start();
  Background * bg2 = myCan2->getBackground();

  bg2->drawText(0,128,0,L"Method:","./assets/freefont/FreeSerif.ttf",32,0,0,0,BLACK);
  bg2->drawText(0,96,0,L"\"" + methodString + L"\"","./assets/freefont/FreeSerif.ttf",24,0,0,0,BLACK);
  bg2->drawText(0,64,0,L"Legend:","./assets/freefont/FreeSerif.ttf",24,0,0,0,BLACK);
  bg2->drawText(0,32,0,L"Red: Hungry","./assets/freefont/FreeSerif.ttf",24,0,0,0,RED);
  bg2->drawText(0,0,0,L"Orange: Has Right Fork","./assets/freefont/FreeSerif.ttf",24,0,0,0,ORANGE);
  bg2->drawText(0,-32,0,L"Yellow: Has Left Fork","./assets/freefont/FreeSerif.ttf",24,0,0,0,YELLOW);
  bg2->drawText(0,-64,0,L"Green: Eating","./assets/freefont/FreeSerif.ttf",24,0,0,0,GREEN);
  bg2->drawText(0,-96,0,L"Blue: Thinking","./assets/freefont/FreeSerif.ttf",24,0,0,0,BLUE);
  bg2->drawText(0,-121,0,L"Meals eaten:","./assets/freefont/FreeSerif.ttf",24,0,0,0,BROWN);

  bg2->drawRegularPolygon(65, -121,0,3,3,0,0,0, BROWN);
}

/*!
 * \brief Destructor for Table.
 */
Table::~Table() {
  if (myCan2->isOpen())
    myCan2->stop();
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
  // myCan2->sleep();
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
  const float RAD = 225;
  float FORK_RAD = 175;
  const float ARC =2*PI/numPhils;
  const float CLOSE = 0.15f;
  const float BASEDIST = RAD+54;

  int i = omp_get_thread_num();
  float pangle = (i*2*PI)/numPhils;
  ColorFloat fcolor = ColorFloat(1,1,1,1);
  float fangle = (i+0.5f)*ARC;

  if( !phils[i].hasCircle() ) {
    phils[i].draw(*myCan,RAD*cos(pangle),RAD*sin(pangle));
  }

  phils[i].refreshColor();
  if( phils[i].state() == isFull ) {
    int meals = phils[i].getMeals();
    float angle = pangle+(meals/10)*2*PI/(RAD), dist = BASEDIST+8*(meals%10);
    phils[i].addMeal();
    myCan->getBackground()->drawRegularPolygon(dist*cos(angle),dist*sin(angle),0,3,3,0,0,0,ColorFloat(0.5,0.3,0,1));
  }
  if (forks[i].user == i) {
    fangle = i*ARC + CLOSE;
    fcolor = (phils[i].state() == hasBoth) ? GREEN : PURPLE;
  }
  else if((forks[i].user == (i+1)%numPhils)) {
    fangle = ((i+1)*ARC) - CLOSE;
    fcolor = (phils[(i+1)%numPhils].state() == hasBoth) ? GREEN : ORANGE;
  } else {
    FORK_RAD = 140;
    fangle = pangle + PI / numPhils;
  }
  forks[i].draw(FORK_RAD*cos(fangle),FORK_RAD*sin(fangle),fangle,fcolor);
}