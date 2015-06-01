#include "Mandelbrot.h"

Mandelbrot::Mandelbrot() {
		myThreads = 8;
		myDepth = MAX_COLOR;
		myFirstX = myFirstY = mySecondX = mySecondY = 0.0;
		myRedraw = true;
}

void Mandelbrot::bindings(CartesianCanvas& can) {
		can.bindToButton(TSGL_SPACE, TSGL_PRESS, [&can, this]() {
			can.clear();
			this->myRedraw = true;
		});
		can.bindToButton(TSGL_MOUSE_LEFT, TSGL_PRESS, [&can, this]() {
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), this->myFirstX, this->myFirstY);
		});
		can.bindToButton(TSGL_MOUSE_LEFT, TSGL_RELEASE, [&can, this]() {
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), this->mySecondX, this->mySecondY);
			can.zoom(this->myFirstX, this->myFirstY, this->mySecondX, this->mySecondY);
			this->myRedraw = true;
		});
		can.bindToButton(TSGL_MOUSE_RIGHT, TSGL_PRESS, [&can, this]() {
			Decimal x, y;
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
			can.zoom(x, y, 1.5);
			this->myRedraw = true;
		});
		can.bindToScroll([&can, this](double dx, double dy) {
			Decimal x, y;
			can.getCartesianCoordinates(can.getMouseX(), can.getMouseY(), x, y);
			Decimal scale;
			if (dy == 1) scale = .5;
			else scale = 1.5;
			can.zoom(x, y, scale);
			this->myRedraw = true;
		});
	}

void Mandelbrot::draw(CartesianCanvas& can, unsigned int & numberOfThreads) {
		while(myRedraw) {
			setRedraw(false);
			can.reset();
#pragma omp parallel num_threads(myThreads)
{
	unsigned int holder = omp_get_num_threads();  //Temp variable
	unsigned int nthreads = 1;   //Actual number of threads
	if (numberOfThreads > holder || numberOfThreads == 0 || numberOfThreads < myThreads) {  //Check if the passed number of threads is valid
		nthreads = holder;  //If not, use the number of threads that we can use with OMP
	} else {
		nthreads = numberOfThreads;  //Else, use that many threads
	}
	double blocksize = can.getCartHeight() / nthreads;
	double blockheight = can.getWindowHeight() / nthreads;
	for(unsigned int k = 0; k <= blockheight && can.getIsOpen(); k++) {  // As long as we aren't trying to render off of the screen...
		long double row = blocksize * omp_get_thread_num() + can.getMinY() + can.getPixelHeight() * k;
		for(long double col = can.getMinX(); col <= can.getMaxX(); col += can.getPixelWidth()) {
			complex originalComplex(col, row);
			complex c(col, row);
			unsigned iterations = 0;
			while (std::abs(c) < 2.0 && iterations != myDepth) {  // Compute it until it escapes or we give up
				iterations++;
				c = c * c + originalComplex;
			}
			if(iterations == myDepth) { // If the point never escaped, draw it black
				can.drawPoint(col, row, BLACK);
			} else { // Otherwise, draw it with color based on how long it took
				can.drawPoint(col, row,
						ColorInt(iterations % 151,
								((iterations % 131) + omp_get_thread_num() * 128 / nthreads) % 255,
								iterations % 255));
			}
			if (myRedraw) break;
		}
		if (myRedraw) break;
	}
}
std::cout << can.getTime() << std::endl;
while (can.getIsOpen() && !myRedraw)
	can.sleep(); //Removed the timer and replaced it with an internal timer in the Canvas class
		}

	}

	//mutator
void Mandelbrot::setRedraw(bool newValue) {
  myRedraw = newValue;
}
