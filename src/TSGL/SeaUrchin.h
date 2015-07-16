/*
 * SeaUrchin.h
 *
 *  Created on: Jul 16, 2015
 *      Author: cpd5
 */

#ifndef SEAURCHIN_H_
#define SEAURCHIN_H_

#include <tsgl.h>

using namespace tsgl;

class SeaUrchin {
public:
  SeaUrchin(Canvas& can, int threadId, int colorScheme);  //Default constructor
  void draw(Canvas& can);
  virtual ~SeaUrchin();

private:
  static const int MY_SPOKES = 16;
  int myOldX, myOldY, myNewX, myNewY;
  ColorHSV myColor;
};

#endif /* SEAURCHIN_H_ */
