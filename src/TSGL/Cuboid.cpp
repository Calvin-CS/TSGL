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
    numberOfVertices = 24;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    addVertex(-0.5*myWidth, -0.5*myHeight, -0.5*myLength, c);
    addVertex(-0.5*myWidth, -0.5*myHeight, 0.5*myLength, c);
    addVertex(-0.5*myWidth, 0.5*myHeight, 0.5*myLength, c);
    addVertex(-0.5*myWidth, 0.5*myHeight, -0.5*myLength, c);
    
    addVertex(0.5*myWidth, -0.5*myHeight, -0.5*myLength, c);
    addVertex(0.5*myWidth, -0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, 0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, 0.5*myHeight, -0.5*myLength, c);

    addVertex(-0.5*myWidth, -0.5*myHeight, -0.5*myLength, c);
    addVertex(-0.5*myWidth, -0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, -0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, -0.5*myHeight, -0.5*myLength, c);

    addVertex(-0.5*myWidth, 0.5*myHeight, -0.5*myLength, c);
    addVertex(-0.5*myWidth, 0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, 0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, 0.5*myHeight, -0.5*myLength, c);

    addVertex(-0.5*myWidth, -0.5*myHeight, -0.5*myLength, c);
    addVertex(-0.5*myWidth, 0.5*myHeight, -0.5*myLength, c);
    addVertex(0.5*myWidth, 0.5*myHeight, -0.5*myLength, c);
    addVertex(0.5*myWidth, -0.5*myHeight, -0.5*myLength, c);

    addVertex(-0.5*myWidth, -0.5*myHeight, 0.5*myLength, c);
    addVertex(-0.5*myWidth, 0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, 0.5*myHeight, 0.5*myLength, c);
    addVertex(0.5*myWidth, -0.5*myHeight, 0.5*myLength, c);
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
    numberOfVertices = 24;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    addVertex(-0.5*myWidth, -0.5*myHeight, -0.5*myLength, c[0]);
    addVertex(-0.5*myWidth, -0.5*myHeight, 0.5*myLength, c[1]);
    addVertex(-0.5*myWidth, 0.5*myHeight, 0.5*myLength, c[2]);
    addVertex(-0.5*myWidth, 0.5*myHeight, -0.5*myLength, c[3]);
    
    addVertex(0.5*myWidth, -0.5*myHeight, -0.5*myLength, c[4]);
    addVertex(0.5*myWidth, -0.5*myHeight, 0.5*myLength, c[5]);
    addVertex(0.5*myWidth, 0.5*myHeight, 0.5*myLength, c[6]);
    addVertex(0.5*myWidth, 0.5*myHeight, -0.5*myLength, c[7]);

    addVertex(-0.5*myWidth, -0.5*myHeight, -0.5*myLength, c[0]);
    addVertex(-0.5*myWidth, -0.5*myHeight, 0.5*myLength, c[1]);
    addVertex(0.5*myWidth, -0.5*myHeight, 0.5*myLength, c[5]);
    addVertex(0.5*myWidth, -0.5*myHeight, -0.5*myLength, c[4]);

    addVertex(-0.5*myWidth, 0.5*myHeight, -0.5*myLength, c[3]);
    addVertex(-0.5*myWidth, 0.5*myHeight, 0.5*myLength, c[2]);
    addVertex(0.5*myWidth, 0.5*myHeight, 0.5*myLength, c[6]);
    addVertex(0.5*myWidth, 0.5*myHeight, -0.5*myLength, c[7]);

    addVertex(-0.5*myWidth, -0.5*myHeight, -0.5*myLength, c[0]);
    addVertex(-0.5*myWidth, 0.5*myHeight, -0.5*myLength, c[3]);
    addVertex(0.5*myWidth, 0.5*myHeight, -0.5*myLength, c[7]);
    addVertex(0.5*myWidth, -0.5*myHeight, -0.5*myLength, c[4]);

    addVertex(-0.5*myWidth, -0.5*myHeight, 0.5*myLength, c[1]);
    addVertex(-0.5*myWidth, 0.5*myHeight, 0.5*myLength, c[2]);
    addVertex(0.5*myWidth, 0.5*myHeight, 0.5*myLength, c[6]);
    addVertex(0.5*myWidth, -0.5*myHeight, 0.5*myLength, c[5]);
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
    GLfloat ratio = length/myLength;
    myLength = length;
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*3 + 2] *= ratio;
    }
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
    for(int i = 0; i < numberOfVertices; i++) {
        if (vertices[i*3 + 2] > 0)
            vertices[i*3 + 2] += delta/2;
        else
            vertices[i*3 + 2] -= delta/2;
    }
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
    GLfloat ratio = width/myWidth;
    myWidth = width;
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*3] *= ratio;
    }
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
    for(int i = 0; i < numberOfVertices; i++) {
        if (vertices[i*3] > 0)
            vertices[i*3] += delta/2;
        else
            vertices[i*3] -= delta/2;
    }
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
    GLfloat ratio = height/myHeight;
    myHeight = height;
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*3 + 1] *= ratio;
    }
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
    for(int i = 0; i < numberOfVertices; i++) {
        if (vertices[i*3 + 1] > 0)
            vertices[i*3 + 1] += delta/2;
        else
            vertices[i*3 + 1] -= delta/2;
    }
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Cuboid.
 */
Cuboid::~Cuboid() { }

}