/*
 * testPong.cpp
 *
 * Usage: ./testPong <ballSpeed> <paddleSpeed>
 */

#include "Pong/Pong.h"

using namespace tsgl;

/*!
 * \brief Let's play some Pong!
 * \details
 * - Create an instance of the Pong class which encapsulates a lot of the data needed
 *   in order to draw the game of Pong.
 * - Take in command-line arguments for the ball and paddle speed.
 * - When you create an instance of the class:
 *    - Seed the random number generator.
 *    - Create the Paddle objects and the Ball object needed in order to play Pong.
 *    - ( see the Paddle and Ball object classes for more details on how they are created ).
 *    .
 * - When you draw:
      - Bind the buttons of the Canvas to the two Paddle objects:
 *       - Bind the up arrow's on-press and on-release events to set the right
 *         Paddle object's direction to -1 (up) and 0 (static) respectively.
 *       - Bind the down arrow's on-press and on-release events to set the right
 *         Paddle object's direction to 1 (down) and 0 respectively.
 *       - Bind the W key's on-press and on-release events to set the left
 *         Paddle object's direction to -1 and 0 respectively.
 *       - Bind the S key's on-press and on-release events to set the left
 *         Paddle object's direction to 1 and 0 respectively.
 *       .
 *    - While the Canvas is open:
 *      - Sleep the internal timer until the Canvas is ready to draw.
 *      - Move the Ball object in its current direction at its current speed.
 *      - If the Ball object passed by one of the goal areas, increment the appropriate player's score,
 *        reset the Ball object's position, and send it off in a random direction and speed.
 *      - If the Ball object hits the top or bottom of the screen, bounce it.
 *      - If the Ball object hits a Paddle object, reverse its x-speed and randomly adjust its y-speed.
 *      - Move the Paddle objects according to their current directions.
 *      - Draw the Ball object and Paddle objects.
 *      - Draw the scores at the top of the screen.
 *      .
 *    .
 * .
 * \param can Reference to the Canvas being drawn to.
 * \param ballSpeed Reference to the ball speed in the game.
 * \param paddleSpeed Reference to the paddle speed in the game.
 */
void pongFunction(Canvas& can, int ballSpeed, int paddleSpeed) {
    Pong p1(can, ballSpeed, paddleSpeed); //Create the Pong object
    p1.draw(can);  //Draw the game
}

//Takes command-line arguments for the ball and paddle speed
int main(int argc, char * argv[]) {
    //Default speeds for the ball and paddles
    const int BALL_DEFAULT_SPEED = 8, PADDLE_DEFAULT_SPEED = 4;
    //Window width and height
    int w = 1.5*Canvas::getDisplayHeight();
    int h = 0.5*w;
    //Determine the ball and paddle speed from command-line arguments
    int ballSpeed = (argc > 1) ? atoi(argv[1]) : BALL_DEFAULT_SPEED;
    int paddleSpeed = (argc > 2) ? atoi(argv[2]) : PADDLE_DEFAULT_SPEED;
    //Check if valid
    if(ballSpeed > 10 || paddleSpeed > 10) {
      ballSpeed = BALL_DEFAULT_SPEED;
      paddleSpeed = PADDLE_DEFAULT_SPEED;
    }
    Canvas c(-1,-1,w,h,"Tennis for Two", FRAME);
    c.setBackgroundColor(BLACK);
    c.run(pongFunction,ballSpeed, paddleSpeed);
}
