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

enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

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
void updateFunction5(CartesianCanvas* can) {
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
	long double j;
	static int numProcs = 8;
	unsigned int iterations, tid;
	int share = can->getWindowHeight() / numProcs;
	if (can->getFrameNumber() <= share) {	// As long as we aren't trying to render off of the screen...
		double i;
		unsigned int depth = 255;

		#pragma omp parallel num_threads(numProcs) private(i, j, iterations, tid)
		{
			tid = omp_get_thread_num();
			j = -1.125 + ((can->getFrameNumber()-1)*numProcs+tid)*can->getPixelHeight();
			for (i = -2; i <= 1; i += can->getPixelWidth()) {
				std::complex<double> originalComplex(i, j);
				std::complex<double> complex(i, j);
				iterations = 0;
				while (std::abs(complex) < 2 && iterations != depth) {
					iterations++;
					complex = originalComplex + std::pow(complex, 2);
				}
				long double ci,cj;
				can->getScreenCoordinates(i,j,ci,cj);
				if (iterations == depth)					// If the point never escaped...
					can->drawPointColor(i, j, 0,0,0);
					//can->drawPointColor(i, j, (int)(ci*cj) % 255, (int)(ci*cj) % 255, (int)(ci*cj) % 255);		// Draw it black
				else
					can->drawPointColor(i, j, iterations, iterations, iterations);	// Draw it grayscale
			}
		}
	}
}

void mandelbrotFunction2(CartesianCanvas* can) {
	long double j;
	if (can->getFrameNumber() <= can->getWindowHeight()) {	// As long as we aren't trying to render off of the screen...
		long double i;
		unsigned int depth = 500;

		j = -1.125 + can->getFrameNumber()*can->getPixelHeight();

//		#pragma omp parallel num_threads(omp_get_num_procs()) private(i)
//		{
			for (i = -2; i <= 1; i += can->getPixelWidth()) {
				std::complex<double> originalComplex(i, j);
				std::complex<double> complex(i, j);
				unsigned int iterations = 0;

				while (std::abs(complex) < 2 && iterations != depth) {
					iterations++;
					complex = originalComplex + std::pow(complex, 2);
				}

				// do color stuff here based on count
				if (iterations == depth) {					// If the point never escaped...
					can->drawPointColor(i, j, 0, 0, 0);		// Draw it black
				} else {
					can->drawPointColor(i, j, 255, 255, 255);	// Draw it white
					/*while (true) {
						if (iterations < 50) {
							return color(myColor1 + difference1to2 * iterations / 50, 255, 255);
						}
						else if (iterations < 100) {
							return color(myColor2 + difference2to3 * (iterations - 50) / 50, 255, 255);
						}
						else if (iterations < 150) {
							return color(myColor3 + difference3to1 * (iterations - 100) / 50, 255, 255);
						}
						iterations -= 150;
					}*/
//				}
			}
		}
	}
}

static bool filled[800][600] = { false };
static int xx[4],yy[4], dir[4], red[4], green[4], blue[4];
void langtonFunction(CartesianCanvas* can) {
	static int xx = 400, yy = 300;
	static int direction = UP;
	for (int i = 0; i < 500; i ++) {
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

void langtonInit() {
	xx[0] = 300; yy[0] = 300; red[0] = 255; green[0] = 0;   blue[0] = 0;
	xx[1] = 400; yy[1] = 200; red[1] = 0;   green[1] = 0;   blue[1] = 255;
	xx[2] = 500; yy[2] = 300; red[2] = 0;   green[2] = 255; blue[2] = 0;
	xx[3] = 400; yy[3] = 400; red[3] = 255; green[3] = 0;   blue[3] = 255;
	for (int i = 0; i < 4; i++) { dir[i] = i; }
}

void langtonFunction2(CartesianCanvas* can) {
	for (int i = 0; i < 5000; i++) {
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
				if (xx[j] < 799)
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
					xx[j] = 799;
			}
		}
		for (int j = 0; j < 4; j++) {
			filled[xx[j]][yy[j]] = !filled[xx[j]][yy[j]];
		}
	}
}

int main() {
//	Canvas* can = new Canvas(updateFunction2, -1);
////	can->setAutoRefresh(false);
//	can->start();

//	Canvas* can2 = new Canvas(updateFunction2);
//	can2->start();

//	CartesianCanvas* can = new CartesianCanvas(updateFunction5, -1);
//	can->start();

//	CartesianCanvas* can = new CartesianCanvas(mandelbrotFunction,
//			0, 0, WINDOW_W, WINDOW_H, -1.1, -1.125, 1, 1.125, -1);

	langtonInit();
	CartesianCanvas* can = new CartesianCanvas(langtonFunction2,
			0, 0, WINDOW_W, WINDOW_H, 0,0,800,600, -1);

//	can->setAutoRefresh(false);
//	mandelbrotFunction2(can);
//	can->start();
	can->start();

}
