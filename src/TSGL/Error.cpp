#include "Error.h"

namespace tsgl {

/*!
 * \brief Prints an error message to the console
 * \details This method prints the string <code>message</code> to the
 *   console in boldface red text.
 *   \param message The message to print to the console
 */
void TsglErr(std::string message) {
#ifdef _WIN32
  SetConsoleTextAttribute(std_con_out, ((unsigned)0<<4)|(unsigned)12);
  std::cerr << message << std::endl;
  SetConsoleTextAttribute(std_con_out, ((unsigned)0<<4)|(unsigned)15);
#else
  std::cerr << TSGL_RED << message << TSGL_CLR << std::endl;
#endif
}

/*!
 * \brief Prints a debug message to the console
 * \details This method prints the string <code>message</code> to the
 *   console in boldface magenta text.
 * \note This method prints only if DEBUG is defined, and does nothing otherwise.
 *   \param message The message to print to the console
 */
void TsglDebug(std::string message) {
#ifdef TSGL_DEBUG
  #ifdef _WIN32
    SetConsoleTextAttribute(std_con_out, ((unsigned)0<<4)|(unsigned)13);
    std::cerr << message << std::endl;
    SetConsoleTextAttribute(std_con_out, ((unsigned)0<<4)|(unsigned)15);
  #else
    std::cerr << TSGL_MGN << message << TSGL_CLR << std::endl;
  #endif
#endif
}

}
