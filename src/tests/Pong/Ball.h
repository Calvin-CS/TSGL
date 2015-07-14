/*
 * Ball.h
 *
 *  Created on: Jul 14, 2015
 *      Author: cpd5
 */

#ifndef BALL_H_
#define BALL_H_

#include <tsgl.h>

class Ball {
 public:
  /*!
   * \brief
   */
  Ball(Canvas& can);

  /*!
   * \brief
   */
  void draw(Canvas& can);

  /*!
   * \brief
   */
  float getX() const;

  /*!
   * \brief
   */
  float getY() const;

  /*!
   * \brief
   */
  void invert(int choice);

  /*!
   * \brief
   */
  void move();

  /*!
   * \brief
   */
  void reset(Canvas& can);

private:
  float myX, myY, myXX, myYY, mySpeed, myDir;
  float randfloat(int divisor = 10000);

};

#endif /* BALL_H_ */
