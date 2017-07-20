/*
 * SeaUrchin.h
 */

#ifndef SEAURCHIN_H_
#define SEAURCHIN_H_

#include <tsgl.h>
#include <vector>

using namespace tsgl;

/*!
 * \class SeaUrchin
 * \brief Who doesn't love sea urchins?
 * \details Draws a sea urchin onto the Canvas.
 * \details Used as an example of what it means to put a process on a thread.
 * \details The SeaUrchin objects are created in a parallel block, and each thread gets one SeaUrchin.
 * \details Then, the thread draws the SeaUrchin onto the Canvas.
 * \details Each SeaUrchin object has its own color and plot data based off of the thread's id number.
 * \details SeaUrchin objects are drawn similar to the line fan in testLineFan.
 * \see testLineFan.
 */
class SeaUrchin {
public:

  /*!
   * \brief Explicitly construct a SeaUrchin object.
   * \details Explicit constructor for a SeaUrchin object and adds it to the Canvas.
   * \param can Reference to the Canvas to draw to.
   * \param threadId The id of the thread that is currently creating a SeaUrchin object.
   * \return The constructed SeaUrchin object.
   */
  SeaUrchin(Canvas& can, int threadId);  //Default constructor

  /*!
   * \brief Move the SeaUrchin.
   * \details Moves the SeaUrchin by moving its lines.
   * \param can Reference to the Canvas object drawn to. (Used to coordinate timing.)
   */
  void move(Canvas& can);

  /*!
   * \brief Destroy a SeaUrchin.
   * \details Destructor for a SeaUrchin object.
   */
  virtual ~SeaUrchin();

private:
  static const int MY_SPOKES = 8; //Number of spokes for all SeaUrchins
  int myOldX, myOldY, myNewX, myNewY; //Coordinates for the spokes
  ColorFloat myColor; //Color of SeaUrchin
  std::vector<Line*> lines; //Lines on Canvas for SeaUrchin
};

#endif /* SEAURCHIN_H_ */
