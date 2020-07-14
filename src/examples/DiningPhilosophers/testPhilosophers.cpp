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
 *  'n' for nFrameRelease, which does not lock and is mostly fair for N philosophers, N >= 5
 *  'r' for resourceHierarchy, which does not lock and is mostly fair for N philosophers, N >= 2
 *  'o' for oddEven, which does not lock and is perfectly fair for N philosophers, N >= 2 (also default)
 *
 * for <speed> enter:
 *  a number, such as 2, 5, or 10, to specify speed (increasing with higher numbers).
 *  't' or 'y' to turn on step-through. Press the spacebar to proceed at each step.
 *
 * If step-through is turned off, the spacebar pauses the visualization.
 */

#include <omp.h>
#include <tsgl.h>
#include <random>
#include "Table.h"
#include "Philosopher.h"

using namespace tsgl;

void philosopherFunction(Canvas& can,int philosophers, std::string RM, bool step) {

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

  bool stepThrough = step; // Flag that determines whether the animation pauses between steps
  bool paused = false; // Flag that determines whether the animation is paused
  bool philPauses[philosophers];
  for(int i = 0; i < philosophers; i++) {
    philPauses[i] = false;
  }
  can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&paused,&philPauses,&philosophers]() { // toggle pause when spacebar is pressed
		paused = !paused;
    for(int i = 0; i < philosophers; i++) {
      philPauses[i] = false;
    }
	});

  #pragma omp parallel num_threads(philosophers)
  {
  while(can.isOpen()) {
    can.sleep();
		  if ((!stepThrough && !paused) || (stepThrough && !philPauses[omp_get_thread_num()])) {
        if(stepThrough) { philPauses[omp_get_thread_num()] = true; }
      	t.checkStep();
      	can.pauseDrawing();
        if(method == forfeitWhenBlocked) { //Synchronize to see Livelock
        #pragma omp barrier               //Barrier for optional synchronization
        }
      	t.actStep();
        t.drawStep();
        can.resumeDrawing();
		  }
    }
  }
}

int main(int argc, char* argv[]) {
  if( argc == 1) {
    std::cout << "\nTo run the program with different values, use the format:\n\t./DiningPhilosophers <numPhilosophers> <speed> <resolutionMethodChar>"
    << "\nwhere <resolutionMethodChar> is a character specifying conflict resolution of the philosophers. Find options in Table.h" << std::endl;
  }
  int  nphil = (argc > 1) ? atoi(argv[1]) : 5;  //Number of philosophers defaults to 5
  int  speed = (argc > 2 && atoi(argv[2]) > 0) ? atoi(argv[2]) : 10; //Speed defaults to 5
  bool stepThrough = (argc > 2 && ((std::string(argv[2]) == "t") || (std::string(argv[2]) == "y")));
  std::string resM  = (argc > 3) ? argv[3] : "o"; //ResolutionMethod defaults to oddEven
  Canvas c(-1, -1, 1300, 1000, "Dining Philosophers",1.0f/speed);
  c.run(philosopherFunction,nphil,resM,stepThrough);
}
