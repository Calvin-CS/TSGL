#ifndef VISUALQUEUE_H_
#define VISUALQUEUE_H_

//Imports, constants....
#include <cmath>
#include <omp.h>

#include "Canvas.h"

using namespace tsgl;

/*!
 *
 */
enum VQState {
  RUNNING = 0,
  FINISHED = 1
};

/*!
 *
 */
class VisualQueue {
private:
  int space, border;
  Canvas* vcan;
  int rowLength, blockSize, totalElements;
public:

  /*!
   *
   */
  VisualQueue(int elements, int sideLength = 12, float aspect = 1.0f, int spacing = 2, int borderLength = 8);

  /*!
   *
   */
  ~VisualQueue();

  /*!
   *
   */
  void update(int index, int state);

  /*!
   *
   */
  void reset();

  /*!
   *
   */
  void close();
};

#endif /* VISUALQUEUE_H_ */
