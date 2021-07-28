/*
 * VisualTaskQueue.h provides a visual reprsentation of a generic parallel task queue
 */

#ifndef VISUALTASKQUEUE_H_
#define VISUALTASKQUEUE_H_

#include <cmath>
#include <omp.h>

#include "Canvas.h"
#include "tsgl.h"

namespace tsgl {

/*! \brief Enum for states of elements in the VisualTaskQueue
 *  \details VQState is an enum for the valid states of an element in the VisualTaskQueue.
 *  \details <code>RUNNING</code> specifies an element is currently being worked on by some thread.
 *  \details <code>FINISHED</code> specifies an element has been completed by some thread
 */
enum VQState {
  RUNNING = 0,
  FINISHED = 1
};

/*! \class VisualTaskQueue
 *  \brief Provides a a visualization tool for a parallel queue of elements.
 *  \details VisualTaskQueue is a self-contained Canvas intended to help visualize the progress of a parallel queue
 *    of taks or elements.
 *  \details Given a maximum queue length of <code>N</code> elements, a VisualTaskQueue will display <code>N</code>
 *    rectangles on its internal Canvas. These rectangles can be in one of three states: cleared, running, or
 *    finished.  Cleared elements are always displayed in white, whereas running elements are displayed in a
 *    darker shade of the current thread's color, and finished elements are displayed in a lighter shade of the
 *    color of the thread the completed the element.
 */
class VisualTaskQueue {
private:
  int space, border;
  bool showingLegend;
  Canvas *vcan, *lcan;
  int rowLength, blockSize, totalElements;
public:

  VisualTaskQueue(int elements, int sideLength = 12, float aspect = 1.0f, int spacing = 2, int borderLength = 8);

  ~VisualTaskQueue();

  void showLegend(int threads = -1);

  void update(int index, VQState state);

  void reset();

  void close();

  void sleep();
};

}

#endif /* VISUALTASKQUEUE_H_ */
