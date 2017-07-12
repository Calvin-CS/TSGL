/*
 * SeaUrchin.cpp
 */

#include "SeaUrchin.h"

SeaUrchin::SeaUrchin(Canvas& can, int threadId) {
  myOldX = myOldY = 50;
  myOldX += (threadId % 8) * 110;
  myOldY += (threadId / 8) * 110;
  myNewX = myNewY = 0;
  myColor = Colors::highContrastColor(threadId);
}

void SeaUrchin::draw(Canvas& can) {
  float delta = (2.0f / MY_SPOKES) * PI;
  for(int j = 0; j < MY_SPOKES; ++j) {
    myNewX = myOldX + 50 * cos(j * delta + can.getReps());
    myNewY = myOldY + 50 * sin(j * delta + can.getReps());
    can.drawLine(myOldX, myOldY, myNewX, myNewY, myColor);
  }
}

SeaUrchin::~SeaUrchin() {

}

