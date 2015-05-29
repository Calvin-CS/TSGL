/*
 * GradientMandelbrot.cpp
 *
 *  Created on: May 28, 2015
 *      Author: Chris Dilley
 */

#include "GradientMandelbrot.h"

GradientMandelbrot::GradientMandelbrot() {
	myThreads = 32;
	myDepth = 32;
	myRedraw = true;
}

void GradientMandelbrot::draw(CartesianCanvas& can, unsigned int & numberOfThreads) {
	while (myRedraw) {
		setRedraw(false);
#pragma omp parallel num_threads(myThreads)
		{
			unsigned int holder = omp_get_num_threads();  //Temp variable
			unsigned int nthreads = 1;   //Actual number of threads
			if (numberOfThreads > holder || numberOfThreads == 0) {  //Check if the passed number of threads is valid
				nthreads = holder;  //If not, use the number of threads that we can use with OMP
			} else {
				nthreads = numberOfThreads;  //Else, use that many threads
			}
			double blockstart = can.getCartHeight() / nthreads;
			unsigned int iterations;
			double smooth;
			for (unsigned int k = 0; k <= (can.getWindowHeight() / nthreads) && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
				long double row = blockstart * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
				for (long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
					complex c(col, row);
					complex z(col, row);
					smooth = exp(-std::abs(z));
					iterations = 0;
					while (std::abs(z) < 2.0l && iterations != myDepth) {
						iterations++;
						z = z * z + c;
						smooth += exp(-std::abs(z));
					}
					int i;
					for (i = 0; i < 2; i++) {
						iterations++;
						z = z * z + c;
						smooth += exp(-std::abs(z));
					}
					smooth /= (myDepth + i + 1);
					can.drawPoint(col, row, ColorHSV((float) smooth * 6.0f, 1.0f, 1.0f, 1.0f));
					if (myRedraw) break;
				}
				if (myRedraw) break;
		 }
	 }
		while (can.getIsOpen() && !myRedraw)
			can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
	}

}

void GradientMandelbrot::setRedraw(bool newValue) {
		myRedraw = newValue;
}



