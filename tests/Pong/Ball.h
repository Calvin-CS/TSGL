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

  /*!
   * \brief Explicitly construct the Ball object.
   * \details Explicit constructor for the Ball object.
   * \param can Reference to the Canvas that will have the Ball object.
   * \param speed Reference to the speed of the Ball object.
   * \return The constructed Ball object.
   */
  Ball(Canvas& can, int & speed);

  /*!
   * \brief Accessor for the Ball object's current x-coordinate.
   * \return myX The x-coordinate of the Ball object.
   */
  float getX() const;

  /*!
   * \brief Accessor for the Ball object's current y-coordinate.
   * \return myY The y-coordinate of the Ball object.
   */
  float getY() const;

  /*!
   * \brief Invert the Ball's direction.
   * \details The Ball's direction must be inverted whenever it collides with a Paddle object or
   * when it touches one of the boundary walls in the game of Pong.
   * \param choice Determines which coordinate value to invert (y-coordinate = 0, x-coordinate = 1).
   * \see Paddle class, Pong class.
   */
  void invert(int choice);

  /*!
   * \brief Move the Ball object.
   * \details Actually moves the Ball object around.
   */
  void move();

  /*!
   * \brief Reset the Ball object's position.
   * \details After a point is earned in the game of Pong, the Ball object's position must be reset back
   * to the middle of the Canvas and its direction must be randomized again.
   * \param can Reference to the Canvas object that has the Ball object.
   */
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
