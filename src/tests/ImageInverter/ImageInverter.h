/*
 * ImageInverter.h declares the ImageInverter class.
 *
 *  Created on: Nov 25, 2014
 *      Author: adams
 */

#ifndef IMAGEINVERTER_H_
#define IMAGEINVERTER_H_

#include <tsgl.h>     // Canvas, ColorInt, etc.
#include <omp.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>    // sleep()
#endif

using namespace tsgl;

class ImageInverter {
private:
  Canvas myCanvas1;
  Canvas myCanvas2;
  unsigned myWidth;
  unsigned myHeight;
  std::string   myFileName;
  #ifdef _WIN32
  void sleep(unsigned seconds) { Sleep(seconds * 1000); }
  #endif
protected:
  /* invertImage inverts the image using a given number of threads
   * @param: numThreads, the number of threads to use
   *           when inverting the image (default 1).
   * Postcondition: myCanvas2 contains an image that is the
   *                 inverse of the image in myCanvas1.
   */
  virtual void invertImage(unsigned numThreads = 1);

  /* helper method to keep Canvases up until the user
   *   clicks their window-frame's close button.
   *
   * Postcondition: myCanvas1 and myCanvas2 have been closed.
   */
  virtual void stop();
public:
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
  ImageInverter(const std::string& fileName, unsigned width, unsigned height);

  /* destructor
   * Postcondition: myCanvas1 and myCanvas2 have been closed
   *              && myCanvas1 and myCanvas2 have been deallocated
   *              && glfwTerminate() has shut down GLFW
   *              && a termination message was written to the console.
   */
  virtual ~ImageInverter();

  /* method to coordinate the image inversion
   * @param: numThreads, the number of threads to use
   *           when inverting the image (default 1).
   * PostCondition: myCanvas2 contains the inverse
   *           of the image in myCanvas1.
   */
  virtual void run(unsigned numThreads = 1);
};

#endif /* IMAGEINVERTER_H_ */
