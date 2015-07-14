#include "Pong.h"
#include "Ball.h"
#include "Paddle.h"

using namespace tsgl;


void Pong::draw(Canvas& can) {
  srand(time(NULL));
  Paddle leftPaddle(can);  //Create the paddles and the balls
  Paddle rightPaddle(can);
  Ball pongBall(can);
  //Bind the buttons
  leftPaddle.bindings(can, -1);
  rightPaddle.bindings(can, 1);

  // Check to see if the window has been closed
  while (can.getIsOpen()) {
    can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class

    // Move the ball
    pongBall.move();
    // Handle ball boundary collisions

    if (myBallX > can.getWindowWidth() + 8) {
      myLeftPoints++;
      myBallX = can.getWindowWidth() / 2 - 8;
      myBallY = can.getWindowHeight() / 2 - 8;
      do {

        //Reset the ball
        myDir = randfloat(1000) * 2 * 3.14159f;  //Looks like it resets the ball...
        myXX = mySpeed * cos(myDir);
        myYY = mySpeed * sin(myDir);
      } while (myXX > -4 && myXX < 4);

    } else if (myBallX < -8) {
      myRightPoints++;
      myBallX = can.getWindowWidth() / 2 - 8;
      myBallY = can.getWindowHeight() / 2 - 8;
      do {

        //Reset the ball
        myDir = randfloat(1000) * 2 * 3.14159f;   //Looks like it resets the ball...
        myXX = mySpeed * cos(myDir);
        myYY = mySpeed * sin(myDir);
      } while (myXX > -4 && myXX < 4);

    } else if (myBallY > can.getWindowHeight() - 8 || myBallY < 8) myYY = -myYY;

    // Handle ball paddle collisions
    if (myBallX < 32 && myBallX > 0 && myBallY > myLeftY - 8 && myBallY < myLeftY + 72) {
      myXX = -myXX;
      myYY += randfloat(1000) * 2 - 1;
    } else if (myBallX > can.getWindowWidth() - 32 && myBallX < can.getWindowWidth() && myBallY > myRightY - 8 &&  myBallY < myRightY + 72) {
      myXX = -myXX;
      myYY += randfloat(1000) * 2 - 1;
    }
    // Move the paddles if necessary
    leftPaddle.move();
    rightPaddle.move();
    can.clear();

    //draw the paddles and ball
    leftPaddle.draw(can, -1);
    rightPaddle.draw(can, 1);
    pongBall.draw(can);

    // Draw Scores
    int cww = can.getWindowWidth() / 2;
    for (int i = 0; i < leftPaddle.getPoints(); i++) {
      int x = cww - 64 - 4 * i;
      can.drawRectangle(x, 16, x+2, 24, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f));
    }
    for (int i = 0; i < rightPaddle.getPoints(); i++) {
      int x = cww + 64 + 4 * i;
      can.drawRectangle(x, 16, x + 2, 24, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f));
    }
  }
}

float Pong::randfloat(int divisor) {
    return (rand() % divisor) / (float) divisor;
}
