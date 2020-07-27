/*
 * testSeaUrchin.cpp
 *
 * Usage: ./testSeaUrchin <numThreads>
 */

#include "tsgl.h"
#include <omp.h>

#include "SeaUrchin.h"

using namespace tsgl;

// possible FIXME: sometimes this results in a segmentation fault. no discernable pattern+

/*!
 * \brief Displays different colored sea urchins and takes a command-line argument.
 * \details Displays different colored sea urchins where each sea urchin is drawn by a thread
 * (the command-line argument is the number of threads to use).
 * It is drawn as follows:
 * - The sea urchins are drawn in a similar way as the line fan in testLineFan.
 * - A class contains all of the necessary data and methods to draw the sea urchins.
 * - A parallel block is created and the process is forked.
 * - The current thread's id number is stored.
 * - When you create the SeaUrchin object:
 *  - Set old x and y-coordinate values and increment them based off of the current thread's id number.
 *  - Set new x and y-coordinate values to 0.
 *  - Assign a color to the current thread.
 *  .
 * - While the Canvas is open:
 *   - Sleep the Canvas' internal timer until the next draw cycle.
 *   - Clear the Canvas.
 *   - When you draw the SeaUrchin onto the Canvas:
 *     - Set a delta value to make the SeaUrchins spin.
 *     - For i to the number of spokes of a SeaUrchin:
 *       - Calculate the new x and y-coordinate values based off of the delta value and the old x and y-coordinate values.
 *       - Draw a spoke of the SeaUrchin on the Canvas.
 *       .
 *     .
 *   .
 * - If the Canvas has been closed, output a message to the console saying "YOU KILLED MY SEA URCHINS! :'(" .
 * .
 * \param can Reference to the Canvas being drawn on.
 * \param threads Reference to the number of threads to use in the process.
 * \see testLineFan, SeaUrchin class.
 */
void seaUrchinFunction(Canvas& can, int threads) {
  #pragma omp parallel num_threads(threads)
  {
    SeaUrchin s(can, omp_get_thread_num());   //A thread gets a Sea Urchin
    while(can.isOpen()) {   //Draw loop
      can.sleep();
      s.move(can);  //And draws it
    }
  }
}

int main(int argc, char * argv[]) {
  int nthreads = (argc > 1) ? atoi(argv[1]) : 16;  //Number of threads
  clamp(nthreads,1,16);                            //Max number of threads is 16
  Canvas c(-1, -1, 885, 230, "Sea Urchins!", BLACK, FRAME * 2);
  c.run(seaUrchinFunction, nthreads);
}

