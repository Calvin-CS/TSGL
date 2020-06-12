#include "Cylinder.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Cylinder.
  * \details Explicit constructor for a Cylinder object.
  *   \param x The x coordinate of the center of the Cylinder.
  *   \param y The y coordinate of the center of the Cylinder.
  *   \param z The z coordinate of the center of the Cylinder.
  *   \param height The height of the Cylinder.
  *   \param radius The radius of the Cylinder's circular base.
  *   \param yaw The Cylinder's yaw.
  *   \param pitch The Cylinder's pitch.
  *   \param roll The Cylinder's roll.
  *   \param c A ColorFloat for the Cylinder's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cylinder with a buffer for storing the specified numbered of vertices.
  */
Cylinder::Cylinder(float x, float y, float z, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c)  
: Prism(x, y, z, 40, height, radius, yaw, pitch, roll, c)  {
    outlineStride = 3;
    numberOfOutlineVertices = mySides * 4;
    outlineGeometryType = GL_LINES;
}

 /*!
  * \brief Explicitly constructs a new Cylinder.
  * \details Explicit constructor for a Cylinder object.
  *   \param x The x coordinate of the center of the Cylinder.
  *   \param y The y coordinate of the center of the Cylinder.
  *   \param z The z coordinate of the center of the Cylinder.
  *   \param height The height of the Cylinder.
  *   \param radius The radius of the Cylinder's circular base.
  *   \param yaw The Cylinder's yaw.
  *   \param pitch The Cylinder's pitch.
  *   \param roll The Cylinder's roll.
  *   \param c An array of ColorFloats for the Cylinder's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cylinder with a buffer for storing the specified numbered of vertices.
  */
Cylinder::Cylinder(float x, float y, float z, float height, float radius, float yaw, float pitch, float roll, ColorFloat c[])  
: Prism(x, y, z, 40, height, radius, yaw, pitch, roll, c)  { 
    outlineStride = 3;
    numberOfOutlineVertices = mySides * 4;
    outlineGeometryType = GL_LINES;
}

/*!
 * \brief Destructor for the Cylinder.
 */
Cylinder::~Cylinder() { }

}