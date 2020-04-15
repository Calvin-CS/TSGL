#include "Cuboid.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Cuboid.
  * \details Explicit constructor for a Cuboid object.
  *   \param x The x coordinate of the center of the Cuboid.
  *   \param y The y coordinate of the center of the Cuboid.
  *   \param z The z coordinate of the center of the Cuboid.
  *   \param length The length of the Cuboid.
  *   \param width The width of the Cuboid.
  *   \param height The height of the Cuboid.
  *   \param yaw The Cuboid's yaw.
  *   \param pitch The Cuboid's pitch.
  *   \param roll The Cuboid's roll.
  *   \param c A ColorGLfloat for the Cuboid's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cuboid with a buffer for storing the specified numbered of vertices.
  */
Cuboid::Cuboid(float x, float y, float z, GLfloat width, GLfloat height, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat c)  
: Object3D(x, y, z, yaw, pitch, roll)  {
    if (length <= 0 || width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Cuboid with non-positive length, width, or height.");
    }
    attribMutex.lock();
    geometryType = GL_QUADS;
    myLength = length;
    myWidth = width;
    myHeight = height;
    myXScale = width;
    myYScale = height;
    myZScale = length;
    numberOfVertices = numberOfOutlineVertices = 24;
    outlineGeometryType = GL_LINES;
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

    addVertex(0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(0.5, -0.5, 0.5, c);

    addVertex(0.5, 0.5, -0.5, c);
    addVertex(-0.5, 0.5, -0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);

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
  * \brief Explicitly constructs a new Cuboid.
  * \details Explicit constructor for a Cuboid object.
  *   \param x The x coordinate of the center of the Cuboid.
  *   \param y The y coordinate of the center of the Cuboid.
  *   \param z The z coordinate of the center of the Cuboid.
  *   \param length The length of the Cuboid.
  *   \param width The width of the Cuboid.
  *   \param height The height of the Cuboid.
  *   \param yaw The Cuboid's yaw.
  *   \param pitch The Cuboid's pitch.
  *   \param roll The Cuboid's roll.
  *   \param c An array of ColorGLfloats for the Cuboid's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cuboid with a buffer for storing the specified numbered of vertices.
  */
Cuboid::Cuboid(float x, float y, float z, GLfloat width, GLfloat height, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat c[])  
: Object3D(x, y, z, yaw, pitch, roll)  {
    if (length <= 0 || width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Cuboid with non-positive length, width, or height.");
    }
    attribMutex.lock();
    geometryType = GL_QUADS;
    myLength = length;
    myWidth = width;
    myHeight = height;
    myXScale = width;
    myYScale = height;
    myZScale = length;
    numberOfVertices = numberOfOutlineVertices = 24;
    outlineGeometryType = GL_LINES;
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

    addVertex(0.5, -0.5, -0.5, c[4]);
    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(0.5, -0.5, 0.5, c[5]);

    addVertex(0.5, 0.5, -0.5, c[7]);
    addVertex(-0.5, 0.5, -0.5, c[3]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(0.5, 0.5, 0.5, c[6]);

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
 * \brief Mutates the distance from the Cuboid's front face to its back face.
 * \param height The Cuboid's new length.
 */
void Cuboid::setLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have a Cuboid with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myLength = length;
    myZScale = length;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the Cuboid's front face to its back face by the parameter amount.
 * \param delta The amount by which to change the length of the Cuboid.
 */
void Cuboid::changeLengthBy(GLfloat delta) {
    if (myLength + delta <= 0) {
        TsglDebug("Cannot have a Cuboid with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myLength += delta;
    myZScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the Cuboid's left face to its right face.
 * \param height The Cuboid's new width.
 */
void Cuboid::setWidth(GLfloat width) {
    if (width <= 0) {
        TsglDebug("Cannot have a Cuboid with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth = width;
    myXScale = width;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the Cuboid's left face to its right face by the parameter amount.
 * \param delta The amount by which to change the width of the Cuboid.
 */
void Cuboid::changeWidthBy(GLfloat delta) {
    if (myWidth + delta <= 0) {
        TsglDebug("Cannot have a Cuboid with width less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myWidth += delta;
    myXScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Cuboid's base to the top.
 * \param height The Cuboid's new height.
 */
void Cuboid::setHeight(GLfloat height) {
    if (height <= 0) {
        TsglDebug("Cannot have a Cuboid with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight = height;
    myYScale = height;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Cuboid's base to the top by the parameter amount.
 * \param delta The amount by which to change the height of the Cuboid.
 */
void Cuboid::changeHeightBy(GLfloat delta) {
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Cuboid with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight += delta;
    myYScale += delta;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Cuboid.
 */
Cuboid::~Cuboid() { }

}