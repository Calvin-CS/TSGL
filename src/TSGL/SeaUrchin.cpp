/*
 * SeaUrchin.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: cpd5
 */

#include "SeaUrchin.h"

SeaUrchin::SeaUrchin(Canvas& can, int threadId, int colorScheme) {
  myOldX = myOldY = 50;
  myOldX += (threadId % 8) * 110;
  myOldY += (threadId / 8) * 110;
  myNewX = myNewY = 0;
  if(colorScheme == 0) {  //0 = all same color, 1 = all different color
  myColor = RED;
  } else if(colorScheme == 1) {
    myColor = Colors::highContrastColor(threadId);
  }
}

void SeaUrchin::draw(Canvas& can) {
  float delta = (2.0f / MY_SPOKES) * PI;
 // can.drawRectangle(myOldX - 50, myOldY - 50, myNewX + 50, myNewY + 50, can.getBackgroundColor());
  for(int j = 0; j < MY_SPOKES; ++j) {
    myNewX = myOldX + 50 * cos(j * delta + can.getReps());
    myNewY = myOldY + 50 * sin(j * delta + can.getReps());
    can.drawLine(myOldX, myOldY, myNewX, myNewY, myColor);
  }
}

SeaUrchin::~SeaUrchin() {

}

