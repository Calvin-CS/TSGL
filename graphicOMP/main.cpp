/*
 * main.cpp provides example usage for the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/17/2014
 */

#include "Canvas.h"
#include "CartesianCanvas.h"
#include <omp.h>
#include <iostream>
#include <cmath>
#include <complex>

const int 	NUM_COLORS = 256,
			MAX_COLOR = 255;

// Shared values between langton functions
enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };
// Background colors for the testing functions
enum bgcolor { BG_NONE = -1, BG_BLACK = 0, BG_WHITE = 1 };

typedef CartesianCanvas Cart;
typedef std::complex<long double> complex;

static void print(const double d) {
	std::cout << d << std::endl << std::flush;
}

void points1(Canvas* can) {
	int nthreads = omp_get_num_threads();
	#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int color;
		for (int i = omp_get_thread_num(); i < can->getWindowWidth(); i+= nthreads) {
			for (int j = 0; j <= can->getWindowHeight(); j++) {
				color = i*MAX_COLOR/2/can->getWindowWidth() + j*MAX_COLOR/2/can->getWindowHeight();
				can->drawPointColor(i,j,RGBintToRGBfloat(color,color,color));
			}
		}
	}
}
void points2(Canvas* can) {
	int myPart = can->getWindowHeight() / omp_get_num_threads();
	#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int myStart = myPart * omp_get_thread_num();
		for (int j = myStart; j < myStart + myPart; j++) {
			for (int i = 100; i < can->getWindowWidth()-100; i++) {
				if (i % 2 == 0)
					can->drawPointColor(i,j,RGBintToRGBfloat(j % NUM_COLORS,i % NUM_COLORS,i*j % 113));
				else
					can->drawPointColor(i,j,RGBintToRGBfloat(i % NUM_COLORS,j % NUM_COLORS,i*j % NUM_COLORS));
			}
		}
	}
}

void lines1(Canvas* can) {
	int xOld,yOld,xNew = can->getWindowWidth()/2,yNew = can->getWindowHeight()/2,red,green,blue;
	int lastFrame = 0;
	while(can->isOpen()) {									// Checks to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {			// One line a frame
			lastFrame = can->getFrameNumber();
			xOld  = xNew;
			yOld  = yNew;
			xNew  = rand() % can->getWindowWidth();
			yNew  = rand() % can->getWindowHeight();
			red   = rand() % NUM_COLORS;
			green = rand() % NUM_COLORS;
			blue  = rand() % NUM_COLORS;
			can->drawLineColor(xOld,yOld,xNew,yNew,RGBintToRGBfloat(red,green,blue));
		}
	}
}
void lines2(Canvas* can) {
	const double 	RAD = M_PI/180,			// One radian in degrees
					ARC = 7.11;				// Arc length
	int lastFrame = 0;
	#pragma omp parallel num_threads(omp_get_num_procs())
	{
		int a,b,c,d,red,green,blue;
		double angle, offset = omp_get_thread_num() * ARC;
		bool reverse = false;
		while(can->isOpen()) {				// Checks to see if the window has been closed
			if (can->getFrameNumber() > lastFrame) {
				lastFrame = can->getFrameNumber();
				angle = offset+lastFrame*RAD;
				a = can->getWindowWidth()/2 * (1 + sin(angle));
				b = can->getWindowHeight()/2 * (1 + cos(angle));
				c = can->getWindowWidth()/2 * (1 - sin(angle));
				d = can->getWindowHeight()/2 * (1 - cos(angle));
				red = (a + lastFrame) % NUM_COLORS;
				green = (b + lastFrame) % NUM_COLORS;
				blue = (a*b + lastFrame) % NUM_COLORS;
				if (!reverse)
					can->drawLineColor(a,b,c,d,RGBintToRGBfloat(red,green,blue));
				else
					can->drawLineColor(c,d,a,b,RGBintToRGBfloat(red,green,blue));
				reverse = !reverse;
			}
		}
	}
}
void shadingPoints(Canvas* can) {
	int lastFrame = 0, nthreads = omp_get_num_procs();
	#pragma omp parallel num_threads(nthreads)
	{
		while(can->isOpen()) {							// Check to see if the window has been closed
			if (can->getFrameNumber() > lastFrame) {
				lastFrame = can->getFrameNumber();
				for (int i = omp_get_thread_num(); i < NUM_COLORS; i+= nthreads)
					for (int j = 0; j < NUM_COLORS; j++)
						can->drawPointColor(i,j,RGBintToRGBfloat(i,j,lastFrame % NUM_COLORS));
			}
		}
	}
}

