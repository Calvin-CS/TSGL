/*
 * AntFarm.h
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include <tsgl.h>
#include <omp.h>
#include "ConwayAnt.h"

const int NUM_COLORS = 256, MAX_COLOR = 255;

class ConwayAnt;  //Forward Declaration

class LifeFarm {
private:
    unsigned framecount;
    bool **filledA, **filledB;
    bool drawdead;
    bool flipped;
public:
    int width, height, size;
    Canvas* can;
    LifeFarm(int w, int h, Canvas* can, bool randomize);
    ~LifeFarm();
    void addAnt(int x, int y);
    void moveAnts();
    bool lives(int row, int col);
    void setDrawdead(bool b);
};

#endif /* ANTFARM_H_ */
