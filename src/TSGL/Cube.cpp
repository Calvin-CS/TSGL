#include "Cube.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Cube.
  * \details Explicit constructor for a Cube object.
  *   \param x The x coordinate of the center of the Cube.
  *   \param y The y coordinate of the center of the Cube.
  *   \param z The z coordinate of the center of the Cube.
  *   \param sideLength The side length of the Cube.
  *   \param yaw The Cube's yaw, in degrees.
  *   \param pitch The Cube's pitch, in degrees.
  *   \param roll The Cube's roll, in degrees.
  *   \param c A ColorGLfloat for the Cube's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cube with a buffer for storing the specified numbered of vertices.
  */
Cube::Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c)  
: Object3D(x, y, z, yaw, pitch, roll)  { // FIXME vertices
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Cube with non-positive sidelength.");
    }
    attribMutex.lock();
    geometryType = GL_QUADS;
    mySideLength = sideLength;
    myXScale = sideLength;
    myYScale = sideLength;
    myZScale = sideLength;
    numberOfVertices = 24;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(-0.5, 0.5, -0.5, c);
    
    addVertex(0.5, -0.5, -0.5, c);
    addVertex(0.5, -0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, -0.5, c);

    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(0.5, -0.5, 0.5, c);
    addVertex(0.5, -0.5, -0.5, c);

    addVertex(-0.5, 0.5, -0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, -0.5, c);

    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, 0.5, -0.5, c);
    addVertex(0.5, 0.5, -0.5, c);
    addVertex(0.5, -0.5, -0.5, c);

    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(0.5, -0.5, 0.5, c);
}

 /*!
  * \brief Explicitly constructs a new Cube.
  * \details Explicit constructor for a Cube object.
  *   \param x The x coordinate of the center of the Cube.
  *   \param y The y coordinate of the center of the Cube.
  *   \param z The z coordinate of the center of the Cube.
  *   \param sideLength The side length of the Cube.
  *   \param yaw The Cube's yaw, in degrees.
  *   \param pitch The Cube's pitch, in degrees.
  *   \param roll The Cube's roll, in degrees.
  *   \param c An array of ColorGLfloats for the Cube's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cube with a buffer for storing the specified numbered of vertices.
  */
Cube::Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorGLfloat c[])  
: Object3D(x, y, z, yaw, pitch, roll)  { // FIXME vertices
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Cube with non-positive sidelength.");
    }
    attribMutex.lock();
    geometryType = GL_QUADS;
    mySideLength = sideLength;
    myXScale = sideLength;
    myYScale = sideLength;
    myZScale = sideLength;
    numberOfVertices = 24;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(-0.5, 0.5, -0.5, c[3]);
    
    addVertex(0.5, -0.5, -0.5, c[4]);
    addVertex(0.5, -0.5, 0.5, c[5]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(0.5, 0.5, -0.5, c[7]);

    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(0.5, -0.5, 0.5, c[5]);
    addVertex(0.5, -0.5, -0.5, c[4]);

    addVertex(-0.5, 0.5, -0.5, c[3]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(0.5, 0.5, -0.5, c[7]);

    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, 0.5, -0.5, c[3]);
    addVertex(0.5, 0.5, -0.5, c[7]);
    addVertex(0.5, -0.5, -0.5, c[4]);

    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(0.5, -0.5, 0.5, c[5]);
}

/**
 * \brief Mutates the distance between the Cube's opposite faces.
 * \param length The Cube's new side length.
 */
void Cube::setSideLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have a Cube with side length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = length;
    myYScale = length;
    myZScale = length;
    mySideLength = length;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the Cube's front face to its back face by the parameter amount.
 * \param delta The amount by which to change the length of the Cube.
 */
void Cube::changeSideLengthBy(GLfloat delta) {
    if (mySideLength + delta <= 0) {
        TsglDebug("Cannot have a Cube with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    mySideLength += delta;
    myXScale += delta;
    myYScale += delta;
    myZScale += delta;
    attribMutex.unlock();
}

GLfloat Cube::getSideLength() {
    return mySideLength;
}

/*!
 * \brief Destructor for the Cube.
 */
Cube::~Cube() { }

}