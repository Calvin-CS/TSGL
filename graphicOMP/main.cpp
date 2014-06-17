/*
 * main.cpp provides example usage for the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/17/2014
 */

#include "Canvas.h"
#include "CartesianCanvas.h"
#include "Array.h"
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <cmath>
#include <complex>
#include <thread>

const int 	WINDOW_X = 200,
			WINDOW_Y = 200,
			WINDOW_W = 800,
			WINDOW_H = 600,
			WINDOW_CW = WINDOW_W/2,
			WINDOW_CH = WINDOW_H/2,
			NUM_COLORS = 256,
			MAX_COLOR = 255;
const double PI = 3.1415926536;

// Shared values between langton functions
enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
// For the testing functions
enum bgcolor { BG_NONE = -1, BG_BLACK = 0, BG_WHITE = 1 };

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

static void print(const double d) {
	std::cout << d << std::endl << std::flush;
}

void points1(Canvas* can) {
	int nthreads, i, j, color;
	nthreads = omp_get_num_threads();
	#pragma omp parallel num_threads(omp_get_num_procs()) private(i,j,color)
	{
		for (i = omp_get_thread_num(); i < WINDOW_W; i+= nthreads) {
			for (j = 0; j <= WINDOW_H; j++) {
				color = i*MAX_COLOR/2/WINDOW_W + j*MAX_COLOR/2/WINDOW_H;
				can->drawPointColor(i,j,color,color,color);
			}
		}
	}
}
void points2(Canvas* can) {
	int i, j, myShare = can->getWindowHeight() / omp_get_num_threads();
	#pragma omp parallel num_threads(omp_get_num_procs()) private(i,j)
	{
		int myStart = myShare * omp_get_thread_num();
		for (j = myStart; j < myStart + myShare; j++) {
			for (i = 100; i < WINDOW_W-100; i++) {
				if (i % 2 == 0)
					can->drawPointColor(i,j,j % NUM_COLORS,i % NUM_COLORS,i*j % 113);
				else
					can->drawPointColor(i,j,i % NUM_COLORS,j % NUM_COLORS,i*j % NUM_COLORS);
			}
		}
	}
}

void lines1(Canvas* can) {
	int a,b,c = WINDOW_CW,d = WINDOW_CH,e,f,g;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			a = c;
			b = d;
			c = rand() % WINDOW_W;
			d = rand() % WINDOW_H;
			e = rand() % NUM_COLORS;
			f = rand() % NUM_COLORS;
			g = rand() % NUM_COLORS;
			can->drawLineColor(a,b,c,d,e,f,g);
		}
	}
}
void lines2(Canvas* can) {
	const double 	RAD = M_PI/180,
					ARC = 7.11;
	int lastFrame = 0;
	#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int a,b,c,d,e,f,g;
		double angle, offset = omp_get_thread_num() * ARC;
		bool reverse = false;
		while(can->isOpen()) {
			if (can->getFrameNumber() > lastFrame) {
				lastFrame = can->getFrameNumber();
				angle = offset+lastFrame*RAD;
				a = WINDOW_CW * (1 + sin(angle));
				b = WINDOW_CH * (1 + cos(angle));
				c = WINDOW_CW * (1 - sin(angle));
				d = WINDOW_CH * (1 - cos(angle));
				e = (a + lastFrame) % NUM_COLORS;
				f = (b + lastFrame) % NUM_COLORS;
				g = (a*b + lastFrame) % NUM_COLORS;
				if (!reverse)
					can->drawLineColor(a,b,c,d,e,f,g);
				else
					can->drawLineColor(c,d,a,b,e,f,g);
				reverse = !reverse;
			}
		}
	}
}
void shadingPoints(Canvas* can) {
	int lastFrame = 0, nthreads = omp_get_num_procs();
	#pragma omp parallel num_threads(nthreads)
	{
		while(can->isOpen()) {
			if (can->getFrameNumber() > lastFrame) {
				lastFrame = can->getFrameNumber();
				for (int i = omp_get_thread_num(); i < NUM_COLORS; i+= nthreads)
					for (int j = 0; j <= NUM_COLORS; j++)
						can->drawPointColor(i,j,i,j,lastFrame % NUM_COLORS);
			}
		}
	}
}

