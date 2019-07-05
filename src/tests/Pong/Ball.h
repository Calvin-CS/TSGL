/*
 * Ball.h
 */

#ifndef BALL_H_
#define BALL_H_

#include <tsgl.h>

using namespace tsgl;

/*!
 * class Ball
 * \brief We can't play Pong without a Ball!
 * \details Creates the ball needed in order to play Pong.
 * \details The constructor determines the speed and direction of the ball.
 * \details The draw() method draws the ball onto the Canvas object.
 * \details The ball's position is always reset after a point has been earned in the game.
 * \details Collisions with the paddle or the boundaries are not handled here; they are handled in the Pong class' draw() method.
 * \see Paddle class, Pong class.
 */
class Ball {
 public:
  Ball(Canvas& can, int & speed);

  float getX() const;

  float getY() const;

  void invert(int choice);

  void move();

  void reset(Canvas& can);

  /*!
   * \brief Destroys the Ball object.
   */
  virtual ~Ball() { delete myCircle; }

private:
  float myX, myY, myXX, myYY, mySpeed, myDir;
  float randfloat(int divisor = 10000);
  Circle* myCircle;
};

#endif /* BALL_H_ */
