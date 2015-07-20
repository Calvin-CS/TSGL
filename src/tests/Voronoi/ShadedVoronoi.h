/*
 * ShadedVoronoi.h
 */

#ifndef SHADEDVORONOI_H_
#define SHADEDVORONOI_H_

#include "Voronoi.h"

class ShadedVoronoi : public Voronoi {
private:
  int * myKValue2;
  ColorFloat myColor[MY_POINTS];
public:
  ShadedVoronoi(Canvas& can);
  void draw(Canvas& can);
  virtual ~ShadedVoronoi();
};

#endif /* SHADEDVORONOI_H_ */
