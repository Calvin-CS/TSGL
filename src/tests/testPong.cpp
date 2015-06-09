/*
 * testPong.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include "Pong.cpp"

using namespace tsgl;

/*!
 * \brief Plays a nice game of Pong!
 * \details
 * - Store the Canvas' dimensions for ease of use, and declare some useful variables.
 * - Seed the random number generator.
 * - Set the intial position of the paddles and the ball.
 * - Randomize the initial direction of the ball until it's at a reasonable speed.
 * - Set up the internal timer of the Canvas to expire every \b FRAME seconds.
 * - Bind the up arrow's on-press and on-release events to set the right
 * paddle's direction to -1 (up) and 0 (static) respectively.
 * - Bind the down arrow's on-press and on-release events to set the right
 * paddle's direction to 1 (down) and 0 respectively.
 * - Bind the W key's on-press and on-release events to set the left
 * paddle's direction to -1 and 0 respectively.
 * - Bind the S key's on-press and on-release events to set the left
 * paddle's direction to 1 and 0 respectively.
 * - While the Canvas is open:
 *   - Sleep the internal timer until the Canvas is ready to draw.
 *   - Move the ball in its current direction at its current speed.
 *   - If the ball passed by one of the goal areas, increment the appropriate player's score,
 *   reset the ball's position, and send it off in a random direction and speed.
 *   - If the ball hits the top or bottom of the screen, bounce it.
 *   - If the ball hits a paddle, reverse its x-speed and randomly adjust its y-speed.
 *   - Move the paddles according to their current directions.
 *   - Draw the balls and paddles.
 *   - Draw the scores at the top of the screen.
 *   .
 * .
 * \param can, Reference to the Canvas being drawn to
 */
void pongFunction(Canvas& can) {
    Pong p1(can);
    p1.bindings(can);
    p1.draw(can);
}

int main() {
    Canvas c25(0,0,1600,600,"", FRAME);
    c25.setBackgroundColor(BLACK);
    c25.start();
    pongFunction(c25);
    c25.wait();
}
