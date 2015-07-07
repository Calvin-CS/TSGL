/*
 * Voronoi.h
 *
 *  Created on: May 29, 2015
 *      Author: Chris Dilley
 */

#ifndef VORONOI_H_
#define VORONOI_H_

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

using namespace tsgl;

const int WINDOW_W = 400*3, WINDOW_H = 300*3, BUFFER = WINDOW_W * WINDOW_H * 2;

class Voronoi {
protected:
  static const int MY_POINTS = 100 * 4;
  int * myX;
  int * myY;
  int * myKValue;
  ColorFloat myColor[MY_POINTS];                 // And for an array of colors
  ColorFloat myTC, myRC, myLC, myBC, myXC, myYC; // Color for the top, right, left, bottom, x-average, and y-average
public:
  Voronoi(Canvas& can);
  void draw(Canvas& can);
  virtual ~Voronoi();
};

#endif /* VORONOI_H_ */
