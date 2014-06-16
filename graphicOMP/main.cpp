/*
 * main.cpp provides example usage for the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/12/2014
 */

#include "Canvas.h"
#include "CartesianCanvas.h"
#include "Array.h"

#include <omp.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <complex>
#include <thread>

const int WINDOW_X = 200, WINDOW_Y = 200, WINDOW_W = 800, WINDOW_H = 600;
const int WINDOW_CW = WINDOW_W/2, WINDOW_CH = WINDOW_H/2;
const double PI = 3.1415926536;
bool reverse = false;

// Shared values between langton functions
enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
static bool filled[800][600] = {};
static int xx[4],yy[4], dir[4], red[4], green[4], blue[4];

static void print(const char* cs) {
	std::cout << cs << std::endl << std::flush;
}
static void print(const double d) {
	std::cout << d << std::endl << std::flush;
}

void points1(Canvas* can) {
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
void points2(Canvas* can) {
	print("Computation started");
	int tid, nthreads, i, j;
	//can->setColor(80,10,160);
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		int myShare = can->getWindowHeight() / nthreads;
		int myStart = myShare * tid;
		for (j = myStart; j < myStart + myShare; j++) {
			for (i = 100; i < WINDOW_W-100; i++) {
				if (i % 2 == 0)
					can->drawPointColor(i,j,j % 255,i % 255,i*j % 113);
				else
					can->drawPointColor(i,j,i % 255,j % 255,i*j % 256);
			}
		}
	}
	print("Computation finished");
}
void points3(Canvas* can) {
	print("Computation started");
	int tid, nthreads, i, color;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,color)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		for (int j = tid; j < WINDOW_H; j+= nthreads) {
			for (i = 0; i < WINDOW_W; i++) {
				color = i*128/WINDOW_W + j*128/WINDOW_H;
				can->drawPointColor(i,j,color,color,color);
			}
		}
	}
	print("Computation finished");
}

void lines1(Canvas* can) {
	print("Computation started");
	int a,b,c = WINDOW_CW,d = WINDOW_CH,e,f,g;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
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
	}
	print("Computation finished");
}
void lines2(Canvas* can) {
	print("Computation started");
	int tid;
	int a,b,c = WINDOW_CW,d = WINDOW_CH,e,f,g;
	reverse = !reverse;
	int lastFrame = 0;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,a,b,c,d,e,f,g)
	{
		tid = omp_get_thread_num();
		while(can->isOpen()) {
			if (can->getFrameNumber() > lastFrame) {
				lastFrame = can->getFrameNumber();
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
	}
	print("Computation finished");
}
void shadingPoints(Canvas* can) {
	int tid, nthreads, i, j;
	int lastFrame = 0;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j)
	{
		while(can->isOpen()) {
			if (can->getFrameNumber() > lastFrame) {
				lastFrame = can->getFrameNumber();
				nthreads = omp_get_num_threads();
				tid = omp_get_thread_num();
				for (i = tid; i < 256; i+= nthreads) {
					for (j = 0; j <= 256; j++) {
						can->drawPointColor(i,j,i,j,can->getFrameNumber() % 256);
					}
				}
			}
		}
	}
}

void mandelbrotFunction(CartesianCanvas* can) {
	const unsigned int threads = 32;
//	const unsigned int threads = omp_get_num_procs();
	const unsigned int depth = 255;
	unsigned int iterations;
	#pragma omp parallel num_threads(threads) private(iterations)
	{
		for (int k = 0; k <= (can->getWindowHeight() / threads); k++) { // As long as we aren't trying to render off of the screen...
			long double j = ((can->getMaxY() - can->getMinY()) / threads) * omp_get_thread_num()
								+ can->getMinY() + can->getPixelHeight() * k;
			for (long double i = can->getMinX(); i <= can->getMaxX(); i += can->getPixelWidth()) {
				std::complex<long double> originalComplex(i, j);
				std::complex<long double> complex(i, j);
				iterations = 0;

				while (std::abs(complex) < 2.0 && iterations != depth) {
					iterations++;
					complex = originalComplex + std::pow(complex, 2);
				}

				if (iterations == depth) {					// If the point never escaped...
					can->drawPointColor(i, j, 0, 0, 0);		// Draw it black
				} else {
					can->drawPointColor(i, j, iterations % 151, (iterations % 131) + 50, iterations % 255);	// Draw with color
				}
			}
		}
	}
}

void langtonFourWayInit() {  // Not a function for calling by a Canvas, but used for setting up the other langton ones
	xx[0] = 200; yy[0] = 300; red[0] = 255; green[0] = 0;   blue[0] = 0;
	xx[1] = 300; yy[1] = 200; red[1] = 0;   green[1] = 0;   blue[1] = 255;
	xx[2] = 400; yy[2] = 300; red[2] = 0;   green[2] = 255; blue[2] = 0;
	xx[3] = 300; yy[3] = 400; red[3] = 255; green[3] = 0;   blue[3] = 255;
	for (int i = 0; i < 4; i++) { dir[i] = i; }
}

