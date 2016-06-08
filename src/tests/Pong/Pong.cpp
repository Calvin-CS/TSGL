/*
 * Pong.cpp
 */

#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE //To avoid issues with fopen, http://stackoverflow.com/questions/14386/fopen-deprecated-warning
#endif

#include "Pong.h"

using namespace tsgl;

Pong::Pong(Canvas& can, int & ballSpeed, int & paddleSpeed) {
  leftPaddle = new Paddle(can, paddleSpeed);  // Create the Paddle objects and the Ball object
  rightPaddle = new Paddle(can, paddleSpeed);
  srand(time(NULL));
  //Bind the buttons
  leftPaddle->bindings(can, -1);  // W & S keys
  rightPaddle->bindings(can, 1);  // Up and Down arrow keys
  pongBall = new Ball(can, ballSpeed);
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
    // Handle ball paddle collisions
    if (pongBall->getX() < 32 && pongBall->getX() > 0 && pongBall->getY() > leftPaddle->getY() - 8 && pongBall->getY() < leftPaddle->getY() + 72) {
      pongBall->invert(1);
    } else if (pongBall->getX() > can.getWindowWidth() - 32 && pongBall->getX() < can.getWindowWidth() && pongBall->getY() > rightPaddle->getY() - 8 &&  pongBall->getY() < rightPaddle->getY() + 72) {
      pongBall->invert(1);
    }
    // Move the paddles if necessary
    leftPaddle->move();
    rightPaddle->move();
    can.clear();
    // Draw the paddles and ball
    leftPaddle->draw(can, -1);
    rightPaddle->draw(can, 1);
    pongBall->draw(can);
    // Draw Scores
    int cww = can.getWindowWidth() / 2;
    for (int i = 0; i < leftPaddle->getPoints(); i++) {
      int x = cww - 64 - 4 * i;
      can.drawRectangle(x, 16, x+2, 24, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
    }
    for (int i = 0; i < rightPaddle->getPoints(); i++) {
      int x = cww + 64 + 4 * i;
      can.drawRectangle(x, 16, x + 2, 24, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
    }
  }
}
