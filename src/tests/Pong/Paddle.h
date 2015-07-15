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

using namespace tsgl;

/*!
 * class Paddle
 * \brief How can you hit a ball without a paddle?
 * \details Creates a Paddle in order to play the game, Pong.
 * \details The W and S keys move the left paddle, the Up and Down arrow keys move the right paddle in the game.
 * \details Collisions with the ball are not handled here; they are handled in the Pong class' draw() method.
 * \details The left and right paddles in the game are created from this class; they are essentially the same object.
 * However, there are side parameters in some of the methods that determine which side to place the Paddle object on and how to treat
 * the Paddle object (either as the left or the right paddle in the game). The Pong class handles how to distribute points to the Paddle objects
 * in the game (the Paddle class only has an increment() method that increments its score counter; the Pong class determines which object should
 * call that method when a point is earned).
 * \see Ball class, Pong class.
 */
class Paddle {
public:

  /*!
   * \brief Explicitly constructs a Paddle object.
   * \details Explicit constructor for a Paddle object.
   * \param can Reference to the Canvas to have the Paddle object on.
   * \return The constructed Paddle object.
   */
  Paddle(Canvas& can);

  /*!
   * \brief Binds the buttons.
   * \details Binds the buttons with the Canvas. In this case, the keys that move the paddle up and down.
   * \param can Reference to the Canvas to bind the keys to.
   * \param side The side that the Paddle object is on (left = -1 and the W and S keys are bound, right = 1 and the Up and Down arrow keys are bound).
   */
  void bindings(Canvas& can, int side);

  /*!
   * \brief Draw the Paddle object.
   * \details Actually draws the Paddle object onto the Canvas.
   * \param can Reference to the Canvas to draw on.
   * \param side The side that the Paddle object is drawn to on the Canvas (left = -1, right = 1).
   */
  void draw(Canvas& can, int side);

  /*!
   * \brief Increments the Paddle object's score in the game of Pong.
   */
  void increment();

  /*!
   * \brief Actually Moves the Paddle object up or down.
   */
  void move();

  /*!
   * \brief Accessor for the score of the Paddle object.
   * \return myPoints The current score of the Paddle object in the game of Pong.
   */
  int getPoints() const;

  /*!
   * \brief Accessor for the current y-coordinate of the Paddle object.
   * \return myY The y-coordinate of the Paddle object.
   */
  float getY() const;

  /*!
   * \brief Mutator for the direction of the Paddle object.
   * \details Changes the current direction of the Paddle object (up or down).
   */
  void setDir(int direction);

private:
  int myDir;  //-1 = up, 1 = down, 0 = stationary
  int myPoints;  //Score
  float myY; //y-coordinate for Paddle
};

#endif /* PADDLE_H_ */
