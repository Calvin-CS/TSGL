/*
 * testAlphaLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#ifndef CONWAYANT_H_
#define CONWAYANT_H_

#include "LifeFarm.h"

class LifeFarm;  //Forward Declaration

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

class ConwayAnt {
public:
    int myX, myY, myRed, myGreen, myBlue, myAlpha, myDir;
    LifeFarm* myFarm;
public:
    ConwayAnt(int x, int y, int r, int g, int b, int d, LifeFarm* p);
    void move();
    void changeColor(int r, int g, int b);
    void changeColor(ColorFloat c);
    void setAlpha(int a);
};

#endif /* CONWAYANT_H_ */