void mandelbrotFunction(CartesianCanvas* can) {
	const unsigned int THREADS = 32;  //omp_get_num_procs();
	const unsigned int DEPTH = MAX_COLOR;
	can->setCanZoom(true);
	while(can->getZoomed()) {
		can->setZoomed(false);
		double blockstart = can->getCartHeight() / THREADS;
		#pragma omp parallel num_threads(THREADS)
		{
			unsigned int iterations;
			for (int k = 0; k <= (can->getWindowHeight() / THREADS) && can->isOpen(); k++) { // As long as we aren't trying to render off of the screen...
				long double row = blockstart * omp_get_thread_num() + can->getMinY() + can->getPixelHeight() * k;
				for (long double col = can->getMinX(); col <= can->getMaxX(); col += can->getPixelWidth()) {
					complex originalComplex(col, row);
					complex c(col, row);
					iterations = 0;
					while (std::abs(c) < 2.0 && iterations != DEPTH) {		// Computer until it escapes or we give up
						iterations++;
						c = c * c + originalComplex;
					}
					if (iterations == DEPTH) 	// If the point never escaped, draw it black
						can->drawPointColor(col, row, {0, 0, 0, 1.0});
					else						// Otherwise, draw it with color based on how long it took
						can->drawPointColor(col, row, RGBintToRGBfloat(iterations % 151, (iterations % 131) + 50, iterations % 255));
					if (can->getZoomed())
						break;
				}
				if (can->getZoomed())
					break;
			}
		}
		while(can->isOpen() && !can->getZoomed()) {
			//busy wait
		}
	}
}

