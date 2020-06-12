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
  for(int i = 0; i < MY_SPOKES; i++) {
    Line * l = new Line(myOldX, myOldY, myNewX, myNewY, myColor);
    // Line * l = new Line()
    lines.push_back(l);
    can.add(l);
  }
}

void SeaUrchin::move(Canvas& can) {
  float delta = (2.0f / MY_SPOKES) * PI;
  for(int j = 0; j < MY_SPOKES; ++j) {
    myNewX = myOldX + 50 * cos(j * delta + can.getReps());
    myNewY = myOldY + 50 * sin(j * delta + can.getReps());
    lines[j]->setFirstEnd(myOldX, myOldY);
    lines[j]->setSecondEnd(myNewX, myNewY);
  }
}

SeaUrchin::~SeaUrchin() {
  for (unsigned int i = 0; i < lines.size(); i++) {
    delete lines[i];
  }
}

