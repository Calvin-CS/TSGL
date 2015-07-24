/*
 * VisualTaskQueue.h provides a visual reprsentation of a generic parallel task queue
 */

#ifndef VISUALTASKQUEUE_H_
#define VISUALTASKQUEUE_H_

#include <cmath>
#include <omp.h>

#include "Canvas.h"

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

  /*!
   * \brief Default VisualTaskQueue constructor method.
   * \details This is the default constructor for the VisualTaskQueue class.
   *   \param elements The maximum number of elements to be drawn on the VisualTaskQueue. Setting this to higher than
   *     the actual number of elements may result in some unused, empty rectangle. Setting this to lower than
   *     the actual number of elements may result in some rectangles being drawn off the VisualTaskQueue Canvas.
   *   \param sideLength The side length in pixels of the task rectangles to be drawn on the VisualTaskQueue Canvas.
   *   \param aspec The approximate aspect ratio of height/width for the VisualTaskQueue Canvas.
   *   \param spacing The space in pixels between the rectangles representing elements in the VisualTaskQueue.
   *   \param borderLength The space in pixels between the outer VisualTaskQueue rectangles and the border of the
   *     VisualTaskQueue Canvas.
   * \return A new VisualTaskQueue with the specified maximum number of elements, rectangle side length,
   *   approximate aspect ratio, spacing between rectangles, and spacing around the borders.
   */
  VisualTaskQueue(int elements, int sideLength = 12, float aspect = 1.0f, int spacing = 2, int borderLength = 8);

  /*!
   * \brief VisualTaskQueue destructor method.
   * \details This is the destructor for the VisualTaskQueue class.
   * \details Frees up memory that was allocated to a VisualTaskQueue instance.
   */
  ~VisualTaskQueue();

  /*!
   * \brief Shows a key/legend for the VisualTaskQueue.
   * \details This function opens up a separate Canvas displaying a legend for the VisualTaskQueue, showing which
   *   colors correspond to which threads.
   *   \param threads The number of threads the VisualTaskQueue is using.
   */
  void showLegend(int threads = -1);

  /*!
   * \brief Updates the state of the VisualTaskQueue.
   * \details This function updates an element of the VisualTaskQueue with a new state.
   *   \param index The index of the element to update.
   *   \param state The new state to put the element in. Must be one of RUNNING or FINISHED.
   */
  void update(int index, VQState state);

  /*!
   * \brief Resets all of the elements in the VisualTaskQueue.
   * \details This function tells VisualTaskQueue to clear the state information of all of the elements.
   */
  void reset();

  /*!
   * \brief Closes the visual queue.
   * \details This function closes and destroys the internal Canvas created by the VisualTaskQueue.
   * \warning <b> Do not attempt to reset() or update() the VisualTaskQueue after closing it.</b>
   */
  void close();
};

}

#endif /* VISUALTASKQUEUE_H_ */
