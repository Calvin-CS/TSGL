#ifndef STATUS_ENUM_H_
#define STATUS_ENUM_H_

#include <tsgl.h>

/*! \brief Enum for valid Person states for the Pandemic
 */
enum PersonState {
	infected = 'v', immune = 'i', susceptible = 's', dead = 'd'
};
#endif /* STATUS_ENUM_H_ */