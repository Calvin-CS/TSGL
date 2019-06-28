/*
 * Firework.cpp
 */ 

#include "Firework.h"

/*!
 * \brief Explicit constructor for a Firework.
 * \details Shoots white dots out in random directions from a point.
 * \param can The Canvas to which the dot is to be drawn.
 * \param x The x coordinate of the dot emission point.
 * \param y The y coordinate of the dot emission point.
 */
Firework::Firework(Canvas& can, int x, int y) {
  dead = false;
  myCan = &can;
  myX = x;
  myY = y;
  for (int i = 0; i < 10; ++i) {
    myDots[i] = new Dot(can, myX,myY,(rand() % 10000)/10000.0f,(rand() % 10000)/10000.0f * 2 * PI, 0.99f);
  }
}

/*!
 * \brief Destructor for Firework.
 * \details Deallocates the myDots array.
 */
Firework::~Firework() {
  for (int i = 0; i < 10; delete myDots[i++]);
}

/*!
 * \brief Makes the firework take a step.
 * \details Makes each dot in myDots take a step.
 */
void Firework::step() {
  if (!dead) {
    bool allDead = true;
    for (int i = 0; i < 10; ++i) {
      myDots[i]->step();
      if (!myDots[i]->isDead())
        allDead = false;
    }
    dead = allDead;
  }
}

/*!
 * \brief Accessor for if the Firework is dead.
 * \details Returns the dead variable.
 * \return dead A bool stating whether the Firework is dead.
 */
bool Firework::isDead() { 
  return dead; 
}