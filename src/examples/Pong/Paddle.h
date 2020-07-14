/*
 * Paddle.h
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
    Paddle(Canvas& can, int & speed, int side);

    void bindings(Canvas& can, int side);

    void draw(Canvas& can, int side);

    void increment();

    void move();

    int getPoints() const;

    float getY() const;

    void setDir(int direction);

    /**
     * \brief Destroys the Paddle object.
     */
    ~Paddle() { delete myRect; }

private:
    int myDir;  //-1 = up, 1 = down, 0 = stationary
    int myPoints;  //Score
    int mySpeed;  //Speed
    float myY; //y-coordinate for Paddle
    Rectangle * myRect;
};

#endif /* PADDLE_H_ */
