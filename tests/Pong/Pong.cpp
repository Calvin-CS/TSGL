/*
 * Pong.cpp
 */

#include "Pong.h"

using namespace tsgl;

Pong::Pong(Canvas& can, int & ballSpeed, int & paddleSpeed) {
  leftPaddle = new Paddle(can, paddleSpeed, -1);  // Create the Paddle objects and the Ball object
  rightPaddle = new Paddle(can, paddleSpeed, 1);
  srand(time(NULL));
  //Bind the buttons
  leftPaddle->bindings(can, -1);  // W & S keys
  rightPaddle->bindings(can, 1);  // Up and Down arrow keys
  pongBall = new Ball(can, ballSpeed);
  leftScore = new Text("0", can.getWindowWidth() / 2-64, 20, 8, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
  rightScore = new Text("0", can.getWindowWidth()/2+64, 20, 8, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
  can.add(leftScore); can.add(rightScore);
}

void Pong::draw(Canvas& can) {
  // While the window has not been closed....
  while (can.isOpen()) {
    can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
    // Move the ball
    pongBall->move();
    // Handle ball boundary collisions
    if (pongBall->getX() > can.getWindowWidth() + 8) {
      leftPaddle->increment();   // Increment the points
      pongBall->reset(can);   // Reset the ball's position
    } else if (pongBall->getX() < -8) {
      rightPaddle->increment();
      pongBall->reset(can);
    } else if (pongBall->getY() > can.getWindowHeight() - 8 || pongBall->getY() < 8) pongBall->invert(0); //Invert the ball's y-coordinate changer
    // Handle ball paddle collisions TODO: correct these so ball bounces off any part of paddle.
    if (pongBall->getX() < 32 &&
        pongBall->getX() > -16 &&
        pongBall->getY() > leftPaddle->getY() &&
        pongBall->getY() < leftPaddle->getY() + 64)
    {
      pongBall->invert(1);
    } else if (pongBall->getX() > can.getWindowWidth() - 32 &&
        pongBall->getX() < can.getWindowWidth() + 16 &&
        pongBall->getY() > rightPaddle->getY() &&
        pongBall->getY() < rightPaddle->getY() + 64)
    {
      pongBall->invert(1);
    }
    // Move the paddles if necessary
    leftPaddle->move();
    rightPaddle->move();
    // Update Scores
    leftScore->setString( to_string(leftPaddle->getPoints()));
    rightScore->setString( to_string(rightPaddle->getPoints()));
  }
}

Pong::~Pong() {
  delete pongBall;
  delete leftPaddle, rightPaddle;
  delete leftScore, rightScore;
}
