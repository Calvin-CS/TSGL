/*
 * testPhilosophers.cpp
 *
 * Usage: ./testPhilosophers <numPhilosophers> <speed>
 */

#include <omp.h>
#include <tsgl.h>

#include "DiningPhilosophers/Table.h"

using namespace tsgl;

void philosopherFunction(Canvas& can,int philosophers) {
  //Uncomment exactly one of the below constructors to select a resolution method
//  Table t(can,philosophers,waitWhenBlocked);    //Deadlock
//  Table t(can,philosophers,forfeitWhenBlocked); //Livelock (when synchronized)
//  Table t(can,philosophers,nFrameRelease);      //No locking; mostly fair for N philosophers, N >= 5
//  Table t(can,philosophers,resourceHierarchy);  //No locking; mostly fair for N philosophers, N >= 2
  Table t(can,philosophers,oddEven);            //No locking; perfectly fair for N philosophers, N >= 2

  #pragma omp parallel num_threads(philosophers)
  {
    while(can.isOpen()) {
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
    Canvas c(-1, -1, -1, -1, "Dining Philosophers",1.0f/speed);
    c.run(philosopherFunction,nphil);
}
