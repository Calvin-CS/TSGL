/*
 * AntFarm.h
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#ifndef ANTFARM_H_
#define ANTFARM_H_

#include "LangtonAnt.h"

const int NUM_COLORS = 256, MAX_COLOR = 255;

class LangtonAnt;  //Forward Declaration

class AntFarm {
private:
    LangtonAnt** ants;
    bool* filled;
public:
    int width, height, size, cap;
    Canvas* can;
    AntFarm(int w, int h, int s, Canvas* can);
    ~AntFarm();
    void addAnt(int x, int y, int r, int g, int b, int d);
    void moveAnts();
};

#endif /* ANTFARM_H_ */
