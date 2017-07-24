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
 * Usage: ./DiningPhilosophers <numPhilosophers> <speed> <resolutionMethodChar>
 * for <resolutionMethodChar> enter:
 *  'w' for waitWhenBlocked, which results in Deadlock
 *  'f' for forfeitWhenBlocked, which results in Livelock
 *  'n' for nCountRelease, which does not lock and is mostly fair for N philosophers, N >= 5
 *  'r' for resourceHierarchy, which does not lock and is mostly fair for N philosophers, N >= 2
 *  'o' for oddEven, which does not lock and is perfectly fair for N philosophers, N >= 2 (also default)
 */

#include <omp.h>
#include <tsgl.h>
#include <random>
#include "Table.h"
#include "Philosopher.h"

using namespace tsgl;

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
      method = nCountRelease;      //No locking; mostly fair for N philosophers, N >= 5
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
        if(method == forfeitWhenBlocked || method == waitWhenBlocked) { //Synchronize to see Livelock and Deadlock
        #pragma omp barrier               //Barrier for optional synchronization
        }
      	t.actStep();
        can.sleep(); // ensures each fork is only drawn once per frame
        t.drawStep();
        can.resumeDrawing();
		  }
      can.sleep();
    }
  }
}

int main(int argc, char* argv[]) {
    int  nphil = (argc > 1) ? atoi(argv[1]) : 5;  //Number of philosophers defaults to 5
    int  speed = (argc > 2) ? atoi(argv[2]) : 5; //Speed defaults to 5
    std::string resM  = (argc > 3) ? argv[3] : "o"; //ResolutionMethod defaults to oddEven
    Canvas c(-1, -1, -1, -1, "Dining Philosophers",1.0f/speed);
    c.setBackgroundColor(WHITE);
    c.run(philosopherFunction,nphil,resM);
}