void langtonFunction(CartesianCanvas* can) {
	static int IPF = 500;		// Iterations per frame
	static int xx = 400, yy = 300;
	static int direction = UP;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
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
	}
}

void langtonFunction2(CartesianCanvas* can) {
	static int IPF = 1000;		// Iterations per frame
//	const unsigned int threads = 4;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
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
	}
}
void langtonFunction3(CartesianCanvas* can) {
	static int IPF = 1000;		// Iterations per frame
//	const unsigned int threads = 4;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				//#pragma omp parallel for
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j]][yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
		//				can->drawPointColor(xx[j],yy[j],red[j]/2,green[j]/2,blue[j]/2);
						can->drawPointColor(xx[j],yy[j],128,128,128);
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						can->drawPointColor(xx[j],yy[j],red[j],green[j],blue[j]);
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
	}
}
void langtonFunctionShiny(CartesianCanvas* can) {
	static int IPF = 1000;		// Iterations per frame
//	const unsigned int threads = 4;
	RGBType color;
	HSVType other;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				//#pragma omp parallel for
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j]][yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
						other = {((can->getFrameNumber() + 3*j)%12) / 2.0f,1.0f,1.0f};
						color = Canvas::HSVtoRGB(other);
						can->drawPointColor(xx[j],yy[j],color.R*255,color.G*255,color.B*255);
		//				can->drawPointColor(xx[j],yy[j],red[j],green[j],blue[j]);
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						other = {((can->getFrameNumber() + 3*j)%12) / 2.0f,1.0f,0.5f};
						color = Canvas::HSVtoRGB(other);
						can->drawPointColor(xx[j],yy[j],color.R*255,color.G*255,color.B*255);
		//				can->drawPointColor(xx[j],yy[j],red[j]/2,green[j]/2,blue[j]/2);
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
	}
}

void dumbSortFunction(CartesianCanvas* can) {
	const int SIZE = 350;
	int numbers[SIZE];
	int pos = 0, temp, min = 1, max = SIZE-1, lastSwap = 0;
	bool goingUp = true;
	for (int i = 0; i < SIZE; i++) {
		numbers[i] = rand() % SIZE;
	}
	int lastFrame = 0;
	while(can->isOpen()) {
		if (min != max) {
			if (goingUp) {
				if (numbers[pos] > numbers[pos+1]) {
					temp = numbers[pos];
					numbers[pos] = numbers[pos+1];
					numbers[pos+1] = temp;
					lastSwap = pos;
				}
				if (pos >= max) {
					pos = max;
					if (lastSwap < max)
						max = lastSwap;
					else
						max--;
					goingUp = !goingUp;
				} else
					pos++;
			}
			else {
				if (numbers[pos] < numbers[pos-1]) {
					temp = numbers[pos];
					numbers[pos] = numbers[pos-1];
					numbers[pos-1] = temp;
					lastSwap = pos;
				}
				if (pos <= min) {
					pos = min;
					if (lastSwap > min)
						min = lastSwap;
					else
						min++;
					goingUp = !goingUp;
				} else
					pos--;
			}
		}
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			can->drawRectangleColor(0,0,800,600,128,128,128);
			int start = 50, width = 1, height;
			for (int i = 0; i < SIZE; i++) {
				height = (numbers[i]);
				if (i == pos)
					can->drawRectangleColor(start,580-height,width,height,255,255,0);
				else
					can->drawRectangleColor(start,580-height,width,height,255,0,0);
				start += width+1;
			}
		}
	}
}

void colorWheelFunction(CartesianCanvas* can) {
	const int threads = 256, delta = 256/threads;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			int f = lastFrame % 256;
			int start[threads] = { 0 };
			start[0] = f;
			for (int i = 1; i < threads; i++) {
				start[i] = (start[i-1] + delta) % 255;
			}
			RGBType color;
			HSVType other;
			const float RADIUS = 280;
			float x2, x3, y2, y3;
			int shading, tid;
			#pragma omp parallel num_threads(threads) private(other,color,x2,x3,y2,y3,shading,tid)
			{
				tid = omp_get_thread_num();
				shading = 255-tid*256/threads;
				other = {start[tid]/255.0f*6.0f,1.0f,1.0f};
				color = Canvas::HSVtoRGB(other);
				x2 = RADIUS*sin(2*PI*start[tid]/255.0);
				y2 = RADIUS*cos(2*PI*start[tid]/255.0);
				x3 = RADIUS*sin(2*PI*(start[tid]+1)/255.0);
				y3 = RADIUS*cos(2*PI*(start[tid]+1)/255.0);
				can->drawTriangleColor(400,300,400+x2,300+y2,400+x3,300+y3,
						shading*color.R,shading*color.G,shading*color.B);
			}
		}
	}
}