void langtonFunction(CartesianCanvas* can) {
	const int	WINDOW_W = can->getCartWidth(),			// Set the screen sizes
				WINDOW_H = can->getCartHeight();
	bool* filled = new bool[WINDOW_W * WINDOW_H]();		// Create an empty bitmap for the window
	const int IPF = 500;								// Iterations per frame
	int xx = WINDOW_W / 2,								// Start at the center
		yy = WINDOW_H / 2;
	int direction = UP, lastFrame = 0;
	while(can->isOpen()) {								// Check to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i ++) {
				if (filled[xx + WINDOW_W * yy]) {
					direction = (direction + 1) % 4;							// Turn right
					can->drawPointColor(xx,yy,RGBintToRGBfloat(MAX_COLOR,0,0));	// Color it
				}
				else {
					direction = (direction + 3) % 4;							// Turn left
					can->drawPointColor(xx,yy,{0,0,0,1});						// Don't color it
				}
				filled[xx + WINDOW_W * yy] ^= true;								// Invert the square
				switch(direction) {												// Check for wrap-around and move
					case UP:
						yy = yy > 0 ? yy-1 : WINDOW_H-1; break;
					case RIGHT:
						xx = xx < WINDOW_W-1 ? xx+1 : 0; break;
					case DOWN:
						yy = yy < WINDOW_H-1 ? yy+1 : 0; break;
					case LEFT:
						xx = xx > 0 ? xx-1 : WINDOW_W-1; break;
					default:
						std::cout << "BAD: dir == " << direction << std::endl; break;
				}
			}
		}
	}
}
void langtonFunction2(CartesianCanvas* can) {
	const int 	IPF = 1000,								// Iterations per frame
				WINDOW_W = can->getCartWidth(),			// Set the window sizes
				WINDOW_H = can->getCartHeight(),
				RADIUS = WINDOW_H/6;					// How far apart to space the ants
	bool* filled = new bool[WINDOW_W * WINDOW_H]();		// Create an empty bitmap for the window
	int xx[4],yy[4], dir[4], red[4], green[4], blue[4];
	xx[0] = WINDOW_W/2-RADIUS; yy[0] = WINDOW_H/2; red[0] = MAX_COLOR; 	green[0] = 0;   		blue[0] = 0;
	xx[1] = WINDOW_W/2; yy[1] = WINDOW_H/2-RADIUS; red[1] = 0;   		green[1] = 0;   		blue[1] = MAX_COLOR;
	xx[2] = WINDOW_W/2+RADIUS; yy[2] = WINDOW_H/2; red[2] = 0;   		green[2] = MAX_COLOR; 	blue[2] = 0;
	xx[3] = WINDOW_W/2; yy[3] = WINDOW_H/2+RADIUS; red[3] = MAX_COLOR; 	green[3] = 0;   		blue[3] = MAX_COLOR;
	for (int i = 0; i < 4; i++) { dir[i] = i; }

	int lastFrame = 0;
	while(can->isOpen()) {								// Check to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j] + WINDOW_W * yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
						can->drawPointColor(xx[j],yy[j],RGBintToRGBfloat(red[j],green[j],blue[j]));
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						can->drawPointColor(xx[j],yy[j],RGBintToRGBfloat(red[j]/2,green[j]/2,blue[j]/2));
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
					filled[xx[j] + WINDOW_W * yy[j]] ^= true;		//Invert the squares the ants are on
			}
		}
	}
}
void langtonFunctionShiny(CartesianCanvas* can) {
	const int 	IPF = 1000,								// Iterations per frame
				WINDOW_W = can->getCartWidth(),			// Set the window sizes
				WINDOW_H = can->getCartHeight(),
				RADIUS = WINDOW_H/6;					// How far apart to space the ants
	bool* filled = new bool[WINDOW_W * WINDOW_H]();		// Create an empty bitmap for the window
	int xx[4], yy[4], dir[4];
	xx[0] = WINDOW_W/2-RADIUS; 	yy[0] = WINDOW_H/2;
	xx[1] = WINDOW_W/2; 		yy[1] = WINDOW_H/2-RADIUS;
	xx[2] = WINDOW_W/2+RADIUS; 	yy[2] = WINDOW_H/2;
	xx[3] = WINDOW_W/2; 		yy[3] = WINDOW_H/2+RADIUS;
	for (int i = 0; i < 4; i++) { dir[i] = i; }

	RGBfloatType color;
	int lastFrame = 0;
	while(can->isOpen()) {
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j] + WINDOW_W * yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
						color = HSVToRGBfloat((can->getFrameNumber() + 3*j)%12 / 2.0f, 1.0f, 1.0f, .25f);
						can->drawPointColor(xx[j],yy[j],color);
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						color = HSVToRGBfloat((can->getFrameNumber() + 3*j)%12 / 2.0f, 1.0f, 0.5f, .25f);
						can->drawPointColor(xx[j],yy[j],color);
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
					filled[xx[j] + WINDOW_W * yy[j]] ^= true;		//Invert the squares the ants are on
			}
		}
	}
}

