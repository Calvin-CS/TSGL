/*
 * Util.h contains commonly used constants and typedefs as well as some utility functions.
 */

#ifndef SRC_TSGL_UTIL_H_
#define SRC_TSGL_UTIL_H_

#include <string>
#include <cmath>  //To determine M_PI and is also used for math operations

namespace tsgl {

class CartesianCanvas;  //Forward declaration for typedef

//Constants to be used by TSGL classes and tests
#ifdef _WIN32
const double PI = 3.14159265358979323846;
#else
const double PI = M_PI;
#endif
const double RAD = PI / 180;     // One radian in degrees

const int FPS = 60;                // Frames per second
const float FRAME = 1.0f/FPS;      // Number of seconds between frames

/*!
 * \var typedef long double Decimal
 * \brief A type definition for a long double.
 * \note Used by: CartesianCanvas, Function, and IntegralViewer.
 */
typedef long double Decimal;  // Define the variable type to use for coordinates
typedef CartesianCanvas Cart; // Shorter name for CartesianCanvas

const int NUM_COLORS = 256, MAX_COLOR = 255;

/*!
 * \brief Clamp a floating point number between two values
 * \details This function clamps the floating point variable <code>f</code> between <code>min</code>
 *   and <code>max</code>.  In other words, <code>f</code> is set to <code>min</code> if less than
 *   <code>min</code>, to <code>max</code> if greater than <code>max</code>, and untouched otherwise.
 *   \param f Reference to the floating point number to clamp
 *   \param min Minimum legal value for <code>f</code>
 *   \param max Maximum legal value for <code>f</code>
 */
inline bool clamp(float& f, float min, float max) {
  if (f < min) {
    f = min; return true;
  }
  if (f > max) {
    f = max; return true;
  }
  return false;
}

/*!
 * \brief Clamp an integer number between two values
 * \details This function clamps the integer variable <code>i</code> between <code>min</code>
 *   and <code>max</code>.  In other words, <code>i</code> is set to <code>min</code> if less than
 *   <code>min</code>, to <code>max</code> if greater than <code>max</code>, and untouched otherwise.
 *   \param i Reference to the integer number to clamp
 *   \param min Minimum legal value for <code>i</code>
 *   \param max Maximum legal value for <code>i</code>
 */
inline bool clamp(int& i, int min, int max) {
  if (i < min) {
    i = min; return true;
  }
  if (i > max) {
    i = max; return true;
  }
  return false;
}

/*!
 * \brief Wrap a floating point number between zero and a maximum number
 * \details This function wraps the floating point variable <code>f</code>
 *   between <code>0</code> and <code>max</code>.  In other words, if <code>f</code> is greater
 *   than 0, <code>max</code> is subtracted from <code>f</code> until it is less than <code>max</code>.
 *   If <code>f</code> is less than 0, <code>max</code> is added to <code>f</code> until it is greater
 *   than 0.
 *   \param f Reference to the floating point number to wrap
 *   \param max Absolute maximum legal magnitude for <code>f</code>
 */
inline void wrap(float& f, float max) {
  f = (f < 0) ? (f + max*ceil((-f)/max)) : fmod(f,max);
}

/*!
 * \brief Wrap an integer number between zero and a maximum number
 * \details This function wraps the integer variable <code>i</code>
 *   between <code>0</code> and <code>max</code>.  In other words, if <code>i</code> is greater
 *   than 0, <code>max</code> is subtracted from <code>i</code> until it is less than <code>max</code>.
 *   If <code>i</code> is less than 0, <code>max</code> is added to <code>i</code> until it is greater
 *   than 0.
 *   \param i Reference to the integer number to wrap
 *   \param max Absolute maximum legal magnitude for <code>i</code>
 */
inline void wrap(int& i, int max) {
  i = (i < 0) ? (i + max*ceil((-(float)i)/max)) : i % max;
}

/*!
 * \brief Test if a file with the give name exists
 * \details This function tests if the file with the given name exists, returning
 *  true for files that exist, and false for directories and files that don't exist
 * \param name The name of the file to test for
 */
inline bool fileExists (const std::string& name) {
  #ifdef _WIN32  //To handle the fopen issue in Windows...
		FILE * file;
		//https://msdn.microsoft.com/en-us/library/z5hh6ee9.aspx
		//Use fopen_s() instead of fopen() in Windows
		if (fopen_s(&file, name.c_str(), "r")) {
			fclose(file);
			return true;
		}
		else {
			return false;
		}
	#else
	  //Use the normal fopen()
		if (FILE *file = fopen(name.c_str(), "r")) {
			fclose(file);
			return true;
		}
		else {
			return false;
	  }
	#endif
}

}

#endif /* SRC_TSGL_UTIL_H_ */
