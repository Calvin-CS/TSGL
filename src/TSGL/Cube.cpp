#include "Cube.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Cube.
  * \details Explicit constructor for a Cube object.
  *   \param x The x coordinate of the center of the Cube.
  *   \param y The y coordinate of the center of the Cube.
  *   \param z The z coordinate of the center of the Cube.
  *   \param sideLength The side length of the Cube.
  *   \param yaw The Cube's yaw.
  *   \param pitch The Cube's pitch.
  *   \param roll The Cube's roll.
  *   \param c A ColorGLfloat for the Cube's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cube with a buffer for storing the specified numbered of vertices.
  */
Cube::Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c)  
: Prism(x, y, z, 4, yaw, pitch, roll)  { // FIXME vertices
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Cube with non-positive sidelength.");
    }
    mySideLength = sideLength;
    addVertex(-1*mySideLength, -1*mySideLength, -1*mySideLength, c);
    addVertex(-1*mySideLength, -1*mySideLength, 1*mySideLength, c);
    addVertex(-1*mySideLength, 1*mySideLength, 1*mySideLength, c);
    addVertex(-1*mySideLength, 1*mySideLength, -1*mySideLength, c);
    
    addVertex(1*mySideLength, -1*mySideLength, -1*mySideLength, c);
    addVertex(1*mySideLength, -1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, 1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, 1*mySideLength, -1*mySideLength, c);

    addVertex(-1*mySideLength, -1*mySideLength, -1*mySideLength, c);
    addVertex(-1*mySideLength, -1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, -1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, -1*mySideLength, -1*mySideLength, c);

    addVertex(-1*mySideLength, 1*mySideLength, -1*mySideLength, c);
    addVertex(-1*mySideLength, 1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, 1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, 1*mySideLength, -1*mySideLength, c);

    addVertex(-1*mySideLength, -1*mySideLength, -1*mySideLength, c);
    addVertex(-1*mySideLength, 1*mySideLength, -1*mySideLength, c);
    addVertex(1*mySideLength, 1*mySideLength, -1*mySideLength, c);
    addVertex(1*mySideLength, -1*mySideLength, -1*mySideLength, c);

    addVertex(-1*mySideLength, -1*mySideLength, 1*mySideLength, c);
    addVertex(-1*mySideLength, 1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, 1*mySideLength, 1*mySideLength, c);
    addVertex(1*mySideLength, -1*mySideLength, 1*mySideLength, c);
}

 /*!
  * \brief Explicitly constructs a new Cube.
  * \details Explicit constructor for a Cube object.
  *   \param x The x coordinate of the center of the Cube.
  *   \param y The y coordinate of the center of the Cube.
  *   \param z The z coordinate of the center of the Cube.
  *   \param sideLength The side length of the Cube.
  *   \param yaw The Cube's yaw.
  *   \param pitch The Cube's pitch.
  *   \param roll The Cube's roll.
  *   \param c An array of ColorGLfloats for the Cube's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cube with a buffer for storing the specified numbered of vertices.
  */
Cube::Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c[])  
: Prism(x, y, z, 4, yaw, pitch, roll)  { // FIXME vertices
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Cube with non-positive side length.");
    }
    mySideLength = sideLength;
    addVertex(-1*mySideLength, -1*mySideLength, -1*mySideLength, c[0]);
    addVertex(-1*mySideLength, -1*mySideLength, 1*mySideLength, c[0]);
    addVertex(-1*mySideLength, 1*mySideLength, 1*mySideLength, c[0]);
    addVertex(-1*mySideLength, 1*mySideLength, -1*mySideLength, c[0]);
    
    addVertex(1*mySideLength, -1*mySideLength, -1*mySideLength, c[0]);
    addVertex(1*mySideLength, -1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, 1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, 1*mySideLength, -1*mySideLength, c[0]);

    addVertex(-1*mySideLength, -1*mySideLength, -1*mySideLength, c[0]);
    addVertex(-1*mySideLength, -1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, -1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, -1*mySideLength, -1*mySideLength, c[0]);

    addVertex(-1*mySideLength, 1*mySideLength, -1*mySideLength, c[0]);
    addVertex(-1*mySideLength, 1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, 1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, 1*mySideLength, -1*mySideLength, c[0]);

    addVertex(-1*mySideLength, -1*mySideLength, -1*mySideLength, c[0]);
    addVertex(-1*mySideLength, 1*mySideLength, -1*mySideLength, c[0]);
    addVertex(1*mySideLength, 1*mySideLength, -1*mySideLength, c[0]);
    addVertex(1*mySideLength, -1*mySideLength, -1*mySideLength, c[0]);

    addVertex(-1*mySideLength, -1*mySideLength, 1*mySideLength, c[0]);
    addVertex(-1*mySideLength, 1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, 1*mySideLength, 1*mySideLength, c[0]);
    addVertex(1*mySideLength, -1*mySideLength, 1*mySideLength, c[0]);
}

/**
 * \brief Mutates the distance from the Cube's front face to its back face.
 * \param height The Cube's new length.
 */
void Cube::setSideLength(GLfloat length) {
    attribMutex.lock();
    if (length <= 0) {
        TsglDebug("Cannot have a Cube with side length less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    mySideLength = length;
    attribMutex.unlock();
    // FIXME alter vertices
}

/**
 * \brief Mutates the distance from the Cube's front face to its back face by the parameter amount.
 * \param delta The amount by which to change the length of the Cube.
 */
void Cube::changeSideLengthBy(GLfloat delta) {
    attribMutex.lock();
    if (mySideLength + delta <= 0) {
        TsglDebug("Cannot have a Cube with length less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    mySideLength += delta;
    attribMutex.unlock();
    // FIXME alter vertices
}

void Cube::setRotation(float radians) {

}

/*!
 * \brief Destructor for the Cube.
 */
Cube::~Cube() { }

}