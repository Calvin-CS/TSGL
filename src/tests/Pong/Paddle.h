/*
 * Paddle.h
 *
 *  Created on: Jul 14, 2015
 *      Author: cpd5
 */

#ifndef PADDLE_H_
#define PADDLE_H_

#include <tsgl.h>
#include "Ball.h"

/*!
 * class Paddle
 * \brief Let's play some Pong!
 * \details Creates a Paddle in order to play the game, Pong.
 * \details
 * \details
 * \details
 */
class Paddle {
public:
  /*!
   * \brief Explicitly constructs a Paddle object.
   * \details Explicit constructor for a Paddle object.
   * \param can Reference to the Canvas
   * \return The constructed Paddle object.
   */
  Paddle(Canvas& can);

  /*!
   * \brief Binds the buttons.
   * \details Binds the buttons with the Canvas.
   */
  void bindings(Canvas& can, int side);

  /*!
   * \brief
   */
  void draw(Canvas& can, int side);

  /*!
   * \brief
   */
  void increment();

  /*!
   * \brief
   */
  void move();

  /*!
   * \brief
   */
  int getPoints() const;

  /*!
   * \brief
   */
  float getY() const;

  /*!
   * \brief
   */
  void setDir(int direction);
private:
  int myDir;  //-1 = up, 1 = down, 0 = stationary
  int myPoints;  //Score
  float myY; //y-coordinate for Paddle
};

#endif /* PADDLE_H_ */
