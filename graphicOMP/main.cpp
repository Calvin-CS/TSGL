//
// Test driver for the graphicOMP library
//
// Last Modified: Patrick Crain, 6/4/2014

#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"
#include "FL/Fl_Shared_Image.H"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Box.H"

#include "Canvas.h"
#include <iostream>
#include <omp.h>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

int main() {
	Canvas *can = new Canvas();
	int tid, nthreads, i, j, color;
	#pragma omp parallel num_threads(omp_get_num_procs()) private(tid,nthreads,i,j,color)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
//		for (i = tid; i < WINDOW_W; i+= nthreads) {
//			for (int j = 0; j <= WINDOW_H; j++) {
//				color = i*128/WINDOW_W + j*128/WINDOW_H;
//				//Fl::lock();
//				can->drawPointColor(i,j,color,color,color);
//				//Fl::unlock();
//			}
//		}
		for (i = 100+tid; i < WINDOW_W-100; i+= nthreads) {
			for (j = 100; j < WINDOW_H-100; j++) {
				can->setColor(80,10,160);
				if (i % 2 == 0)
					can->drawPoint(i,j);
				else
					can->drawPointColor(i,j,i,j,i*j % 256);
			}
		}
	}
	return (can->start());
}
