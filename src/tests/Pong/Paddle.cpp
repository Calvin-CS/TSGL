/*
 * Paddle.cpp
 */

#include "Paddle.h"

 /*!
  * \brief Explicitly constructs a Paddle object.
  * \details Explicit constructor for a Paddle object.
  * \param can Reference to the Canvas to have the Paddle object on.
  * \param speed Reference to the speed of the Paddle object.
  * \return The constructed Paddle object.
  */
Paddle::Paddle(Canvas& can, int & speed, int side) {
    mySpeed = speed;
    myDir = myPoints = 0;
    myY = - 32;
    myRect = new Rectangle(0,myY,0,24,64,0,0,0, BLACK);
    if(side == -1) {  //Left side
        myRect->setColor(BLUE);
        myRect->setCenterX(-can.getWindowWidth() / 2 + 20);
    } else if(side == 1) { //Right side
        myRect->setColor(RED);
        myRect->setCenterX(can.getWindowWidth() / 2 - 20);
    }
    can.add(myRect);
}

 /*!
  * \brief Binds the buttons.
  * \details Binds the buttons with the Canvas. In this case, the keys that move the paddle up and down.
  * \param can Reference to the Canvas to bind the keys to.
  * \param side The side that the Paddle object is on (left = -1 and the W and S keys are bound, right = 1 and the Up and Down arrow keys are bound).
  */
void Paddle::bindings(Canvas& can, int side) {
    if(side == 1) { //Right
        can.bindToButton(TSGL_UP, TSGL_PRESS, [this]() {this->myDir = 1;});
        can.bindToButton(TSGL_DOWN, TSGL_PRESS, [this]() {this->myDir = -1;});
        can.bindToButton(TSGL_UP, TSGL_RELEASE, [this]() {if (this->myDir == 1) this->myDir = 0;});
        can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [this]() {if (this->myDir == -1) this->myDir = 0;});
    } else if(side == -1) { //Left
        can.bindToButton(TSGL_W, TSGL_PRESS, [this] () {this->myDir = 1;});
        can.bindToButton(TSGL_S, TSGL_PRESS, [this] () {this->myDir = -1;});
        can.bindToButton(TSGL_W, TSGL_RELEASE, [this] () {if (this->myDir == 1) this->myDir = 0;});
        can.bindToButton(TSGL_S, TSGL_RELEASE, [this] () {if (this->myDir == -1) this->myDir = 0;});
    }
}

 /*!
  * \brief Increments the Paddle object's score in the game of Pong.
  */
void Paddle::increment() {
    ++myPoints;
}

 /*!
  * \brief Actually Moves the Paddle object up or down.
  */
void Paddle::move() {
    myY += mySpeed * myDir;
    myRect->changeYBy(mySpeed * myDir);
}

 /*!
  * \brief Accessor for the score of the Paddle object.
  * \return myPoints The current score of the Paddle object in the game of Pong.
  */
int Paddle::getPoints() const {
    return myPoints;
}

 /*!
  * \brief Accessor for the current y-coordinate of the Paddle object.
  * \return myY The y-coordinate of the Paddle object.
  */
float Paddle::getY() const {
    return myY;
}

 /*!
  * \brief Mutator for the direction of the Paddle object.
  * \details Changes the current direction of the Paddle object (up or down).
  */
void Paddle::setDir(int direction) {
    myDir = direction;
}
