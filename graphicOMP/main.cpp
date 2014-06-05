// main.cpp provides example usage for the graphicOMP library
//
// Last Modified: Patrick Crain, 6/5/2014

#include "Canvas.h"
#include <omp.h>

const int WINDOW_W = 800, WINDOW_H = 600;
Canvas *can, *can2;

void updateFunction(Canvas* can) {
	int tid, nthreads, i, j, color;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j,color)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		for (i = tid; i < WINDOW_W; i+= nthreads) {
			for (int j = 0; j <= WINDOW_H; j++) {
				color = i*128/WINDOW_W + j*128/WINDOW_H;
				can->drawPointColor(i,j,color,color,color);
			}
		}
	}
}
void updateFunction2(Canvas* can) {
	int tid, nthreads, i, j, color;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j,color)
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

int main() {
//	#pragma omp parallel sections
//	{
//		XInitThreads();
//		#pragma omp section
//		{
			can = new Canvas(updateFunction);
			can->start();
//		}
//		#pragma omp section
//		{
			can2 = new Canvas(updateFunction2);
			can2->start();
//		}
//	}
}
