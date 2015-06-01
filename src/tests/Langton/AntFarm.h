/*
 * AntFarm.h
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include <omp.h>

#include <tsgl.h>
#include "LangtonAnt.h"

class LangtonAnt;  //Forward Declaration

class AntFarm {
private:
    bool* filled;
    bool shading;
    bool inParallel;
    void moveAnt(int j);
public:
    LangtonAnt** ants;
    int width, height, size, cap;
    Canvas* can;
    AntFarm(int w, int h, int s, Canvas* can);
    ~AntFarm();
    void addAnt(int x, int y, int r, int g, int b, int d);
    void moveAnts();
    void setShading(bool b);
    void setParallel(bool b);
};

#endif /* ANTFARM_H_ */
