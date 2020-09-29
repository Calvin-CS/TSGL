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
  *   \param c A ColorFloat for the Cuboid's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cuboid with a buffer for storing the specified numbered of vertices.
  */
Cuboid::Cuboid(float x, float y, float z, GLfloat width, GLfloat height, GLfloat length, float yaw, float pitch, float roll, ColorFloat c)  
: Shape(x, y, z, yaw, pitch, roll)  {
    if (length <= 0 || width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Cuboid with non-positive length, width, or height.");
    }
    attribMutex.lock();
    geometryType = GL_TRIANGLES;
    myLength = length;
    myWidth = width;
    myHeight = height;
    myXScale = width;
    myYScale = height;
    myZScale = length;
    numberOfVertices = 36;
    vertices = new GLfloat[numberOfVertices * 7];
    outlineGeometryType = GL_LINES;
    numberOfOutlineVertices = 24;
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    attribMutex.unlock();
    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(-0.5, 0.5, -0.5, c);
    
    addVertex(0.5, -0.5, -0.5, c);
    addVertex(0.5, -0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(0.5, -0.5, -0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, -0.5, c);

    addVertex(0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(0.5, -0.5, -0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(0.5, -0.5, 0.5, c);

    addVertex(0.5, 0.5, -0.5, c);
    addVertex(-0.5, 0.5, -0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, -0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);

    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(-0.5, 0.5, -0.5, c);
    addVertex(0.5, 0.5, -0.5, c);
    addVertex(-0.5, -0.5, -0.5, c);
    addVertex(0.5, 0.5, -0.5, c);
    addVertex(0.5, -0.5, -0.5, c);

    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(-0.5, 0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(-0.5, -0.5, 0.5, c);
    addVertex(0.5, 0.5, 0.5, c);
    addVertex(0.5, -0.5, 0.5, c);

    addOutlineVertex( 0.5,  0.5,  0.5, GRAY);
    addOutlineVertex( 0.5,  0.5, -0.5, GRAY);
    addOutlineVertex( 0.5,  0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5,  0.5, GRAY);
    addOutlineVertex( 0.5, -0.5,  0.5, GRAY);
    addOutlineVertex( 0.5,  0.5,  0.5, GRAY);

    addOutlineVertex(-0.5,  0.5,  0.5, GRAY);
    addOutlineVertex(-0.5,  0.5, -0.5, GRAY);
    addOutlineVertex(-0.5,  0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5,  0.5, GRAY);
    addOutlineVertex(-0.5, -0.5,  0.5, GRAY);
    addOutlineVertex(-0.5,  0.5,  0.5, GRAY);

    addOutlineVertex(-0.5,  0.5,  0.5, GRAY);
    addOutlineVertex( 0.5,  0.5,  0.5, GRAY);
    addOutlineVertex(-0.5,  0.5, -0.5, GRAY);
    addOutlineVertex( 0.5,  0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5,  0.5, GRAY);
    addOutlineVertex( 0.5, -0.5,  0.5, GRAY);
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
  *   \param c An array of ColorFloats for the Cuboid's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cuboid with a buffer for storing the specified numbered of vertices.
  */
Cuboid::Cuboid(float x, float y, float z, GLfloat width, GLfloat height, GLfloat length, float yaw, float pitch, float roll, ColorFloat c[])  
: Shape(x, y, z, yaw, pitch, roll)  {
    if (length <= 0 || width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Cuboid with non-positive length, width, or height.");
    }
    attribMutex.lock();
    geometryType = GL_TRIANGLES;
    myLength = length;
    myWidth = width;
    myHeight = height;
    myXScale = width;
    myYScale = height;
    myZScale = length;
    numberOfVertices = 36;
    vertices = new GLfloat[numberOfVertices * 7];
    outlineGeometryType = GL_LINES;
    numberOfOutlineVertices = 24;
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
    attribMutex.unlock();
    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(-0.5, 0.5, -0.5, c[3]);
    
    addVertex(0.5, -0.5, -0.5, c[4]);
    addVertex(0.5, -0.5, 0.5, c[5]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(0.5, -0.5, -0.5, c[4]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(0.5, 0.5, -0.5, c[7]);

    addVertex(0.5, -0.5, -0.5, c[4]);
    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(0.5, -0.5, -0.5, c[4]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(0.5, -0.5, 0.5, c[5]);

    addVertex(0.5, 0.5, -0.5, c[7]);
    addVertex(-0.5, 0.5, -0.5, c[3]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(0.5, 0.5, -0.5, c[7]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(0.5, 0.5, 0.5, c[6]);

    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(-0.5, 0.5, -0.5, c[3]);
    addVertex(0.5, 0.5, -0.5, c[7]);
    addVertex(-0.5, -0.5, -0.5, c[0]);
    addVertex(0.5, 0.5, -0.5, c[7]);
    addVertex(0.5, -0.5, -0.5, c[4]);

    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(-0.5, 0.5, 0.5, c[2]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(-0.5, -0.5, 0.5, c[1]);
    addVertex(0.5, 0.5, 0.5, c[6]);
    addVertex(0.5, -0.5, 0.5, c[5]);

    addOutlineVertex( 0.5,  0.5,  0.5, GRAY);
    addOutlineVertex( 0.5,  0.5, -0.5, GRAY);
    addOutlineVertex( 0.5,  0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5,  0.5, GRAY);
    addOutlineVertex( 0.5, -0.5,  0.5, GRAY);
    addOutlineVertex( 0.5,  0.5,  0.5, GRAY);

    addOutlineVertex(-0.5,  0.5,  0.5, GRAY);
    addOutlineVertex(-0.5,  0.5, -0.5, GRAY);
    addOutlineVertex(-0.5,  0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5,  0.5, GRAY);
    addOutlineVertex(-0.5, -0.5,  0.5, GRAY);
    addOutlineVertex(-0.5,  0.5,  0.5, GRAY);

    addOutlineVertex(-0.5,  0.5,  0.5, GRAY);
    addOutlineVertex( 0.5,  0.5,  0.5, GRAY);
    addOutlineVertex(-0.5,  0.5, -0.5, GRAY);
    addOutlineVertex( 0.5,  0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5, -0.5, GRAY);
    addOutlineVertex( 0.5, -0.5, -0.5, GRAY);
    addOutlineVertex(-0.5, -0.5,  0.5, GRAY);
    addOutlineVertex( 0.5, -0.5,  0.5, GRAY);
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
 * \param c An array of new ColorFloats.
 * \details The array should have 8 ColorFloats minimum, one for each corner.
 */
void Cuboid::setColor(ColorFloat c[]) {
    attribMutex.lock();
    vertices[3] = vertices[24] = vertices[94] = vertices[171] = vertices[192] = c[0].R;
    vertices[4] = vertices[25] = vertices[95] = vertices[172] = vertices[193] = c[0].G;
    vertices[5] = vertices[26] = vertices[96] = vertices[173] = vertices[194] = c[0].B;
    vertices[6] = vertices[27] = vertices[97] = vertices[174] = vertices[195] = c[0].A;

    vertices[10] = vertices[101] = vertices[115] = vertices[213] = vertices[234] = c[1].R;
    vertices[11] = vertices[102] = vertices[116] = vertices[214] = vertices[235] = c[1].G;
    vertices[12] = vertices[103] = vertices[117] = vertices[215] = vertices[236] = c[1].B;
    vertices[13] = vertices[104] = vertices[118] = vertices[216] = vertices[237] = c[1].A;

    vertices[17] = vertices[31] = vertices[143] = vertices[157] = vertices[220] = c[2].R;
    vertices[18] = vertices[32] = vertices[144] = vertices[158] = vertices[221] = c[2].G;
    vertices[19] = vertices[33] = vertices[145] = vertices[159] = vertices[222] = c[2].B;
    vertices[20] = vertices[34] = vertices[146] = vertices[160] = vertices[223] = c[2].A;

    vertices[38] = vertices[136] = vertices[178] = c[3].R;
    vertices[39] = vertices[137] = vertices[179] = c[3].G;
    vertices[40] = vertices[138] = vertices[180] = c[3].B;
    vertices[41] = vertices[139] = vertices[181] = c[3].A;

    vertices[45] = vertices[66] = vertices[87] = vertices[108] = vertices[206] = c[4].R;
    vertices[46] = vertices[67] = vertices[88] = vertices[109] = vertices[207] = c[4].G;
    vertices[47] = vertices[68] = vertices[89] = vertices[110] = vertices[208] = c[4].B;
    vertices[48] = vertices[69] = vertices[90] = vertices[111] = vertices[209] = c[4].A;

    vertices[52] = vertices[122] = vertices[248] = c[5].R;
    vertices[53] = vertices[123] = vertices[249] = c[5].G;
    vertices[54] = vertices[124] = vertices[250] = c[5].B;
    vertices[55] = vertices[125] = vertices[251] = c[5].A;

    vertices[59] = vertices[73] = vertices[164] = vertices[227] = vertices[241] = c[6].R;
    vertices[60] = vertices[74] = vertices[165] = vertices[228] = vertices[242] = c[6].G;
    vertices[61] = vertices[75] = vertices[166] = vertices[229] = vertices[243] = c[6].B;
    vertices[62] = vertices[76] = vertices[167] = vertices[230] = vertices[244] = c[6].A;

    vertices[80] = vertices[129] = vertices[150] = vertices[185] = vertices[199] = c[7].R;
    vertices[81] = vertices[130] = vertices[151] = vertices[186] = vertices[200] = c[7].G;
    vertices[82] = vertices[131] = vertices[152] = vertices[187] = vertices[201] = c[7].B;
    vertices[83] = vertices[132] = vertices[153] = vertices[188] = vertices[202] = c[7].A;
    myAlpha = (c[0].A + c[1].A + c[2].A + c[3].A + c[4].A + c[5].A + c[6].A + c[7].A) / 8;
    attribMutex.unlock();
}

/**
 * \brief Accessor for Cuboid's colors.
 * \details Populates the reference parameter vector with a ColorFloat for each corner of Cuboid.
 * \param colorVec A vector of ColorFloats to which the ColorFloats associated with Cuboid will be pushed.
 * \note Overrides Shape::getColors().
 */
void Cuboid::getColors(std::vector<ColorFloat> &colorVec) {
    attribMutex.lock();
    colorVec.push_back(ColorFloat(vertices[3],vertices[4],vertices[5],vertices[6]));
    colorVec.push_back(ColorFloat(vertices[10],vertices[11],vertices[12],vertices[13]));
    colorVec.push_back(ColorFloat(vertices[17],vertices[18],vertices[19],vertices[20]));
    colorVec.push_back(ColorFloat(vertices[38],vertices[39],vertices[40],vertices[41]));
    colorVec.push_back(ColorFloat(vertices[45],vertices[46],vertices[47],vertices[48]));
    colorVec.push_back(ColorFloat(vertices[52],vertices[53],vertices[54],vertices[55]));
    colorVec.push_back(ColorFloat(vertices[59],vertices[60],vertices[61],vertices[62]));
    colorVec.push_back(ColorFloat(vertices[80],vertices[81],vertices[82],vertices[83]));
    attribMutex.unlock();
}

}