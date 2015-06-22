#include "Error.h"

namespace tsgl {

void TsglErr(std::string message) {
#ifdef _WIN32
  SetConsoleTextAttribute(std_con_out, ((unsigned)0<<4)|(unsigned)12);
  std::cerr << message << std::endl;
  SetConsoleTextAttribute(std_con_out, ((unsigned)0<<4)|(unsigned)15);
#else
  std::cerr << TSGL_RED << message << TSGL_CLR << std::endl;
#endif
}

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
