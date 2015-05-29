/*
 * AntFarm.cpp
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#include "LifeFarm.h"

LifeFarm::LifeFarm(int w, int h, Canvas* c, bool randomize) {
  framecount = 0;
  width = w;
  height = h;
  size = w * h;
  filledA = new bool*[h]();
  for (int i = 0; i < h; ++i) {
    filledA[i] = new bool[w];
    if (randomize)
      for (int j = 0; j < w; ++j)
        filledA[i][j] = ((rand() % 2) > 0);
  }
  filledB = new bool*[h]();
  for (int i = 0; i < h; ++i) {
    filledB[i] = new bool[w];
    if (randomize)
      for (int j = 0; j < w; ++j)
        filledB[i][j] = 0;
  }
  flipped = false;
  can = c;
  drawdead = false;
}
LifeFarm::~LifeFarm() {
  delete filledA;
  delete filledB;
}
void LifeFarm::addAnt(int x, int y) {
  if (flipped)
    filledB[y][x] = true;
  else
    filledA[y][x] = true;
}
void LifeFarm::moveAnts() {
  const int P1 = 7, P2 = 11, P3 = 17;
  ++framecount;
  std::cout << framecount << std::endl;
  int r = (framecount*P1/50 % 510);
  if (r > 255)
    r = 510 - r;
  int g = (framecount*P2/50 % 510);
  if (g > 255)
    g = 510 - g;
  int b = (framecount*P3/50 % 510);
  if (b > 255)
    b = 510 - b;
  if (flipped) {
#pragma omp parallel num_threads(4)
{
    bool lastfilled;
    int nthreads = omp_get_num_threads();
    int tid = omp_get_thread_num();
    int count = (width*height)/nthreads;
    int col = 0, row = tid*(height/nthreads);
    for (int i = 0; i < count; ++i) {
      lastfilled = filledA[row][col];
      filledA[row][col] = lives(row,col);
      if (lastfilled != filledA[row][col]) {
        if (filledA[row][col])
          can->drawPoint(col, row, ColorInt(r,g,b,255));
        else if (drawdead)
          can->drawPoint(col, row, can->getBackgroundColor());
      }
      ++col;
      if (col == width) {
        col = 0;
        ++row;
      }
    }
}
  } else {
#pragma omp parallel num_threads(4)
{
    bool lastfilled;
    int nthreads = omp_get_num_threads();
    int tid = omp_get_thread_num();
    int count = (width*height)/nthreads;
    int col = 0, row = tid*(height/nthreads);
    for (int i = 0; i < count; ++i) {
      lastfilled = filledB[row][col];
      filledB[row][col] = lives(row,col);
      if (lastfilled != filledB[row][col]) {
        if (filledB[row][col])
          can->drawPoint(col, row, ColorInt(r,g,b,255));
        else if (drawdead)
          can->drawPoint(col, row, can->getBackgroundColor());
      }
      ++col;
      if (col == width) {
        col = 0;
        ++row;
      }
    }
}
  }
  flipped ^= true;
}
bool LifeFarm::lives(int row, int col) {
  int tl, tm, tr, ml, me, mr, bl, bm, br, total;
  int xm = col > 1 ? col-1 : width-1;
  int xp = col < width-1 ? col+1 : 0;
  int ym = row > 1 ? row-1 : height-1;
  int yp = row < height-1 ? row+1 : 0;
  if (!flipped) {
    tl = filledA[ym][xm];
    ml = filledA[row][xm];
    bl = filledA[yp][xm];
    tm = filledA[ym][col];
    me = filledA[row][col];
    bm = filledA[yp][col];
    tr = filledA[ym][xp];
    mr = filledA[row][xp];
    br = filledA[yp][xp];
  } else {
    tl = filledB[ym][xm];
    ml = filledB[row][xm];
    bl = filledB[yp][xm];
    tm = filledB[ym][col];
    me = filledB[row][col];
    bm = filledB[yp][col];
    tr = filledB[ym][xp];
    mr = filledB[row][xp];
    br = filledB[yp][xp];
  }
  total = tl+tm+tr+ml+mr+bl+bm+br;
  return ( (total == 3) || ( (me + total )== 3) );
}

void LifeFarm::setDrawdead(bool b) {
  drawdead = b;
}