void functionFunction(CartesianCanvas* can) {
	Function* function1 = new CosineFunction;
//	Function* function2 = new PowerFunction(2);
//
//	class myFunction : public Function {
//	public:
//		virtual long double valueAt(long double x) const {
//			return 5*pow(x,4) + 2*pow(x,3) + x + 15;
//		}
//	};
//
//	Function* function3 = new myFunction;

	can->drawFunction(function1);
//	can->drawFunction(function2);
//	can->drawFunction(function3);

}

Function* function1 = new CosineFunction;
void integral1(CartesianCanvas* can) {
	const unsigned int threads = 1;

	if (can->getFrameNumber() - 1 <= (can->getWindowWidth() / threads)) {	// As long as we aren't trying to render off of the screen...
		can->drawFunction(function1);
		#pragma omp parallel num_threads(threads)
		{
			long double x = (can->getMaxX() - can->getMinX()) / threads * omp_get_thread_num()
								+ can->getMinX() + can->getPixelWidth() * can->getFrameNumber();

			can->drawLineColor(x, 0, x, function1->valueAt(x), 0,0,0);
		}
	} else {
		static bool shownTime = false;
		if (!shownTime) {
			std::cout << can->getTime() << std::endl;
			can->showFPS(false);
			shownTime = true;
		}
	}
}

int main() {
//	Canvas* can1 = new Canvas(480800);
//	can1->start();
//	can1->setAutoRefresh(false);
//	points1(can1);
//	can1->end();

//	Canvas* can2 = new Canvas(480000);
//	can2->start();
//	can2->setAutoRefresh(false);
//	points2(can2);
//	can2->end();

//	Canvas* can3 = new Canvas(480000);
//	can3->start();
//	can3->setAutoRefresh(false);
//	points3(can3);
//	can3->end();

//	Canvas* can4 = new Canvas(100000);
//	can4->setAutoRefresh(false);
//	can4->start();
//	lines1(can4);
//	can4->end();

//	Canvas* can5 = new Canvas(1000);
//	can5->setAutoRefresh(false);  //THIS CRASHES: WAIT UNTIL ARRAY IS FIXED
//	can5->start();
//	lines2(can5);
//	can5->end();

//	Canvas* can6 = new Canvas(250000);
//	can6->showFPS(true);
//	can6->setAutoRefresh(false);
//	can6->start();
//	shadingPoints(can6);
//	can6->end();

//	CartesianCanvas* can7 = new CartesianCanvas(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, 500000);
//	can7->start();
//	can7->setAutoRefresh(false);
//	can7->showFPS(true);
//	mandelbrotFunction(can7);
//	can7->showFPS(false);
//	print(can7->getTime());
//	can7->end();

//	CartesianCanvas* can8 = new CartesianCanvas(0, 0, WINDOW_W, WINDOW_H, 0,0,800,600, 100000);
//	can8->setAutoRefresh(false);
//	can8->start();
//	langtonFunction(can8);
//	can8->end();

//	langtonFourWayInit();
//	CartesianCanvas* can9 = new CartesianCanvas(0, 0, 600, 600, 0,0,600,600, -1);
//	can9->start();
//	langtonFunction2(can9);
//	can9->end();

//	langtonFourWayInit();
//	CartesianCanvas* can10 = new CartesianCanvas(0, 0, 600, 600, 0,0,600,600, -1);
//	can10->start();
//	langtonFunction3(can10);
//	can10->end();

//	langtonFourWayInit();
//	CartesianCanvas* can11 = new CartesianCanvas(0, 0, 600, 600, 0,0,600,600, -1);
//	can11->start();
//	langtonFunctionShiny(can11);
//	can11->end();

//	CartesianCanvas* can12 = new CartesianCanvas(0, 0, 800, 600, 0,0,800,600, -1);
//	can12->start();
//	can12->setAutoRefresh(false);
//	dumbSortFunction(can12);
//	can12->end();

//	CartesianCanvas* can13 = new CartesianCanvas(0, 0, 800, 600, 0,0,800,600, 512);
//	can13->setAutoRefresh(false);
//	can13->start();
//	colorWheelFunction(can13);
//	can13->end();

//	CartesianCanvas* can14 = new CartesianCanvas(0, 0, 800, 600, -5,-5,5,50, 0);
//	can14->setAutoRefresh(false);
//	can14->start();
//	can14->showFPS(true);
//	functionFunction(can14);
//	can14->showFPS(false);
//	print(can14->getTime());
//	can14->end();

//	CartesianCanvas* can15 = new CartesianCanvas(integral1,
//										0, 0, 800, 600, -5,-1.5,5,1.5, 64);
//	can15->showFPS(true);
//	can15->start();
}