void dumbSortFunction(Canvas* can) {
	const int 	SIZE = 350,				// Size of the data pool
				IPF = 100;				// Iterations per frame
	int numbers[SIZE];					// Array to store the data
	int pos = 0, temp, min = 1, max = SIZE-1, lastSwap = 0, lastFrame = 0;
	bool goingUp = true;
	for (int i = 0; i < SIZE; i++)
		numbers[i] = rand() % SIZE;
	while(can->isOpen()) {				// Check to see if the window has been closed
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
				can->drawRectangleColor(0,0,can->getWindowWidth(),can->getWindowHeight(),
										RGBintToRGBfloat(MAX_COLOR/2,MAX_COLOR/2,MAX_COLOR/2));
				int start = 50, width = 1, height;
				for (int i = 0; i < SIZE; i++) {
					height = numbers[i];
					if (i == pos)
						can->drawRectangleColor(start,580-height,width,height,
												RGBintToRGBfloat(MAX_COLOR,MAX_COLOR,0));
					else
						can->drawRectangleColor(start,580-height,width,height,
												RGBintToRGBfloat(MAX_COLOR,0,0));
					start += width+1;
				}
			}
			lastFrame = can->getFrameNumber();
		}
	}
}

void colorWheelFunction(CartesianCanvas* can) {
	const int 	THREADS = NUM_COLORS,					// Number of threads to compute with
				DELTA = NUM_COLORS/THREADS,				// Distance between threads to compute
				WINDOW_CW = can->getCartWidth() / 2,	// Set the center of the window
				WINDOW_CH = can->getCartHeight() / 2;
	const float RADIUS = (WINDOW_CH < WINDOW_CW ? WINDOW_CH : WINDOW_CW) * .95,		// Radius of wheel
				GRADIENT = 2*M_PI/NUM_COLORS;
	RGBfloatType color;
	float x2, x3, y2, y3, shading;
	int tid, f, lastFrame = 0;
	int start[THREADS];
	while(can->isOpen()) {								// Check to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			f = lastFrame % NUM_COLORS;
			start[0] = f;
			for (int i = 1; i < THREADS; i++)			// Spread out the threads starting position
				start[i] = (start[i-1] + DELTA) % NUM_COLORS;
			#pragma omp parallel num_threads(THREADS) private(color,x2,x3,y2,y3,shading,tid)
			{
				tid = omp_get_thread_num();
				shading = (float)tid/THREADS;
				color = HSVToRGBfloat(start[tid]*6.0f/NUM_COLORS,1.0,shading);
				x2 = WINDOW_CW + RADIUS*sin(GRADIENT*start[tid]);
				y2 = WINDOW_CH + RADIUS*cos(GRADIENT*start[tid]);
				x3 = WINDOW_CW + RADIUS*sin(GRADIENT*(start[tid]+1));
				y3 = WINDOW_CH + RADIUS*cos(GRADIENT*(start[tid]+1));
				can->drawTriangleColor(WINDOW_CW,WINDOW_CH,x2,y2,x3,y3,color);
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
	const unsigned int THREADS = 8;
	long double pw = can->getPixelWidth();
	Function* function1 = new CosineFunction;
	can->drawFunction(function1);
	long double offset = can->getCartWidth() / THREADS;
	#pragma omp parallel num_threads(THREADS)
	{
		long double start = can->getMinX() + omp_get_thread_num() * offset;
		long double stop = start + offset;
		for (long double i = start; i < stop; i += pw) {
			can->drawLineColor(i, 0, i, function1->valueAt(i), {(float)omp_get_thread_num()/THREADS,0,0,1.0});
		}
	}
}

void gradientWheelFunction(CartesianCanvas* can) {
	const int 	THREADS = NUM_COLORS,							// Number of threads to compute with
				DELTA = NUM_COLORS/THREADS,						// Distance between threads to compute
				WINDOW_CW = can->getCartWidth() / 2,			// Center of the screen
				WINDOW_CH = can->getCartHeight() / 2;
	const float RADIUS = (WINDOW_CH < WINDOW_CW ? WINDOW_CH : WINDOW_CW) * .95,		// Radius of wheel
				GRADIENT = 2*M_PI/NUM_COLORS;
	int f, lastFrame = 0;
	int start[THREADS];
	while(can->isOpen()) {										// Check to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			f = lastFrame % MAX_COLOR;
			start[0] = f;
			for (int i = 1; i < THREADS; i++)					// Calculate the location and color of the
				start[i] = (start[i-1] + DELTA) % NUM_COLORS;	// shapes by the location and frame
			#pragma omp parallel num_threads(THREADS)
			{
				RGBfloatType color[3];							// The arrays of colors for the vertices
				int xx[3],yy[3];								// Setup the arrays of values for vertices
				xx[0] = WINDOW_CW; yy[0] = WINDOW_CH;			// Set first vertex to center of screen
				int tid = omp_get_thread_num();
				float shading = (float)tid/THREADS;				// Shade based on what thread this is
				color[0] = HSVToRGBfloat((start[(tid+1)%THREADS]) / (float)MAX_COLOR * 6.0f, 0.0f, 1.0f, 1.0f);
				color[1] = HSVToRGBfloat( start[ tid           ]  / (float)MAX_COLOR * 6.0f, 1.0f, 1.0f, 1.0f);
				color[2] = HSVToRGBfloat((start[(tid+1)%THREADS]) / (float)MAX_COLOR * 6.0f, 1.0f, 1.0f, 1.0f);
				for (int i = 0; i < 3; i++) {
					color[i].R *= shading;
					color[i].G *= shading;
					color[i].B *= shading;
					color[i].A = 1.0;
				}
				xx[1] = WINDOW_CW+RADIUS * sin(GRADIENT*start[tid]);	// Add the next two vertices to
				yy[1] = WINDOW_CH+RADIUS * cos(GRADIENT*start[tid]);	//	to around the circle
				xx[2] = WINDOW_CW+RADIUS * sin(GRADIENT*(start[tid]+1));
				yy[2] = WINDOW_CH+RADIUS * cos(GRADIENT*(start[tid]+1));
				can->drawShinyPolygon(3,xx,yy,color);
			}
		}
	}
}
void alphaRectangleFunction(CartesianCanvas* can) {
	const int 	WINDOW_W = can->getCartWidth(),			// Set the center of the window
				WINDOW_H = can->getCartHeight();
	int lastFrame = 0;
	int a, b;
	while(can->isOpen()) {								// Check to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			a = rand() % WINDOW_W;
			b = rand() % WINDOW_H;
			can->drawRectangleColor(a, b, rand() % (WINDOW_W-a), rand() % (WINDOW_H-b),
						RGBintToRGBfloat(rand() % MAX_COLOR, rand() % MAX_COLOR,rand() % MAX_COLOR,16));
		}
	}
}
void alphaLangtonFunction(CartesianCanvas* can) {
	const int 	IPF = 5000,								// Iterations per frame
				WINDOW_W = can->getCartWidth(),			// Set the window sizes
				WINDOW_H = can->getCartHeight(),
				RADIUS = WINDOW_H/6;					// How far apart to space the ants
	bool* filled = new bool[WINDOW_W * WINDOW_H]();		// Create an empty bitmap for the window
	int xx[4],yy[4], dir[4], red[4], green[4], blue[4];
	xx[0] = WINDOW_W/2-RADIUS; yy[0] = WINDOW_H/2;	red[0] = MAX_COLOR; 	green[0] = 0;   		blue[0] = 0;
	xx[1] = WINDOW_W/2; yy[1] = WINDOW_H/2-RADIUS; 	red[1] = 0;   			green[1] = 0;   		blue[1] = MAX_COLOR;
	xx[2] = WINDOW_W/2+RADIUS; yy[2] = WINDOW_H/2;	red[2] = 0;   			green[2] = MAX_COLOR; 	blue[2] = 0;
	xx[3] = WINDOW_W/2; yy[3] = WINDOW_H/2+RADIUS;	red[3] = MAX_COLOR; 	green[3] = 0;   		blue[3] = MAX_COLOR;
	for (int i = 0; i < 4; i++) { dir[i] = i; }
	int lastFrame = 0;
	while(can->isOpen()) {								// Check to see if the window has been closed
		if (can->getFrameNumber() > lastFrame) {
			lastFrame = can->getFrameNumber();
			for (int i = 0; i < IPF; i++) {
				for (int j = 0; j < 4; j++) {
					if (filled[xx[j] + WINDOW_W * yy[j]]) {
						dir[j] = (dir[j] + 1) % 4;
//						can->drawPointColor(xx[j],yy[j],RGBintToRGBfloat(0,0,0,255));
						can->drawPointColor(xx[j],yy[j],RGBintToRGBfloat(MAX_COLOR/2,MAX_COLOR/2,MAX_COLOR/2,16));
					}
					else {
						dir[j] = (dir[j] + 3) % 4;
						can->drawPointColor(xx[j],yy[j],RGBintToRGBfloat(red[j],green[j],blue[j],16));
					}
				}
				for (int j = 0; j < 4; j++) {
					filled[xx[j] + WINDOW_W * yy[j]] ^= true;
					switch(dir[j]) {
						case UP:
							yy[j] = (yy[j] > 0) ? yy[j] - 1 : WINDOW_H-1; break;
						case RIGHT:
							xx[j] = (xx[j] < WINDOW_W-1) ? xx[j] + 1 : 0; break;
						case DOWN:
							yy[j] = (yy[j] < WINDOW_H-1) ? yy[j] + 1 : 0; break;
						case LEFT:
							xx[j] = (xx[j] > 0) ? xx[j] - 1 : WINDOW_W-1; break;
						default:
							break;
					}
				}
			}
		}
		if (can->getFrameNumber() % 28 == 0)			// Clear the screen every period
			can->clear();
	}
}
void mandelbrot2Function(CartesianCanvas* can) {
	const unsigned int THREADS = 32;
	const unsigned int DEPTH = 32;
	can->setCanZoom(true);
	while(can->getZoomed()) {
		can->setZoomed(false);
		double blockstart = can->getCartHeight() / THREADS;
		#pragma omp parallel num_threads(THREADS)
		{
			unsigned int iterations;
			double smooth;
			for (int k = 0; k <= (can->getWindowHeight() / THREADS) && can->isOpen(); k++) { // As long as we aren't trying to render off of the screen...
				long double row = blockstart * omp_get_thread_num() + can->getMinY() + can->getPixelHeight() * k;
				for (long double col = can->getMinX(); col <= can->getMaxX(); col += can->getPixelWidth()) {
					complex c(col, row);
					complex z(col, row);
					smooth = exp(-std::abs(z));
					iterations = 0;
					while (std::abs(z) < 2.0l && iterations != DEPTH) {
						iterations++;
						z = z * z + c;
						smooth += exp(-std::abs(z));
					}
					int i;
					for(i = 0; i < 2; i++) {
						iterations++;
						z = z * z + c;
						smooth += exp(-std::abs(z));
					}
					smooth /= (DEPTH + i+1);
					RGBfloatType color = HSVToRGBfloat((float)smooth*6.0f, 1.0, 1.0, 1.0);
					can->drawPointColor(col, row, color);
					if (can->getZoomed())
						break;
				}
				if (can->getZoomed())
					break;
			}
		}
		while(can->isOpen() && !can->getZoomed()) {
			//busy wait
		}
	}
}

