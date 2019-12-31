/*
 * AntFarm.cpp
 */

#include "AntFarm.h"

AntFarm::AntFarm(int w, int h, int s, Canvas* c) {
  width = w;
  height = h;
  cap = s;
  size = 0;
  ants = new LangtonAnt*[cap];
  filled = new bool[w * h]();  // Create an empty bitmap for the window
  can = c;
  shading = false;
  inParallel = true;
}

AntFarm::~AntFarm() {
  for (int i = 0; i < size; i++) {
    delete ants[i];
  }
  delete[] ants;
  delete[] filled;
}

void AntFarm::addAnt(int x, int y, int r, int g, int b, int d) {
  if (size < cap) {
    ants[size] = new LangtonAnt(x, y, r, g, b, d, this);
    size++;
  }
}

inline void AntFarm::moveAnt(int j) {
  if (filled[ants[j]->myX + width * ants[j]->myY]) {
      ants[j]->myDir = (ants[j]->myDir + 1) % 4;
      if (shading)
        can->drawPoint(ants[j]->myX, ants[j]->myY, ColorInt(ants[j]->myRed/2, ants[j]->myGreen/2, ants[j]->myBlue/2, ants[j]->myAlpha));
      else
        can->drawPoint(ants[j]->myX, ants[j]->myY, ColorInt(MAX_COLOR / 2, MAX_COLOR / 2, MAX_COLOR / 2, ants[j]->myAlpha));
  } else {
      ants[j]->myDir = (ants[j]->myDir + 3) % 4;
      can->drawPoint(ants[j]->myX, ants[j]->myY, ColorInt(ants[j]->myRed, ants[j]->myGreen, ants[j]->myBlue, ants[j]->myAlpha));
  }
}

void AntFarm::moveAnts() {
  if (inParallel) {
    #pragma omp parallel num_threads(omp_get_num_procs())
    {
      int nthreads = omp_get_num_threads();
      int tid = omp_get_thread_num();
      for (int j = tid; j < size; j+= nthreads)
        moveAnt(j);
    }
  } else {
    for (int j = 0; j < size; j++)
      moveAnt(j);
  }
  for (int j = 0; j < size; j++) {
      filled[ants[j]->myX + width * ants[j]->myY] ^= true;
      ants[j]->move();
  }
}

void AntFarm::setShading(bool b) {
  shading = b;
}

void AntFarm::setParallel(bool b) {
  inParallel = b;
}
