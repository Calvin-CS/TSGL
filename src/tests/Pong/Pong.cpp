#include "Pong.h"

using namespace tsgl;

Pong::Pong(Canvas& can) {
  srand(time(NULL));
  leftPaddle = new Paddle(can);
  rightPaddle = new Paddle(can);
  pongBall = new Ball(can);
}

void Pong::draw(Canvas& can) {
  //Bind the buttons
  leftPaddle->bindings(can, -1);
  rightPaddle->bindings(can, 1);

  // Check to see if the window has been closed
  while (can.getIsOpen()) {
    can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class

    // Move the ball
    pongBall->move();

    // Handle ball boundary collisions

    if (pongBall->getX() > can.getWindowWidth() + 8) {
      leftPaddle->increment();
      pongBall->reset(can);

    } else if (pongBall->getX() < -8) {
      rightPaddle->increment();
      pongBall->reset(can);

    } else if (pongBall->getY() > can.getWindowHeight() - 8 || pongBall->getY() < 8) pongBall->invert(0);

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

    //draw the paddles and ball
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

