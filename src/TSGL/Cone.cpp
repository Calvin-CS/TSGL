#include "Cone.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Cone.
  * \details Explicit constructor for a Cone object.
  *   \param x The x coordinate of the center of the Cone.
  *   \param y The y coordinate of the center of the Cone.
  *   \param z The z coordinate of the center of the Cone.
  *   \param height The distance from the center of the base to tip of the Cone.
  *   \param radius The radius of the Cone's circular base.
  *   \param yaw The Cone's yaw.
  *   \param pitch The Cone's pitch.
  *   \param roll The Cone's roll.
  *   \param c A ColorFloat for the Cone's vertex colors.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Cone with a buffer for storing the specified numbered of vertices.
  */
Cone::Cone(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c)  
: Pyramid(x, y, z, height, radius, (radius > 1) ? radius * 3 : 3, yaw, pitch, roll, c)  { }

 /*!
  * \brief Explicitly constructs a new Cone.
  * \details Explicit constructor for a Cone object.
  *   \param x The x coordinate of the center of the Cone.
  *   \param y The y coordinate of the center of the Cone.
  *   \param z The z coordinate of the center of the Cone.
  *   \param height The distance from the center of the base to tip of the Cone.
  *   \param radius The radius of the Cone's circular base.
  *   \param yaw The Cone's yaw.
  *   \param pitch The Cone's pitch.
  *   \param roll The Cone's roll.
  *   \param c An array of ColorFloats containing the Cone's vertex colors.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Cone with a buffer for storing the specified numbered of vertices.
  */
Cone::Cone(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c[]) 
: Pyramid(x, y, z, height, radius, (radius > 1) ? radius * 3 : 3, yaw, pitch, roll, c)  { }

/*!
 * \brief Destructor for the Cone.
 */
Cone::~Cone() { }

}