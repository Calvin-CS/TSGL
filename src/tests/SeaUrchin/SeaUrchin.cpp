/*
 * SeaUrchin.cpp
 */

#include "SeaUrchin.h"

SeaUrchin::SeaUrchin(Canvas& can, int threadId) {
  myX = -can.getWindowWidth() / 2 + 60;
  myY = -can.getWindowHeight() / 2 + 60;
  myX += (threadId % 8) * 110;
  myY += (threadId / 8) * 110;
  float delta = 180 / MY_SPOKES;
  myColor = Colors::highContrastColor(threadId);
  for(int i = 0; i < MY_SPOKES; i++) {
    Line * l = new Line(myX, myY, 0, 100, i * delta, 0, 0, myColor);
    lines.push_back(l);
    can.add(l);
  }
}

void SeaUrchin::move(Canvas& can) {
  float delta = 180 / MY_SPOKES;
  for(int j = 0; j < MY_SPOKES; ++j) {
    lines[j]->setYaw(j * delta + can.getReps() * 180 / PI);
  }
}

SeaUrchin::~SeaUrchin() {
  for (unsigned int i = 0; i < lines.size(); i++) {
    delete lines[i];
  }
}