void mandelbrotFunction(CartesianCanvas* can) {
	const unsigned int THREADS = 32;  //omp_get_num_procs();
	const unsigned int DEPTH = MAX_COLOR;
	const double BLOCKSTART = (can->getMaxY() - can->getMinY()) / THREADS;
	#pragma omp parallel num_threads(THREADS)
	{
		unsigned int iterations;
		for (int k = 0; k <= (can->getWindowHeight() / THREADS); k++) { // As long as we aren't trying to render off of the screen...
			long double row = BLOCKSTART * omp_get_thread_num() + can->getMinY() + can->getPixelHeight() * k;
			for (long double col = can->getMinX(); col <= can->getMaxX(); col += can->getPixelWidth()) {
				complex originalComplex(col, row);
				complex c(col, row);
				iterations = 0;
				while (std::abs(c) < 2.0 && iterations != DEPTH) {
					iterations++;
					c = originalComplex + std::pow(c, 2);
				}
				if (iterations == DEPTH) 	// If the point never escaped, draw it black
					can->drawPointColor(col, row, 0, 0, 0);
				else						// Otherwise, draw it with color
					can->drawPointColor(col, row, iterations % 151, (iterations % 131) + 50, iterations % 255);
			}
		}
	}
}

void langtonFunction(CartesianCanvas* can) {
	bool filled[WINDOW_W][WINDOW_H] = {};
	const int IPF = 500;		// Iterations per frame
	int xx = WINDOW_CW, yy = WINDOW_CH;
	int direction = UP, lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i ++) {
				if (filled[xx][yy]) {
					direction = (direction + 1) % 4;
					can->drawPointColor(xx,yy,MAX_COLOR,0,0);
				}
				else {
					direction = (direction + 3) % 4;
					can->drawPointColor(xx,yy,0,0,0);
				}
				filled[xx][yy] ^= true;
				switch(direction) {
					case UP:
						yy = yy > 0 ? yy-1 : WINDOW_H-1; break;
					case RIGHT:
						xx = xx < WINDOW_W-1 ? xx+1 : 0; break;
					case DOWN:
						yy = yy < WINDOW_H-1 ? yy+1 : 0; break;
					case LEFT:
						xx = xx > 0 ? xx-1 : WINDOW_W-1; break;
					default:
						std::cout << "BAD: dir == " << direction << std::endl;
				}
			}
		}
	}
}
void langtonFunction2(CartesianCanvas* can) {
	const int IPF = 1000;		// Iterations per frame
	const int RADIUS = 100;
	bool filled[WINDOW_H][WINDOW_H] = {};
	int xx[4],yy[4], dir[4], red[4], green[4], blue[4];
	xx[0] = WINDOW_CH-RADIUS; yy[0] = WINDOW_CH; red[0] = MAX_COLOR; 	green[0] = 0;   		blue[0] = 0;
	xx[1] = WINDOW_CH; yy[1] = WINDOW_CH-RADIUS; red[1] = 0;   			green[1] = 0;   		blue[1] = MAX_COLOR;
	xx[2] = WINDOW_CH+RADIUS; yy[2] = WINDOW_CH; red[2] = 0;   			green[2] = MAX_COLOR; 	blue[2] = 0;
	xx[3] = WINDOW_CH; yy[3] = WINDOW_CH+RADIUS; red[3] = MAX_COLOR; 	green[3] = 0;   		blue[3] = MAX_COLOR;
	for (int i = 0; i < 4; i++) { dir[i] = i; }

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
					switch(dir[j]) {
						case UP:
							yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H-1; break;
						case RIGHT:
							xx[j] = (xx[j] < WINDOW_H-1) ? xx[j] + 1 : 0; break;
						case DOWN:
							yy[j] = (yy[j] < WINDOW_H-1) ? yy[j] + 1 : 0; break;
						case LEFT:
							xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_H-1; break;
						default:
							break;
					}
				}
				for (int j = 0; j < 4; j++)
					filled[xx[j]][yy[j]] ^= true;	//Invert
			}
		}
	}
}
void langtonFunctionShiny(CartesianCanvas* can) {
	const int RADIUS = 100;
	bool filled[WINDOW_H][WINDOW_H] = {};
	int xx[4],yy[4], dir[4];
	xx[0] = WINDOW_CH-RADIUS; 	yy[0] = WINDOW_CH;
	xx[1] = WINDOW_CH; 			yy[1] = WINDOW_CH-RADIUS;
	xx[2] = WINDOW_CH+RADIUS; 	yy[2] = WINDOW_CH;
	xx[3] = WINDOW_CH; 			yy[3] = WINDOW_CH+RADIUS;
	for (int i = 0; i < 4; i++) { dir[i] = i; }

	const int IPF = 1000;		// Iterations per frame
	RGBType color;
	HSVType other;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j]][yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
						other = {((can->getFrameNumber() + 3*j)%12) / 2.0f,1.0f,1.0f};
						color = Canvas::HSVtoRGB(other);
						can->drawPointColor(xx[j],yy[j],color.R*MAX_COLOR,color.G*MAX_COLOR,color.B*MAX_COLOR,64);
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						other = {((can->getFrameNumber() + 3*j)%12) / 2.0f,1.0f,0.5f};
						color = Canvas::HSVtoRGB(other);
						can->drawPointColor(xx[j],yy[j],color.R*MAX_COLOR,color.G*MAX_COLOR,color.B*MAX_COLOR,64);
					}
					switch(dir[j]) {
						case UP:
							yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H-1; break;
						case RIGHT:
							xx[j] = (xx[j] < WINDOW_H-1) ? xx[j] + 1 : 0; break;
						case DOWN:
							yy[j] = (yy[j] < WINDOW_H-1) ? yy[j] + 1 : 0; break;
						case LEFT:
							xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_H-1; break;
						default:
							break;
					}
				}
				for (int j = 0; j < 4; j++)
					filled[xx[j]][yy[j]] ^= true;
			}
		}
	}
}