void novaFunction(CartesianCanvas* can) {
	const unsigned int THREADS = 32;
	const unsigned int DEPTH = 200;
	const double BLOCKSTART = (can->getMaxY() - can->getMinY()) / THREADS;
	const long double 	R = 1.0l;
	#pragma omp parallel num_threads(THREADS)
	{
		unsigned int iterations;
		double smooth;
		for (int k = 0; k <= (can->getWindowHeight() / THREADS) && can->isOpen(); k++) { // As long as we aren't trying to render off of the screen...
			long double row = BLOCKSTART * omp_get_thread_num() + can->getMinY() + can->getPixelHeight() * k;
			for (long double col = can->getMinX(); col <= can->getMaxX(); col += can->getPixelWidth()) {
				complex c(col, row);
				complex z(1, 0);
				smooth = exp(-std::abs(z));
				complex n, d, c1;
				complex r(1,0);
				iterations = 0;
				while (std::abs(z) < 2.0l && iterations != DEPTH) {
					iterations++;
					n = z*z*z - 1.0l;
					d = z*z * 3.0l;
					z = z + c -(R*n/d);
					smooth += exp(-std::abs(z));
				}
				for(int i = 0; i < 20; i++) {
					iterations++;
					n = z*z*z - 1.0l;
					d = z*z * 3.0l;
					z = z + c -(R*n/d);
					smooth += exp(-std::abs(z));
				}
				smooth /= DEPTH;
				if (smooth != smooth || smooth < 0)			// Check to see if smooth is NAN
					smooth = 0;
				while (smooth > 1)
					smooth -= 1;
				RGBfloatType color = HSVToRGBfloat((float)smooth*6.0f,1.0,(float)smooth, 1.0);
				can->drawPointColor(col, row, color);
			}
		}
	}
}

