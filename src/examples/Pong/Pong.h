/*
 * Pong.h
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
 *   ball collisions with the paddles and the boundaries).
 * \see Paddle class, Ball class.
 */
class Pong {
public:

    Pong(Canvas& can, int & ballSpeed, int & paddleSpeed);

    void draw(Canvas& can);

    ~Pong();

private:
    Paddle *leftPaddle, *rightPaddle;
    Ball *pongBall;
    Text *leftScore, *rightScore;
};

#endif /* PONG_H_ */
