/*
 * SeaUrchin.h
 *
 *  Created on: Jul 16, 2015
 *      Author: cpd5
 */

#ifndef SEAURCHIN_H_
#define SEAURCHIN_H_

#include <tsgl.h>

using namespace tsgl;

class SeaUrchin {
public:

  /*!
   * \brief Explicitly construct a SeaUrchin object.
   * \details Explicit constructor for a SeaUrchin object.
   * \param can Reference to the Canvas to draw to.
   * \param threadId The thread that is currently creating a SeaUrchin object.
   * \param colorScheme 0 equals all of the SeaUrchin objects created are the same color, 1 equals the inverse.
   * \return The constructed SeaUrchin object.
   */
  SeaUrchin(Canvas& can, int threadId, int colorScheme);  //Default constructor

  /*!
   * \brief Draw the SeaUrchin.
   * \details Actually draws the SeaUrchin object onto the Canvas.
   * \param can Reference to the Canvas object to draw to.
   */
  void draw(Canvas& can);

  /*!
   * \brief Destroy a SeaUrchin.
   * \details Destructor for a SeaUrchin object.
   * \note This function does absolutely nothing.
   */
  virtual ~SeaUrchin();

private:
  static const int MY_SPOKES = 16;
  int myOldX, myOldY, myNewX, myNewY;
  ColorHSV myColor;
};

#endif /* SEAURCHIN_H_ */
