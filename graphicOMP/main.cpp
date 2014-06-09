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
/*void mandelbrotFunction(Canvas* can) {
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

	 these are used for calculating the points corresponding to the pixels
	xstep = 1;
	ystep = 1;

	the main loop
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
}*/
void mandelbrotFunction2(CartesianCanvas* can) {
	if (can->getFrameNumber() <= can->getWindowHeight()) {	// As long as we aren't trying to render off of the screen...
		double i, j;
		unsigned int depth = 150;

		j = -1.5 + (can->getFrameNumber() - 1) * can->getPixelHeight();	// Get height based on frame number

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
				}
//			}
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

	CartesianCanvas* can = new CartesianCanvas(mandelbrotFunction2, 0, 0, 800, 600,
												-2, -1.5, 1, 1.5, -1);
//	can->setAutoRefresh(false);
//	mandelbrotFunction2(can);
	can->start();

}