void dumbSortFunction(CartesianCanvas* can) {
	const int 	SIZE = 350,
				IPF = 100;
	int numbers[SIZE];
	int pos = 0, temp, min = 1, max = SIZE-1, lastSwap = 0, lastFrame = 0;
	bool goingUp = true;
	for (int i = 0; i < SIZE; i++)
		numbers[i] = rand() % SIZE;
	while(can->isOpen()) {
		if (lastFrame != can->getFrameNumber()) {
			for (int i = 0; i < IPF; i++) {
				if (min == max) // We are done sorting
					return;
				if (goingUp) {
					if (numbers[pos] > numbers[pos+1]) {
						temp = numbers[pos];
						numbers[pos] = numbers[pos+1];
						numbers[pos+1] = temp;
						lastSwap = pos;
					}
					if (pos >= max) {
						pos = max;
						max = (lastSwap < max) ? lastSwap : max-1;
						goingUp = !goingUp;
					} else pos++;
				} else {
					if (numbers[pos] < numbers[pos-1]) {
						temp = numbers[pos];
						numbers[pos] = numbers[pos-1];
						numbers[pos-1] = temp;
						lastSwap = pos;
					}
					if (pos <= min) {
						pos = min;
						min = (lastSwap > min) ? lastSwap : min+1;
						goingUp = !goingUp;
					} else pos--;
				}
				can->drawRectangleColor(0,0,WINDOW_W,WINDOW_H,NUM_COLORS/2,NUM_COLORS/2,NUM_COLORS/2);
				int start = 50, width = 1, height;
				for (int i = 0; i < SIZE; i++) {
					height = numbers[i];
					if (i == pos)
						can->drawRectangleColor(start,580-height,width,height,NUM_COLORS,NUM_COLORS,0);
					else
						can->drawRectangleColor(start,580-height,width,height,NUM_COLORS,0,0);
					start += width+1;
				}
			}
			lastFrame = can->getFrameNumber();
		}
	}
}

void colorWheelFunction(CartesianCanvas* can) {
	const int 	THREADS = NUM_COLORS,
				DELTA = NUM_COLORS/THREADS,
				RADIUS = 280;
	const float	GRADIENT = 2*PI/NUM_COLORS;
	RGBType col;
	float x2, x3, y2, y3, shading;
	int tid, f, lastFrame = 0;
	int start[THREADS];
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			f = lastFrame % NUM_COLORS;
			start[0] = f;
			for (int i = 1; i < THREADS; i++)
				start[i] = (start[i-1] + DELTA) % NUM_COLORS;
			#pragma omp parallel num_threads(THREADS) private(col,x2,x3,y2,y3,shading,tid)
			{
				tid = omp_get_thread_num();
				shading = tid*NUM_COLORS/THREADS;
				col = Canvas::HSVtoRGB({start[tid]*6.0f/NUM_COLORS,1.0,shading});
				x2 = WINDOW_CW + RADIUS*sin(GRADIENT*start[tid]);
				y2 = WINDOW_CH + RADIUS*cos(GRADIENT*start[tid]);
				x3 = WINDOW_CW + RADIUS*sin(GRADIENT*(start[tid]+1));
				y3 = WINDOW_CH + RADIUS*cos(GRADIENT*(start[tid]+1));
				can->drawTriangleColor(WINDOW_CW,WINDOW_CH,x2,y2,x3,y3,col.R,col.G,col.B);
			}
		}
	}
}

