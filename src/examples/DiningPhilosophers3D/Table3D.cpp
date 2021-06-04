#include "Table3D.h"

/*!
 * \brief Creates a new 3D Table of dining Philosopher3Ds.
 * \details Explicit constructor for a new Table3D object.
 *  \param can The Canvas on which the Table3D is to be drawn.
 *  \param p Integer denoting the number of Philosopher3Ds at the Table3D
 *  \param m PhilMethod denoting how the Philosopher3Ds should interact.
 */
Table3D::Table3D(Canvas& can, int p, PhilMethod m) {
  numPhils = p;
  myCan = &can;
  myTable = new Cylinder(0,0,-55,100,150,0,0,90,ColorFloat(0.5,0.5,0.5,1));
  can.add(myTable);
  phils = new Philosopher3D[numPhils];
  forks = new Fork3D[numPhils];
  for (int i = 0; i < numPhils; ++i) {
    phils[i].setId(i,numPhils);
    forks[i].id = i;
    forks[i].setCanvas(myCan);
  }
  spaghettis = new Image*[numPhils]();
  float delta = 2.0f / numPhils * PI;
  for(int i = 0; i < numPhils; i++) {
    spaghettis[i] = new Image(95 * cos(i*delta), 105 * sin(i*delta), -1, "/usr/include/TSGL/assets/pics/spaghet.png", 100, 50, 0,0,0);
    can.add(spaghettis[i]);
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

  myCan2 = new Canvas(0,0,300,300,"Legend");
  myCan2->start();
  Background * bg2 = myCan2->getBackground();

  bg2->drawText(0,128,0,L"Method:","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",32,0,0,0,BLACK);
  bg2->drawText(0,96,0,L"\"" + methodString + L"\"","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,BLACK);
  bg2->drawText(0,64,0,L"Legend:","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,BLACK);
  bg2->drawText(0,32,0,L"Red: Hungry","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,RED);
  bg2->drawText(0,0,0,L"Orange: Has Right Fork","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,ORANGE);
  bg2->drawText(0,-32,0,L"Yellow: Has Left Fork","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,YELLOW);
  bg2->drawText(0,-64,0,L"Green: Eating","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,GREEN);
  bg2->drawText(0,-96,0,L"Blue: Thinking","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,BLUE);
  bg2->drawText(0,-121,0,L"Meals eaten:","/usr/include/TSGL/assets/freefont/FreeSerif.ttf",24,0,0,0,BROWN);

  bg2->drawRegularPolygon(65, -121,0,3,3,0,0,0, BROWN);
}

/*!
 * \brief Destructor for Table3D.
 */
Table3D::~Table3D() {
  if (myCan2->isOpen())
    myCan2->stop();
  delete myCan2;
  for (int i = 0; i < numPhils; i++)
    delete spaghettis[i];
  delete myTable;
  delete [] phils;
  delete [] forks;
}

 /*!
  * \brief Method for determining which fork a Philosopher3D should get.
  * \details
  * - Store the id numbers for the left and the right Philsopher's state.
  * - Switch for the state of a Philosopher3D:
  *   - Philosopher3D has no fork:
  *     - If the right fork is free, try to get that fork.
  *     - Else, if the left fork is free, try to get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has right fork:
  *     - If the left fork is free, try to get that fork.
  *     .
  *   - Philosopher3D has the left fork:
  *     - If the right fork is free, try to get that fork.
  *     - Else, release the left fork.
  *     .
  *   - Philosopher3D has both forks:
  *     - Release both of them.
  *     .
  *   .
  * .
  * \param id The id number of the current Philosopher3D.
  * \note This is an example of Deadlock amongst threads.
  */
void Table3D::forfeitWhenBlockedMethod(int id) {
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
  * \brief Method for determining which fork a Philosopher3D should get.
  * \details
  * - Store the states of the left and right Philosopher3Ds.
  * - Switch for the state of the current Philosopher3D:
  *   - Philosopher3D has no forks:
  *     - If the right fork is free, try to get that fork.
  *     - Else if the left fork is free, try to get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has right fork:
  *     - If the left fork is free, try to get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has the left fork:
  *     - If the right fork is free, try to get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has both forks:
  *     - Release both of them.
  *     .
  *   .
  * .
  * \param id The id number of the current Philosopher3D.
  * \note This is an example of Livelock amongst threads.
  */
void Table3D::waitWhenBlockedMethod(int id) {
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
  * \brief Method for determining which fork a Philosopher3D should get.
  * \details
  * - Store the states of the left and right Philosopher3Ds.
  * - Switch statement for the current Philosopher3D:
  *   - Philosopher3D has no forks:
  *     - If the right fork is free, try to get that fork.
  *     - Else, if the left fork is free, try to get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has right fork:
  *     - If the left fork is free, try to get that fork.
  *     - Else, if the id of the current Philosopher3D is equal to the frame number of the Canvas
  *       modulo the number of Philosopher3Ds+1, then release the right fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has the left fork:
  *     - If the right fork is free, try and get that fork.
  *     - Else, if the id of the current Philosopher3D is equal to the frame number of the Canvas
  *       modulo the number of Philosopher3Ds+1, then release the left fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has both forks:
  *     - Release both of them.
  *     .
  *   .
  * .
  * \param id The id number of the current Philosopher3D.
  */
void Table3D::nFrameReleaseMethod(int id) {
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
  * \brief Method for determining which fork a Philosopher3D should get.
  * \details
  * - Store the states for the left and right Philosopher3Ds.
  * - Switch statement for the state of the current Philosopher3D.
  *   - Philosopher3D has no forks:
  *     - If the right Philosopher3D's id is less than the left Philsopher's id:
  *       - If the right fork is free, try to get that fork.
  *       - Else, do nothing.
  *       .
  *     - Else, if the left fork is free then try and get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has the right fork:
  *     - If the left fork is free, try and get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has the left fork:
  *     - If the right fork is free, try and get that fork.
  *     - Else, do nothing.
  *     .
  *   - Philosopher3D has both forks:
  *     - Release both of them.
  *     .
  *   .
  * .
  * \param id The id number of the current Philosopher3D.
  */
void Table3D::hierarchyMethod(int id) {
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
  * \brief Method for determining which fork a Philosopher3D should get.
  * \details
  * - Switch statement for the current Philosopher3D:
  *   - Philosopher3D has no forks:
  *     - If the Philosopher3D's id is even
  *   - Philosopher3D has right fork (odd id):
  *     - If the Philsopher's id modulo 2 is equal to the Canvas' current frame number
  *       modulo 2, then try and get the left fork.
  *     - Else, release the right fork.
  *     .
  *   - Philosopher3D has left fork (even id):
  *     - If the Philsopher's id modulo 2 is equal to the Canvas' current frame number
  *       modulo 2, then try and get the right fork.
  *     - Else, release the left fork.
  *     .
  *   - Philosopher3D has both forks:
  *     - Release both of them.
  *     .
  *   .
  * .
  * \param id The id number of the current Philosopher3D.
  * \note This method is the one that works best.
  */
void Table3D::oddEvenMethod(int id) {
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
void Table3D::checkStep() {
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
void Table3D::actStep() {
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
void Table3D::drawStep() {
  const float RAD = 195;
  float FORK_RAD = 130;
  const float ARC =2*PI/numPhils;
  const float CLOSE = 0.15f;
  const float BASEDIST = RAD+100;

  int i = omp_get_thread_num();
  float pangle = (i*2*PI)/numPhils;
  ColorFloat fcolor = WHITE;
  float fangle = (i+0.5f)*ARC;

  if( !phils[i].hasCylinder() ) {
    phils[i].draw(*myCan,RAD*cos(pangle),RAD*sin(pangle));
  }

  phils[i].refreshColor();
  if( phils[i].state() == isFull ) {
    int meals = phils[i].getMeals();
    float angle = pangle+(meals/10)*2*PI/(RAD), dist = BASEDIST+8*(meals%10);
    phils[i].addMeal(dist*cos(angle), dist*sin(angle),0);
    Pyramid * p = phils[i].getLastMeal();
    p->setIsOutlined(false);
    myCan->add(p);
  }
  if (forks[i].user == i) {
    fangle = i*ARC + CLOSE;
    fcolor = (phils[i].state() == hasBoth) ? GREEN : PURPLE;
  }
  else if((forks[i].user == (i+1)%numPhils)) {
    fangle = ((i+1)*ARC) - CLOSE;
    fcolor = (phils[(i+1)%numPhils].state() == hasBoth) ? GREEN : ORANGE;
  } else {
    FORK_RAD = 120;
    fangle = pangle + PI / numPhils;
  }
  forks[i].draw(FORK_RAD*cos(fangle),FORK_RAD*sin(fangle),fangle,fcolor);
}