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
  myY = can.getWindowHeight() / 2 - 32;
  myRect = new Rectangle(0,0,24,64, BLACK);
  if(side == -1) {  //Left side
    myRect->setColor(BLUE);
    myRect->setCenter(20, myY);
  } else if(side == 1) { //Right side
    myRect->setColor(RED);
    myRect->setCenter(can.getWindowWidth() - 20, myY);
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
    can.bindToButton(TSGL_UP, TSGL_PRESS, [this]() {this->myDir = -1;});
    can.bindToButton(TSGL_DOWN, TSGL_PRESS, [this]() {this->myDir = 1;});
    can.bindToButton(TSGL_UP, TSGL_RELEASE, [this]() {if (this->myDir == -1) this->myDir = 0;});
    can.bindToButton(TSGL_DOWN, TSGL_RELEASE, [this]() {if (this->myDir == 1) this->myDir = 0;});
  } else if(side == -1) { //Left
    can.bindToButton(TSGL_W, TSGL_PRESS, [this] () {this->myDir = -1;});
    can.bindToButton(TSGL_S, TSGL_PRESS, [this] () {this->myDir = 1;});
    can.bindToButton(TSGL_W, TSGL_RELEASE, [this] () {if (this->myDir == -1) this->myDir = 0;});
    can.bindToButton(TSGL_S, TSGL_RELEASE, [this] () {if (this->myDir == 1) this->myDir = 0;});
  }
}

 /*!
  * \brief Draw the Paddle object.
  * \details Actually draws the Paddle object onto the Canvas.
  * \param can Reference to the Canvas to draw on.
  * \param side The side that the Paddle object is drawn to on the Canvas (left = -1, right = 1).
  */
void Paddle::draw(Canvas& can, int side) {
  if(side == -1) {  //Left side
    // ColorFloat color[4];
    // for (unsigned i = 0; i < 4; ++i) {
    //   color[i] = Colors::randomColor(1.0f);
    // }
    can.drawRectangle(8, myY, 32, myY + 64, ColorFloat(0.0f, 0.0f, 1.0f, 1.0f) /* color */, false);
  } else if(side == 1) { //Right side
    can.drawRectangle(can.getWindowWidth() - 24 - 8, myY, can.getWindowWidth() - 8, myY + 64, ColorFloat(1.0f, 0.0f, 0.0f, 1.0f), true);
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
  myRect->moveShapeBy(0, mySpeed * myDir);
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