void functionFunction(CartesianCanvas* can) {
	Function* function1 = new CosineFunction;
	can->drawFunction(function1);

	Function* function2 = new PowerFunction(2);
	can->drawFunction(function2);

	class myFunction : public Function {
	public:
		virtual long double valueAt(long double x) const {
			return 5*pow(x,4) + 2*pow(x,3) + x + 15;
		}
	};

	Function* function3 = new myFunction;
	can->drawFunction(function3);
}
void integral1(CartesianCanvas* can) {
	const unsigned int THREADS = 1;
	long double pw = can->getPixelWidth();
	Function* function1 = new CosineFunction;
	can->drawFunction(function1);
	float offset = (can->getMaxX() - can->getMinX()) / THREADS;
	#pragma omp parallel num_threads(THREADS)
	{
		float start = can->getMinX() + omp_get_thread_num() * offset;
		float stop = start + offset;
		for (float i = start; i < stop; i += pw) {
			can->drawLineColor(i, 0, i, function1->valueAt(i), 0,0,0,MAX_COLOR);
		}
	}
}

void gradientWheelFunction(CartesianCanvas* can) {
	const int 	THREADS = NUM_COLORS,
				DELTA = NUM_COLORS/THREADS,
				CX = can->getWindowWidth()/2,
				CY = can->getWindowHeight()/2;
	const float RADIUS = 280,
				GRADIENT = 2*PI/NUM_COLORS;
	int f, lastFrame = 0;
	int start[THREADS];
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			f = lastFrame % NUM_COLORS;
			start[0] = f;
			for (int i = 1; i < THREADS; i++)					// Calculate the location and color of the
				start[i] = (start[i-1] + DELTA) % NUM_COLORS;	// shapes by the location and frame
			#pragma omp parallel num_threads(THREADS)
			{
				RGBType col[3];									// The arrays of colors for the vertices
				int xx[3],yy[3],r[3],g[3],b[3],a[3];			// Setup the arrays of values for vertices
				xx[0] = CX; yy[0] = CY;							// Set first vertex to center of screen
				int tid = omp_get_thread_num();
				float shading = 1.0f*tid/THREADS;				// Shade based on what thread this is
				col[0] = Canvas::HSVtoRGB({(start[tid]+1) / (float)MAX_COLOR * 6.0f, 0.0f, 1.0f});
				col[1] = Canvas::HSVtoRGB({ start[tid]    / (float)MAX_COLOR * 6.0f, 1.0f, 1.0f});
				col[2] = Canvas::HSVtoRGB({(start[tid]+1) / (float)MAX_COLOR * 6.0f, 1.0f, 1.0f});
				for (int i = 0; i < 3; i++) {
					r[i] = col[i].R * MAX_COLOR * shading;
					g[i] = col[i].G * MAX_COLOR * shading;
					b[i] = col[i].B * MAX_COLOR * shading;
					a[i] = MAX_COLOR;
				}
				xx[1] = CX+RADIUS * sin(GRADIENT*start[tid]);	// Add the next two vertices to
				yy[1] = CY+RADIUS * cos(GRADIENT*start[tid]);	// to around the circle
				xx[2] = CX+RADIUS * sin(GRADIENT*(start[tid]+1));
				yy[2] = CY+RADIUS * cos(GRADIENT*(start[tid]+1));
				can->drawShinyPolygon(3,xx,yy,r,g,b,a);
			}
		}
	}
}
void alphaRectangleFunction(CartesianCanvas* can) {
	int lastFrame = 0;
	int a, b;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			a = rand() % WINDOW_W;
			b = rand() % WINDOW_H;
			can->drawRectangleColor(a, b, rand() % (WINDOW_W-a), rand() % (WINDOW_H-b),
				rand() % MAX_COLOR, rand() % MAX_COLOR,rand() % MAX_COLOR,16);
		}
	}
}
void alphaLangtonFunction(CartesianCanvas* can) {
	const int	IPF = 1000,		// Iterations per frame
				RADIUS = 100;
	bool filled[WINDOW_H][WINDOW_H] = {};
	int xx[4],yy[4], dir[4], red[4], green[4], blue[4];
	xx[0] = WINDOW_CH-RADIUS; yy[0] = WINDOW_CH; red[0] = MAX_COLOR; 	green[0] = 0;   		blue[0] = 0;
	xx[1] = WINDOW_CH; yy[1] = WINDOW_CH-RADIUS; red[1] = 0;   			green[1] = 0;   		blue[1] = MAX_COLOR;
	xx[2] = WINDOW_CH+RADIUS; yy[2] = WINDOW_CH; red[2] = 0;   			green[2] = MAX_COLOR; 	blue[2] = 0;
	xx[3] = WINDOW_CH; yy[3] = WINDOW_CH+RADIUS; red[3] = MAX_COLOR; 	green[3] = 0;   		blue[3] = MAX_COLOR;
	for (int i = 0; i < 4; i++) { dir[i] = i; }
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j]][yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
						can->drawPointColor(xx[j],yy[j],MAX_COLOR/2,MAX_COLOR/2,MAX_COLOR/2,16);
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						can->drawPointColor(xx[j],yy[j],red[j],green[j],blue[j],16);
					}
				}
				for (int j = 0; j < 4; j++) {
					filled[xx[j]][yy[j]] ^= true;
					switch(dir[j]) {
						case UP:
							yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H-1; break;
						case RIGHT:
							xx[j] = (xx[j] < WINDOW_H-1) ? xx[j] + 1 : 0; break;
						case DOWN:
							yy[j] = (yy[j] < WINDOW_H-1) ? yy[j] + 1 : 0; break;
						case LEFT:
							xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_H-1; break;
						default:
							break;
					}
				}
			}
		}
		if (can->getFrameNumber() % 28 == 0)
			can->clear();
	}
}

