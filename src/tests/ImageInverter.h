/*
 * ImageInverter.h declares the ImageInverter class.
 *
 *  Created on: Nov 25, 2014
 *      Author: adams
 */

#ifndef IMAGEINVERTER_H_
#define IMAGEINVERTER_H_

#include <tsgl.h>     // Canvas, ColorInt, etc.
#include <iostream>   // cout, string, ...
#include <unistd.h>    // sleep()
using namespace std;

class ImageInverter {
public:
	ImageInverter(const string& fileName, unsigned width, unsigned height);
	virtual ~ImageInverter();
	virtual void run(unsigned numThreads = 1);

protected:
	virtual void invertImage(unsigned numThreads = 1);
	virtual void stop();

private:
	Canvas myCanvas1;
	Canvas myCanvas2;
	unsigned myWidth;
	unsigned myHeight;
	string   myFileName;
};

//-----------------  Method definitions ------------------------------

/* explicit constructor
 * @param fileName, a string.
 * @param width, an unsigned.
 * @param height, an unsigned.
 * Precondition: fileName contains the name of a valid image file
 *               && width contains the number of columns in that image
 *               && height contains the number of rows in the image.
 * Postcondition: myCanvas1 contains the image from fileName
 *               && myCanvas2 is ready for drawing its inverse
 *               && myWidth = width
 *               && myHeight = height.
 */

inline
ImageInverter::ImageInverter(const string& fileName,
		                      unsigned width, unsigned height)
 : myCanvas1(0, 0, width, height, 5000000, fileName),
   myCanvas2(0, 0, width, height, 5000000, fileName),
   myWidth(width), myHeight(height), myFileName(fileName)
{
	myCanvas1.start();
	myCanvas1.drawImage(fileName, 0, 0, width, height);
	sleep(1);
	myCanvas2.start();
}

/* method to coordinate the image inversion
 * @param: numThreads, the number of threads to use
 *           when inverting the image (default 1).
 * PostCondition: myCanvas2 contains the inverse
 *           of the image in myCanvas1.
 */
inline
void ImageInverter::run(unsigned numThreads) {
	sleep(1);
	invertImage(numThreads);
	stop();
}


/* invertImage inverts the image using a given number of threads
 * @param: numThreads, the number of threads to use
 *           when inverting the image (default 1).
 * Postcondition: myCanvas2 contains an image that is the
 *                 inverse of the image in myCanvas1.
 */

inline
void ImageInverter::invertImage(unsigned numThreads) {
	int inverse = 0;
	ColorInt pixelColor;
	#pragma omp parallel for num_threads(numThreads)
	for (unsigned row = 0; row < myHeight; row++) {
		Timer t(0.00001);
		for (unsigned col = 0; col < myWidth; col++) {
			pixelColor = myCanvas1.getPixel(col, row);
			inverse = (pixelColor.R + pixelColor.G + pixelColor.B) / 3;
			myCanvas2.drawPoint(col, row, ColorInt(inverse, inverse, inverse) );
			t.sleep();
		}
	}
}

/* helper method to keep Canvases up until the user
 *   clicks their window-frame's close button.
 *
 * Postcondition: myCanvas1 and myCanvas2 have been closed.
 */
inline
void ImageInverter::stop() {
	myCanvas1.close();
	myCanvas2.close();
}


/* destructor
 * Postcondition: myCanvas1 and myCanvas2 have been closed
 *              && myCanvas1 and myCanvas2 have been deallocated
 *              && glfwTerminate() has shut down GLFW
 *              && a termination message was written to the console.
 */

inline
ImageInverter::~ImageInverter() {
//	delete myCanvas1;
//	delete myCanvas2;
	cout << "\nImageInverter terminated normally.\n" << flush;
}


#endif /* IMAGEINVERTER_H_ */