void test(Canvas* c, void(*f)(Canvas*), bool printFPS = false, bgcolor bg = BG_NONE) {
	switch (bg) {
		case BG_BLACK:
			c->setBackgroundColor({0, 0, 0, 1.0}); break;
		case BG_WHITE:
			c->setBackgroundColor({1.0,1.0,1.0,1.0}); break;
		default:
			break;
	}
	c->start();
	if (printFPS)
		c->showFPS(true);
	(*f)(c);
	if (printFPS) {
		c->showFPS(false);
		print(c->getTime());
	}
	c->end();
}
void test(Cart* c, void(*f)(Cart*), bool printFPS = false, bgcolor bg = BG_NONE) {
	switch (bg) {
		case BG_BLACK:
			c->setBackgroundColor({0, 0, 0, 1.0}); break;
		case BG_WHITE:
			c->setBackgroundColor({1.0,1.0,1.0,1.0}); break;
		default:
			break;
	}
	c->start();
	if (printFPS)
		c->showFPS(true);
	(*f)(c);
	if (printFPS) {
		c->showFPS(false);
		print(c->getTime());
	}
	c->end();
}

const int 	WINDOW_X = 200,
			WINDOW_Y = 200,
			WINDOW_W = 800,
			WINDOW_H = 600;
int main() {
//	test(new Canvas(480800),points1,true);
//	test(new Canvas(480000),points2,true);
//	test(new Canvas(100000),lines1,true,BG_BLACK);
//	test(new Canvas(500),lines2,false,BG_BLACK);
//	test(new Canvas(250000),shadingPoints,false);
	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, 500000),mandelbrotFunction,true);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 100000),langtonFunction,false);
