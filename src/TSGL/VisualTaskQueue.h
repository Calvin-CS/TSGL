#ifndef VISUALTASKQUEUE_H_
#define VISUALTASKQUEUE_H_

#include <cmath>
#include <omp.h>

#include "Canvas.h"

namespace tsgl {

/*! \brief Enum for states of elements in the VisualQueue
 *  \details VQState is an enum for the valid states of an element in the VisualQueue.
 *  \details <code>RUNNING</code> specifies an element is currently being worked on by a thread.
 *  \details <code>FINISHED</code> specifies an element has been completed by some thread
 */
enum VQState {
  RUNNING = 0,
  FINISHED = 1
};

/*! \class VisualQueue
 *  \brief Provides a a visualization tool for a parallel queue of elements.
 *  \details VisualQueue is a self-contained Canvas intended to help visualize the progress of a parallel queue
 *    of taks or elements.
 *  \details Given a maximum queue length of <code>N</code> elements, a VisualQueue will display <code>N</code>
 *    rectangles on its internal Canvas. These rectangles can be in one of three states: cleared, running, or
 *    finished.  Cleared elements are always displayed in white, whereas running elements are displayed in a
 *    darker shade of the current thread's color, and finished elements are displayed in a lighter shade of the
 *    color of the thread the completed the element.
 */
class VisualTaskQueue {
private:
  int space, border;
  Canvas* vcan;
  int rowLength, blockSize, totalElements;
public:
  /*!
   * \brief Default VisualQueue constructor method.
   * \details This is the default constructor for the VisualQueue class.
   *   \param elements The maximum number of elements to be drawn on the VisualQueue. Setting this to higher than
   *     the actual number of elements may result in some unused, empty rectangle. Setting this to lower than
   *     the actual number of elements may result in some rectangles being drawn off the VisualQueue's Canvas.
   *   \param sideLength The side length in pixels of the task rectangles to be drawn on the VisualQueue's Canvas.
   *   \param aspec The approximate aspect ratio of height/width for the VisualQueue's Canvas.
   *   \param spacing The space in pixels between the rectangles representing elements in the VisualQueue.
   *   \param borderLength The space in pixels between the outer VisualQueue rectangles and the border of the
   *     VisualQueue's Canvas.
   * \return A new VisualQueue with the specified maximum number of elements, rectangle side length,
   *   approximate aspect ratio, spacing between rectangles, and spacing around the borders.
   */
  VisualTaskQueue(int elements, int sideLength = 12, float aspect = 1.0f, int spacing = 2, int borderLength = 8);

  /*!
   * \brief VisualQueue destructor method.
   * \details This is the destructor for the VisualQueue class.
   * \details Frees up memory that was allocated to a VisualQueue instance.
   */
  ~VisualTaskQueue();

  /*!
   * \brief Updates the state of the VisualQueue.
   * \details This function updates an element of the VisualQueue with a new state.
   *   \param index The index of the element to update.
   *   \param state The new state to put the element in.
   */
  void update(int index, VQState state);

  /*!
   * \brief Resets all of the elements in the VisualQueue.
   * \details This function tells VisualQueue to clear the state information of all of the elements.
   */
  void reset();

  /*!
   * \brief Closes the visual queue.
   * \details This function closes and destroys the internal Canvas created by the VisualQueue.
   * \warning <b> Do not attempt to reset() or update() the VisualQueue after closing it.</b>
   */
  void close();
};

}

#endif /* VISUALTASKQUEUE_H_ */
