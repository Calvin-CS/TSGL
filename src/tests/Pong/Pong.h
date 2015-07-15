/*
 * Pong.h
 *
 *  Created on: May 29, 2015
 *      Author: cpd5
 */

#ifndef PONG_H_
#define PONG_H_

#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>
#include "Ball.h"
#include "Paddle.h"
#include "Util.h"

using namespace tsgl;

/*!
 * \class Pong
 * \brief An old-school classic!
 * \details Draw the interactive game of Pong. The two paddles are objects and the ball is also an object.
 * \details The constructor constructs the two paddle objects and the ball object.
 * \details Use the W and S keys in order to move the left paddle, the Up and Down arrow keys in order to move the right paddle.
 * \details Everything else is handled in the draw() method (button bindings, score keeping, drawing the objects and the game itself,
 * ball collisions with the paddles and the boundaries).
 * \see Paddle class, Ball class.
 */
class Pong {
public:

  /*!
   * \brief Explicitly construct the game, Pong.
   * \details Explicit constructor for the game, Pong. It sets up the Paddle objects and the Ball object
   * in order to play.
   * \param can Reference to the Canvas to use when playing Pong.
   */
  Pong(Canvas& can);

  /*!
   * \brief Draw the game of Pong.
   * \details Actually draws all of the necessary components in order to play Pong.
   * That also includes any necessary button bindings in order to move the Paddle objects.
   * \param can Reference to the Canvas to draw on.
   * \see Paddle class, Ball class.
   */
  void draw(Canvas& can);

private:
  Paddle * leftPaddle;
  Paddle * rightPaddle;
  Ball * pongBall;
};

#endif /* PONG_H_ */
