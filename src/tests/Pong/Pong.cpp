/*
 * Pong.cpp
 */

#include "Pong.h"

using namespace tsgl;

 /*!
  * \brief Explicitly construct the game, Pong.
  * \details Explicit constructor for the game, Pong. It sets up the Paddle objects and the Ball object
  *   in order to play.
  * \param can Reference to the Canvas to use when playing Pong.
  * \param ballSpeed Reference to the ball speed to use in the game.
  * \param paddleSpeed Reference to the paddle speed to use in the game.
  */
Pong::Pong(Canvas& can, int & ballSpeed, int & paddleSpeed) {
  leftPaddle = new Paddle(can, paddleSpeed, -1);  // Create the Paddle objects and the Ball object
  rightPaddle = new Paddle(can, paddleSpeed, 1);
  srand(time(NULL));
  //Bind the buttons
  leftPaddle->bindings(can, -1);  // W & S keys
  rightPaddle->bindings(can, 1);  // Up and Down arrow keys
  pongBall = new Ball(can, ballSpeed);
  leftScore = new Text(L"0", can.getWindowWidth() / 2-64, 40, 32, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
  rightScore = new Text(L"0", can.getWindowWidth()/2+64, 40, 32, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
  can.add(leftScore); can.add(rightScore);
}

 /*!
  * \brief Draw the game of Pong.
  * \details Actually draws all of the necessary components in order to play Pong.
  *   This also includes any necessary button bindings in order to move the Paddle objects.
  * \param can Reference to the Canvas to draw on.
  * \see Paddle class, Ball class.
  */
void Pong::draw(Canvas& can) {
  // While the window has not been closed....
  while (can.isOpen()) {
    can.sleep();
    // Move the ball
    pongBall->move();
    // Handle ball boundary collisions
    if (pongBall->getX() > can.getWindowWidth() + 8) {
      leftPaddle->increment();   // Increment the points
      leftScore->setText( std::to_wstring(leftPaddle->getPoints()));
      pongBall->reset(can);   // Reset the ball's position
    } else if (pongBall->getX() < -8) {
      rightPaddle->increment();
      rightScore->setText( std::to_wstring(rightPaddle->getPoints()));
      pongBall->reset(can);
    } else if (pongBall->getY() > can.getWindowHeight() - 8 || pongBall->getY() < 8) pongBall->invert(0); //Invert the ball's y-coordinate changer
    // Handle ball paddle collisions
    // handle left
    if (pongBall->getX() - 8 < 32 &&
        pongBall->getX() - 8 > 16 &&
        pongBall->getY() > leftPaddle->getY() - 32&&
        pongBall->getY() < leftPaddle->getY() + 32)
    {
      pongBall->invert(1);
    } 
    // handle right
    else if (pongBall->getX() + 8 > can.getWindowWidth() - 32 &&
        pongBall->getX() + 8 < can.getWindowWidth() - 16 &&
        pongBall->getY() > rightPaddle->getY() - 32 &&
        pongBall->getY() < rightPaddle->getY() + 32)
    {
      pongBall->invert(1);
    }
    // Move the paddles if necessary
    leftPaddle->move();
    rightPaddle->move();
    can.clear();
  }
}

/**
 * \brief Destroys the Pong game object.
 */
Pong::~Pong() {
  delete pongBall;
  delete leftPaddle;
  delete rightPaddle;
  delete leftScore;
  delete rightScore;
}