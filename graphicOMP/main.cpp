/*
 * main.cpp provides example usage for the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/9/2014
 */

#include "Canvas.h"
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

const int WINDOW_W = 800, WINDOW_H = 600, WINDOW_CW = WINDOW_W/2, WINDOW_CH = WINDOW_H/2;
int a,b,c = WINDOW_CW,d = WINDOW_CH,e,f,g;
bool reverse = false;

void updateFunction1(Canvas* can) {
	int tid, nthreads, i, j, color;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j,color)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		for (i = tid; i < WINDOW_W; i+= nthreads) {
			for (j = 0; j <= WINDOW_H; j++) {
				color = i*128/WINDOW_W + j*128/WINDOW_H;
				can->drawPointColor(i,j,color,color,color);
			}
		}
	}
}
void updateFunction2(Canvas* can) {
	int tid, nthreads, i, j;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		j = can->getFrameNumber();
		for (i = 100+tid; i < WINDOW_W-100; i+= nthreads) {
			//for (j = 100; j < WINDOW_H-100; j++) {
			can->setColor(80,10,160);
			if (i % 2 == 0)
				can->drawPointColor(i,j,j,i,i*j % 113);
			else
				can->drawPointColor(i,j,i,j,i*j % 256);
		}
	}
}
void updateFunction3(Canvas* can) {
	int tid, nthreads, i, j, color;
	j = can->getFrameNumber();
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,color)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		for (i = tid; i < WINDOW_W; i+= nthreads) {
			color = i*128/WINDOW_W + j*128/WINDOW_H;
			can->drawPointColor(i,j,color,color,color);
		}
	}
}
void updateFunction4(Canvas* can) {
	a = c;
	b = d;
	c = rand() % WINDOW_W;
	d = rand() % WINDOW_H;
	e = rand() % 256;
	f = rand() % 256;
	g = rand() % 256;
	can->setAutoRefresh(false);
	can->drawLineColor(a,b,c,d,e,f,g);
}
void updateFunction5(Canvas* can) {
	int tid;
	reverse = !reverse;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,a,b,c,d,e,f,g)
	{
		tid = omp_get_thread_num();
		if (reverse) {
			a = WINDOW_CW + WINDOW_CW*sin(7.11*tid+(180+can->getFrameNumber()-1)*M_PI/180);
			b = WINDOW_CH + WINDOW_CH*cos(7.11*tid+(180+can->getFrameNumber()-1)*M_PI/180);
			c = WINDOW_CW + WINDOW_CW*sin(7.11*tid+can->getFrameNumber()*M_PI/180);
			d = WINDOW_CH + WINDOW_CH*cos(7.11*tid+can->getFrameNumber()*M_PI/180);
		} else {
			a = WINDOW_CW + WINDOW_CW*sin(7.11*tid+(can->getFrameNumber()-1)*M_PI/180);
			b = WINDOW_CH + WINDOW_CH*cos(7.11*tid+(can->getFrameNumber()-1)*M_PI/180);
			c = WINDOW_CW + WINDOW_CW*sin(7.11*tid+(180+can->getFrameNumber())*M_PI/180);
			d = WINDOW_CH + WINDOW_CH*cos(7.11*tid+(180+can->getFrameNumber())*M_PI/180);
		}
		e = (a + can->getFrameNumber()) % 256;
		f = (b + can->getFrameNumber()) % 256;
		g = (a*b + can->getFrameNumber()) % 256;
		can->drawLineColor(a,b,c,d,e,f,g);
	}
}
void updateFunction6(Canvas* can) {
	int tid, nthreads, i, j;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		for (i = tid; i < 256; i+= nthreads) {
			for (j = 0; j <= 256; j++) {
				can->drawPointColor(i,j,i,j,can->getFrameNumber() % 256);
			}
		}
	}
}
void mandelbrotFunction(Canvas* can) {
	double x,y;
	double xstep,ystep;
	double z,zi,newz,newzi;
	double colour;
	int iter = 10;
	long col;
	int temp;
	char pic[WINDOW_H][WINDOW_W];
	int i,j,k;
	bool inset;
	int fd;

	/* these are used for calculating the points corresponding to the pixels */
	xstep = 1;
	ystep = 1;

	/*the main loop */
	x = 0;
	y = 0;
	for (i = 0; i < WINDOW_H; i++) {
		for (j = 0; j < WINDOW_W; j++) {
			z = 0;
			zi = 0;
			inset = true;
			for (k=0; k<iter; k++) {
				newz = (z*z)-(zi*zi) + x;
				newzi = 2*z*zi + y;
				z = newz;
				zi = newzi;
				if(((z*z)+(zi*zi)) > 4) {
					inset = false;
					colour = k;
					k = iter;
				}
			}
			temp = inset ? 0 : (colour / iter * 255.0f);
			pic[i][j] = temp;
			std::cout << temp << std::endl;
			x += xstep;
		}
		y += ystep;
		x = 0;
	}
	for (i = 0; i < WINDOW_H; i++) {
		for (j = 0; j <= WINDOW_W; j++) {
			can->drawPointColor(i,j,pic[i][j],pic[i][j]/2,pic[i][j]/2);
		}
	}
}

int main() {
	Canvas* can = new Canvas(updateFunction5, 1000);

//	mandelbrotFunction(can);  //UNFINISHED

	//updateFunction(can);

	//can->setAutoRefresh(false);

	can->start();

//	can->setAutoRefresh(false);
//	Canvas* can2 = new Canvas(updateFunction2);
//	can2->start();
}
