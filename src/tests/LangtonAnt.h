/*
 * testAlphaLangton.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#ifndef LANGTONANT_H_
#define LANGTONANT_H_

#include <cmath>
#include <complex>
#include <iostream>
#include <omp.h>
#include <queue>
#include <tsgl.h>

#include "AntFarm.h"

class AntFarm;  //Forward Declaration

// Shared values between langton functions
enum direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
};

class LangtonAnt {
public:
    int myX, myY, myRed, myGreen, myBlue, myDir;
    AntFarm* myFarm;
public:
    LangtonAnt(int x, int y, int r, int g, int b, int d, AntFarm* p);
    void move();
};

#endif /* LANGTONANT_H_ */
