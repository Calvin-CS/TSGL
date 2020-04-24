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
: Drawable(x, y, z, yaw, pitch, roll)  {
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
: Drawable(x, y, z, yaw, pitch, roll)  {
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

/**
 * \brief Sets the Cuboid to an array of new colors.
 * \param c An array of new ColorGLfloats.
 * \details The array should have 8 ColorGLfloats minimum, one for each corner.
 */
void Cuboid::setColor(ColorGLfloat c[]) {
    attribMutex.lock();
    colors[0] = colors[36] = colors[64] = c[0].R;
    colors[1] = colors[37] = colors[65] = c[0].G;
    colors[2] = colors[38] = colors[66] = c[0].B;
    colors[3] = colors[39] = colors[67] = c[0].A;

    colors[4] = colors[40] = colors[80] = c[1].R;
    colors[5] = colors[41] = colors[81] = c[1].G;
    colors[6] = colors[42] = colors[82] = c[1].B;
    colors[7] = colors[43] = colors[83] = c[1].A;

    colors[8] = colors[56] = colors[84] = c[2].R;
    colors[9] = colors[57] = colors[85] = c[2].G;
    colors[10] = colors[58] = colors[86] = c[2].B;
    colors[11] = colors[59] = colors[87] = c[2].A;

    colors[12] = colors[52] = colors[68] = c[3].R;
    colors[13] = colors[53] = colors[69] = c[3].G;
    colors[14] = colors[54] = colors[70] = c[3].B;
    colors[15] = colors[55] = colors[71] = c[3].A;

    colors[16] = colors[32] = colors[76] = c[4].R;
    colors[17] = colors[33] = colors[77] = c[4].G;
    colors[18] = colors[34] = colors[78] = c[4].B;
    colors[19] = colors[35] = colors[79] = c[4].A;

    colors[20] = colors[44] = colors[92] = c[5].R;
    colors[21] = colors[45] = colors[93] = c[5].G;
    colors[22] = colors[46] = colors[94] = c[5].B;
    colors[23] = colors[47] = colors[95] = c[5].A;

    colors[24] = colors[60] = colors[88] = c[6].R;
    colors[25] = colors[61] = colors[89] = c[6].G;
    colors[26] = colors[62] = colors[90] = c[6].B;
    colors[27] = colors[63] = colors[91] = c[6].A;

    colors[28] = colors[48] = colors[72] = c[7].R;
    colors[29] = colors[49] = colors[73] = c[7].G;
    colors[30] = colors[50] = colors[74] = c[7].B;
    colors[31] = colors[51] = colors[75] = c[7].A;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Cuboid.
 */
Cuboid::~Cuboid() { }

}