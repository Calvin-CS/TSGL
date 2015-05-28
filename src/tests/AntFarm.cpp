/*
 * AntFarm.cpp
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#include "AntFarm.h"

AntFarm::AntFarm(int w, int h, int s, Canvas* c) {
  width = w;
  height = h;
  std::cout << width << "," << height << std::endl;
  cap = s;
  size = 0;
  ants = new LangtonAnt*[cap];
  filled = new bool[w * h]();  // Create an empty bitmap for the window
  can = c;
}
AntFarm::~AntFarm() {
  delete filled;
}
void AntFarm::addAnt(int x, int y, int r, int g, int b, int d) {
  if (size < cap)
    ants[size++] = new LangtonAnt(x, y, r, g, b, d, this);
}
void AntFarm::moveAnts() {
  for (int j = 0; j < size; j++) {
      if (filled[ants[j]->myX + width * ants[j]->myY]) {
          ants[j]->myDir = (ants[j]->myDir + 1) % 4;
          can->drawPoint(ants[j]->myX, ants[j]->myY, ColorInt(MAX_COLOR / 2, MAX_COLOR / 2, MAX_COLOR / 2, 16));
      } else {
          ants[j]->myDir = (ants[j]->myDir + 3) % 4;
          can->drawPoint(ants[j]->myX, ants[j]->myY, ColorInt(ants[j]->myRed, ants[j]->myGreen, ants[j]->myBlue, 16));
      }
  }
  for (int j = 0; j < size; j++) {
      filled[ants[j]->myX + width * ants[j]->myY] ^= true;
      ants[j]->move();
  }
}