void test(Canvas* c, void(*f)(Canvas*), bool printFPS = true, bgcolor bg = BG_NONE) {
	switch (bg) {
		case BG_BLACK:
			c->setBackgroundColor(0, 0, 0); break;
		case BG_WHITE:
			c->setBackgroundColor(255,255,255); break;
		default:
			break;
	}
	c->start();
	c->showFPS(true);
	(*f)(c);
	if (printFPS) {
		c->showFPS(false);
		print(c->getTime());
	}
	c->end();
}
void test(Cart* c, void(*f)(Cart*), bool printFPS = true, bgcolor bg = BG_NONE) {
	switch (bg) {
		case BG_BLACK:
			c->setBackgroundColor(0, 0, 0); break;
		case BG_WHITE:
			c->setBackgroundColor(255,255,255); break;
		default:
			break;
	}
	c->start();
	c->showFPS(true);
	(*f)(c);
	if (printFPS) {
		c->showFPS(false);
		print(c->getTime());
	}
	c->end();
}

int main() {
//	test(new Canvas(480800),points1);
//	test(new Canvas(480000),points2);
//	test(new Canvas(100000),lines1,true,BG_BLACK);
//	test(new Canvas(500),lines2,true,BG_BLACK);
//	test(new Canvas(250000),shadingPoints,false);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, 500000),mandelbrotFunction);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 100000),langtonFunction,false);
//	test(new Cart(0, 0, WINDOW_H, WINDOW_H, 0, 0, WINDOW_H, WINDOW_H, -1),langtonFunction2,false);
//	test(new Cart(0, 0, WINDOW_H, WINDOW_H, 0, 0, WINDOW_H, WINDOW_H, -1),langtonFunctionShiny,false);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, -1),dumbSortFunction);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 512),colorWheelFunction);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -5,-5,5,50, 10),functionFunction);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -5,-1.5,5,1.5, 16000),integral1,true,BG_WHITE);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 512),gradientWheelFunction,true,BG_BLACK);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 512),alphaRectangleFunction,false);
//	test(new Cart(0, 0, WINDOW_H, WINDOW_H, 0, 0, WINDOW_H, WINDOW_H, -1),alphaLangtonFunction,false,BG_BLACK);
}
