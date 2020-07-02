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
  *   \param c A ColorFloat for the Cube's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cube with a buffer for storing the specified numbered of vertices.
  */
Cube::Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat c)  
: Shape(x, y, z, yaw, pitch, roll)  {
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Cube with non-positive sidelength.");
    }
    attribMutex.lock();
    geometryType = GL_TRIANGLES;
    mySideLength = sideLength;
    myXScale = sideLength;
    myYScale = sideLength;
    myZScale = sideLength;
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
  * \brief Explicitly constructs a new Cube.
  * \details Explicit constructor for a Cube object.
  *   \param x The x coordinate of the center of the Cube.
  *   \param y The y coordinate of the center of the Cube.
  *   \param z The z coordinate of the center of the Cube.
  *   \param sideLength The side length of the Cube.
  *   \param yaw The Cube's yaw, in degrees.
  *   \param pitch The Cube's pitch, in degrees.
  *   \param roll The Cube's roll, in degrees.
  *   \param c An array of ColorFloats for the Cube's vertex colors.
  * \warning An invariant is held where if length, width, or height isn't positive then an error message is given.
  * \return A new Cube with a buffer for storing the specified numbered of vertices.
  */
Cube::Cube(float x, float y, float z, GLfloat sideLength, float yaw, float pitch, float roll, ColorFloat c[])  
: Shape(x, y, z, yaw, pitch, roll)  {
    if (sideLength <= 0) {
        TsglDebug("Cannot have a Cube with non-positive sidelength.");
    }
    attribMutex.lock();
    geometryType = GL_TRIANGLES;
    mySideLength = sideLength;
    myXScale = sideLength;
    myYScale = sideLength;
    myZScale = sideLength;
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

/**
 * \brief Sets the Cube to an array of new colors.
 * \param c An array of new ColorFloats.
 * \details The array should have 8 ColorFloats minimum, one for each corner.
 */
void Cube::setColor(ColorFloat c[]) {
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

/*!
 * \brief Destructor for the Cube.
 */
Cube::~Cube() { }

}