//	test(new Cart(0, 0, WINDOW_H, WINDOW_H, 0, 0, WINDOW_H, WINDOW_H, -1),langtonFunction2,false);
//	test(new Cart(0, 0, WINDOW_H, WINDOW_H, 0, 0, WINDOW_H, WINDOW_H, -1),langtonFunctionShiny,true,BG_BLACK);
//	test(new Canvas(0, 0, WINDOW_W, WINDOW_H, -1),dumbSortFunction,true);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 512),colorWheelFunction);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -5,-5,5,50, 10),functionFunction,true,BG_WHITE);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -5,-1.5,5,1.5, 16000),integral1,true,BG_WHITE);
//	test(new Cart(0, 0, 1000, 1000, 0, 0, 1000, 1000, 512),gradientWheelFunction,false,BG_BLACK);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, 0, 0, WINDOW_W, WINDOW_H, 512),alphaRectangleFunction,false,BG_BLACK);
//	test(new Cart(0, 0, 900, 900, 0, 0, 900, 900, -1),alphaLangtonFunction,true,BG_BLACK);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -2, -1.125, 1, 1.125, 500000),mandelbrot2Function,true);
//	test(new Cart(0, 0, WINDOW_W, WINDOW_H, -1, -0.5, 0, 0.5, 500000),novaFunction,true);
}
