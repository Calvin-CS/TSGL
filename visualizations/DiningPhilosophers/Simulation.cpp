#include "Simulation.h"

Simulation::Simulation(Canvas& can, int p, PhilMethod m) {
  numPhils = p;
  myCan = &can;
  counter = 0;
  tabX = can.getWindowWidth()/2;
  tabY = can.getWindowHeight()/2;
  myCircle = NULL;
  phils = new Philosopher[numPhils];
  forks = new Fork[numPhils];
  for (int i = 0; i < numPhils; ++i) {
    phils[i].setId(i,numPhils);
    forks[i].id = i;
    forks[i].setCanvas(&can);
  }
  myMethod = m;
  std::string methodString;
  switch(myMethod) {
    case forfeitWhenBlocked:
      methodString = "forfeit when blocked";
      break;
    case waitWhenBlocked:
      methodString = "wait when blocked";
      break;
    case nCountRelease:
      methodString = "release on nth count";
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

  myCan2 = new Canvas(0,0,350,550,"Legend");
  myCan2->setBackgroundColor(WHITE);
  myCan2->start();
  int offset = 45;
  Text * methodText = new Text("Method:",16,32,24,BLACK);
  Text * methodText2 = new Text(methodString,32,64,24,BLACK);
  Text * legendText = new Text("Meals:",16,96,24,BLACK);
  Text * mealsText = new Text("= one meal",85,120,24,BROWN);
  RegularPolygon * mealExample = new RegularPolygon(66,115,3,8,BROWN); myCan2->add( mealExample );\
  Text * philText = new Text("Philosophers:", 16, 143, 24, BLACK);
  Text * blueText = new Text("Thinking",85,150+offset,24,BLUE);
  Text * orangeText = new Text("With Right Fork",85,225+offset,24,ORANGE);
  Text * purpleText = new Text("With Left Fork",85,300+offset,24,PURPLE);
  Text * greenText = new Text("Eating",85,375+offset,24,GREEN);
  Text * redText = new Text("Hungry",85,450+offset,24,RED);
  myCan2->add( philText );
  myCan2->add( methodText ); myCan2->add( methodText2 ); myCan2->add( legendText );
  myCan2->add( redText ); myCan2->add( orangeText ); myCan2->add( purpleText );
  myCan2->add( greenText ); myCan2->add( blueText ); myCan2->add( mealsText );

  Circle * hungryPhil = new Circle(48, 440+offset, 30, RED);
  Circle * rightPhil = new Circle(48, 215+offset, 30, ORANGE);
  Circle * leftPhil = new Circle(48, 290+offset, 30, PURPLE);
  Circle * eatPhil = new Circle(48, 365+offset, 30, GREEN);
  Circle * thinkPhil = new Circle(48, 140+offset, 30, BLUE);
  myCan2->add( hungryPhil ); myCan2->add( rightPhil ); myCan2->add( leftPhil ); myCan2->add( eatPhil ); myCan2->add( thinkPhil );

  legendShapes.push_back(methodText); legendShapes.push_back(methodText2); legendShapes.push_back(legendText); legendShapes.push_back(redText);
  legendShapes.push_back(orangeText); legendShapes.push_back(purpleText); legendShapes.push_back(greenText); legendShapes.push_back(blueText);
  legendShapes.push_back(mealsText); legendShapes.push_back(mealExample); legendShapes.push_back(hungryPhil); legendShapes.push_back(rightPhil);
  legendShapes.push_back(leftPhil); legendShapes.push_back(eatPhil); legendShapes.push_back(thinkPhil);
}

Simulation::~Simulation() {
  if (myCan2->isOpen())
    myCan2->stop();
  else
    myCan2->wait();
  delete myCan2;
  delete myCircle;
  delete [] phils;
  delete [] forks;
  for(unsigned i = 0; i < legendShapes.size(); i++) {
    delete legendShapes[i];
  }
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
void Simulation::forfeitWhenBlockedMethod(int id) {
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
void Simulation::waitWhenBlockedMethod(int id) {
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
 *     - Else, if the id of the current Philosopher is equal to the counter
 *       modulo the number of Philosophers+1, then release the right fork.
 *     - Else, do nothing.
 *     .
 *   - Philosopher has the left fork:
 *     - If the right fork is free, try and get that fork.
 *     - Else, if the id of the current Philosopher is equal to the counter
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
void Simulation::nCountReleaseMethod(int id) {
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
        if (id == (counter % numPhils+1))
          phils[id].setAction(releaseRight);
        else
          phils[id].setAction(doNothing);
      }
      break;
    case hasLeft:
      if (forks[right].user == -1)
        phils[id].setAction(tryRight);
      else {
        if (id == (counter % numPhils+1))
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
void Simulation::hierarchyMethod(int id) {
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
 *     - If the Philsopher's id modulo 2 is equal to the current counter
 *       modulo 2, then try and get the left fork.
 *     - Else, release the right fork.
 *     .
 *   - Philosopher has left fork (even id):
 *     - If the Philsopher's id modulo 2 is equal to the current counter
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
void Simulation::oddEvenMethod(int id) {
  switch(phils[id].state()) {
    case hasNone:
      if ((id % 2) == (counter % 2))
        phils[id].setAction(tryBoth);
      else
        phils[id].setAction(doNothing);
      break;
    case hasRight:
      if ((id % 2) == (counter % 2))
        phils[id].setAction(tryLeft);
      else {
        phils[id].setAction(releaseRight);
      }
      break;
    case hasLeft:
      if ((id % 2) == (counter % 2))
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
void Simulation::checkStep() {
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
    case nCountRelease:
      nCountReleaseMethod(i);
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
  counter++; //Tracks number of steps for some resolution methods
}

/*!
 * \brief Method for philosopher to act based on myAction.
 */
void Simulation::actStep() {
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
void Simulation::drawStep() {
  const int RAD = 250;
  int FORK_RAD = 200;
  const float ARC =2*PI/numPhils;
  const float CLOSE = 0.175f;
  const float BASEDIST = RAD+64;

  int i = omp_get_thread_num(); //Get index of calling Philosopher
  float pangle = (i*2*PI)/numPhils; //Philosopher angle
  ColorFloat fcolor = BLACK; //Fork color
  float fangle = (i+0.5f)*ARC; //Fork angle

  if( !myCircle ) { //If Simulation not already created
    myCircle = new Circle(tabX,tabY,RAD-48,DARKGRAY);
    myCan->add(myCircle);
  }
  if( !phils[i].hasCircle() ) //If Philosopher drawn for first time
    phils[i].draw(*myCan,tabX+RAD*cos(pangle),tabY+RAD*sin(pangle)); //Draw Philosopher

  phils[i].refreshColor(); //Update the color of philosopher
  if( phils[i].state() == isFull ) { //Draw the next meal if philosopher eating
      float angle = pangle+(phils[i].getMeals()/10)*2*PI/RAD, dist = BASEDIST+8*(phils[i].getMeals()%10);
      if(angle > (pangle+2*PI/numPhils)) {
        for(int n = 0; n < numPhils; n++) {
          phils[n].clearMeals(*myCan);
        }
        angle = pangle+(phils[i].getMeals()/10)*2*PI/RAD, dist = BASEDIST+8*(phils[i].getMeals()%10);
      }
      RegularPolygon * meal = new RegularPolygon(tabX+dist*cos(angle), tabY+dist*sin(angle), 3,8,BROWN);
      phils[i].addMeal(*myCan, meal);
  }
  if (forks[i].user == i) { //Fork at this index belongs to this
    fangle = i*ARC + CLOSE;
    fcolor = (phils[i].state() == hasBoth) ? GREEN : PURPLE;
  }
  else if((forks[i].user == (i+1)%numPhils)) { //Fork at this index belongs to neighbor
    fangle = ((i+1)*ARC) - CLOSE;
    fcolor = (phils[(i+1)%numPhils].state() == hasBoth) ? GREEN : ORANGE;
  } else {
    FORK_RAD = 170; //If unheld, Fork goes in to table
  }
  forks[i].draw(tabX+FORK_RAD*cos(fangle),tabY+FORK_RAD*sin(fangle),fangle,fcolor); //Draw fork
}
