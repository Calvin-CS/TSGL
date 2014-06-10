/*
 * main.cpp provides example usage for the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 6/9/2014
 */

#include "Canvas.h"
#include "CartesianCanvas.h"
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <complex>

const int WINDOW_X = 200, WINDOW_Y = 200, WINDOW_W = 800, WINDOW_H = 600;
const int WINDOW_CW = WINDOW_W/2, WINDOW_CH = WINDOW_H/2;
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

void mandelbrotFunction(CartesianCanvas* can) {
	const unsigned int threads = omp_get_num_procs();

	if (can->getFrameNumber() <= (can->getWindowHeight() / threads)) {	// As long as we aren't trying to render off of the screen...
		long double i, j;
		unsigned int iterations;
		const unsigned int depth = 255;

		j = -1.125 + (can->getFrameNumber() - 1) * can->getPixelHeight() * threads;

		#pragma omp parallel num_threads(threads) private(i, iterations)
		{
			long double j_ = j + can->getPixelHeight() * omp_get_thread_num();

			for (i = -2; i <= 1; i += can->getPixelWidth()) {
				std::complex<long double> originalComplex(i, j_);
				std::complex<long double> complex(i, j_);
				iterations = 0;

				while (std::abs(complex) < 2.0 && iterations != depth) {
//					complex = std::abs(complex);
					iterations++;
					complex = originalComplex + std::pow(complex, 2);
				}

				if (iterations == depth) {					// If the point never escaped...
					can->drawPointColor(i, j_, 0, 0, 0);	// Draw it black
				} else {
					can->drawPointColor(i, j_, iterations % 151, (iterations % 131) + 50, iterations);	// Draw with color
				}
			}
		}
	}
}

enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
static bool filled[800][600] = {false};
static int xx[4],yy[4], dir[4], red[4], green[4], blue[4];
void langtonFunction(CartesianCanvas* can) {
	static int IPF = 500;		// Iterations per frame
	static int xx = 400, yy = 300;
	static int direction = UP;
	for (int i = 0; i < IPF; i ++) {
		if (filled[xx][yy]) {
			direction = (direction + 1) % 4;
			can->drawPointColor(xx,yy,255,0,0);
		}
		else {
			direction = (direction + 3) % 4;
			can->drawPointColor(xx,yy,0,0,0);
		}
		filled[xx][yy] = !filled[xx][yy];
		if (direction == UP)
			yy = yy > 0 ? yy-1 : 599;
		else if (direction == RIGHT)
			xx = xx < 799 ? xx+1 : 0;
		else if (direction == DOWN)
			yy = yy < 599 ? yy+1 : 0;
		else if (direction == LEFT)
			xx = xx > 0 ? xx-1 : 799;
		else
			std::cout << "BAD: dir == " << direction << std::endl;
	}
}
void langtonFourWayInit() {
	xx[0] = 200; yy[0] = 300; red[0] = 255; green[0] = 0;   blue[0] = 0;
	xx[1] = 300; yy[1] = 200; red[1] = 0;   green[1] = 0;   blue[1] = 255;
	xx[2] = 400; yy[2] = 300; red[2] = 0;   green[2] = 255; blue[2] = 0;
	xx[3] = 300; yy[3] = 400; red[3] = 255; green[3] = 0;   blue[3] = 255;
	for (int i = 0; i < 4; i++) { dir[i] = i; }
}
void langtonFunction2(CartesianCanvas* can) {
	static int IPF = 10000;		// Iterations per frame
	const unsigned int threads = 4;

	for (int i = 0; i < IPF; i++) {
		for (int j = 0; j < 4; j++) {
			if (filled[xx[j]][yy[j]]) {
				dir[j] = (dir[j] + 1) % 4;
				can->drawPointColor(xx[j],yy[j],red[j],green[j],blue[j]);
			}
			else {
				dir[j] = (dir[j] + 3) % 4;
				can->drawPointColor(xx[j],yy[j],red[j]/2,green[j]/2,blue[j]/2);
			}
			if (dir[j] == UP) {
				if (yy[j] > 0)
					yy[j] = yy[j] - 1;
				else
					yy[j] = 599;
			}
			else if (dir[j] == RIGHT) {
				if (xx[j] < 599)
					xx[j] = xx[j] + 1;
				else
					xx[j] = 0;
			}
			else if (dir[j] == DOWN) {
				if (yy[j] < 599)
					yy[j] = yy[j] + 1;
				else
					yy[j] = 0;
			}
			else if (dir[j] == LEFT) {
				if (xx[j] > 0)
					xx[j] = xx[j] - 1;
				else
					xx[j] = 599;
			}
		}
		for (int j = 0; j < 4; j++) {
			filled[xx[j]][yy[j]] = !filled[xx[j]][yy[j]];
		}
	}
}

void langtonFunction3(CartesianCanvas* can) {
	for (int i = 0; i < 500; i++) {
		//#pragma omp parallel for
		for (int j = 0; j < 4; j++) {
			if (filled[xx[j]][yy[j]]) {
				dir[j] = (dir[j] + 1) % 4;
				can->drawPointColor(xx[j],yy[j],red[j],green[j],blue[j]);
			}
			else {
				dir[j] = (dir[j] + 3) % 4;
				can->drawPointColor(xx[j],yy[j],red[j]/2,green[j]/2,blue[j]/2);
			}
		}
		for (int j = 0; j < 4; j++)
			filled[xx[j]][yy[j]] = !filled[xx[j]][yy[j]];
		for (int j = 0; j < 4; j++) {
			if (dir[j] == UP)
				yy[j] = (yy[j] > 0) ? yy[j] - 1 : 599;
			else if (dir[j] == RIGHT)
				xx[j] = (xx[j] < 599) ? xx[j] + 1 : 0;
			else if (dir[j] == DOWN)
				yy[j] = (yy[j] < 599) ? yy[j] + 1 : 0;
			else if (dir[j] == LEFT)
				xx[j] = (xx[j] > 0) ? xx[j] - 1 : 599;
		}
	}
}

int main() {
//	Canvas* can = new Canvas(updateFunction1);
//	can->start();

//	Canvas* can2 = new Canvas(updateFunction2);
//	can2->start();

//	Canvas* can3 = new Canvas(updateFunction3);
//	can3->start();

//	Canvas* can4 = new Canvas(updateFunction4, 500);
//	can4->setAutoRefresh(false);
//	can4->start();

//	Canvas* can5 = new Canvas(updateFunction5, 1000);
//	can5->setAutoRefresh(false);
//	can5->start();

//	Canvas* can6 = new Canvas(updateFunction6);
//	can6->start();

//	CartesianCanvas* can7 = new CartesianCanvas(mandelbrotFunction,
//									0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, -1);
//	can7->start();

//	CartesianCanvas* can8 = new CartesianCanvas(langtonFunction,
//									0, 0, WINDOW_W, WINDOW_H, 0,0,800,600, -1);
//	can8->start();

//	langtonFourWayInit();
//	CartesianCanvas* can9 = new CartesianCanvas(langtonFunction2,
//									0, 0, 600, 600, 0,0,600,600, -1);
//	can9->start();

//	langtonFourWayInit();
//	CartesianCanvas* can10 = new CartesianCanvas(langtonFunction3,
//									0, 0, 600, 600, 0,0,600,600, -1);
//	can10->start();

}
