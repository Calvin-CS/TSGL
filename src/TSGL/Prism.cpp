#include "Prism.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Prism.
  * \details Explicit constructor for a Prism object.
  *   \param x The x coordinate of the center of the Prism.
  *   \param y The y coordinate of the center of the Prism.
  *   \param z The z coordinate of the center of the Prism.
  *   \param sides The number of sides of the Prism's base.
  *   \param yaw The Prism's yaw.
  *   \param pitch The Prism's pitch.
  *   \param roll The Prism's roll.
  *   \param c A ColorGLfloat for the Prism's vertex colors.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \warning An invariant is held where if all points in vertices are not in the same plane then an error message is given.
  * \return A new Prism with a buffer for storing the specified numbered of vertices.
  */
Prism::Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c)  : Drawable(x, y, z, yaw, pitch, roll)  {
    if (radius <= 0 ||  height <= 0 || sides < 3) {
        TsglDebug("Cannot have a Prism with non-positive height or radius or fewer than 3 sides");
    }
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myZScale = radius;
    myHeight = height;
    myYScale = height;
    mySides = sides;
    geometryType = GL_TRIANGLES;
    numberOfVertices = mySides * 12;
    outlineStride = 2;
    numberOfOutlineVertices = mySides * 6;
    outlineGeometryType = GL_LINES;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    for (int i = 0; i < mySides; i++) {
        addVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), c);
        addVertex(0,0.5,0, c);
        addVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), c);

        addVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), c);
        addVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), c);
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);

        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);
        addVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), c);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c);

        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c);
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);
        addVertex(0,-0.5,0, c);
    }
}

 /*!
  * \brief Explicitly constructs a new Prism.
  * \details Explicit constructor for a Prism object.
  *   \param x The x coordinate of the center of the Prism.
  *   \param y The y coordinate of the center of the Prism.
  *   \param z The z coordinate of the center of the Prism.
  *   \param sides The number of sides of the Prism's base.
  *   \param yaw The Prism's yaw.
  *   \param pitch The Prism's pitch.
  *   \param roll The Prism's roll.
  *   \param c An array of ColorGLfloats for the Prism's vertex colors.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \warning An invariant is held where if all points in vertices are not in the same plane then an error message is given.
  * \return A new Prism with a buffer for storing the specified numbered of vertices.
  */
Prism::Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c[])  : Drawable(x, y, z, yaw, pitch, roll)  {
    if (radius <= 0 ||  height <= 0 || sides < 3) {
        TsglDebug("Cannot have a Prism with non-positive height or radius or fewer than 3 sides");
    }
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myZScale = radius;
    myHeight = height;
    myYScale = height;
    mySides = sides;
    geometryType = GL_TRIANGLES;
    numberOfVertices = mySides * 12;
    outlineStride = 2;
    numberOfOutlineVertices = mySides * 6;
    outlineGeometryType = GL_LINES;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    for (int i = 0; i < mySides; i++) {
        addVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), c[1]);
        addVertex(0,0.5,0, c[0]);
        addVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), c[1]);

        addVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), c[2]);
        addVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), c[2]);
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[2]);

        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[2]);
        addVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), c[2]);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c[2]);

        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c[3]);
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[3]);
        addVertex(0,-0.5,0, c[4]);
    }
}

/**
 * \brief Mutates the distance from the Prism's bottom to its top.
 * \param height The Prism's new height.
 */
void Prism::setHeight(GLfloat height) {
    if (height <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight = height;
    myYScale = height;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the Prism's bottom to its top by the parameter amount.
 * \param delta The amount by which to change the height of the Prism.
 */
void Prism::changeHeightBy(GLfloat delta) {
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight += delta;
    myYScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Prism's base to the corners.
 * \param radius The Prism's new radius.
 */
void Prism::setRadius(GLfloat radius) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myZScale = radius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Prism's base to the corner by the parameter amount.
 * \param delta The amount by which to change the radius of the Prism.
 */
void Prism::changeRadiusBy(GLfloat delta) {
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius += delta;
    myXScale += delta;
    myZScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutator. Sets the Prism to a new array of colors.
 * \param c The array of new ColorGLfloats.
 * \details The array should have 5 ColorGLfloats minimum: for the top middle, top outside, sides, bottom outside, and bottom middle respectively.
 */
void Prism::setColor(ColorGLfloat c[]) {
    attribMutex.lock();
    for (int i = 0; i < mySides; i++) {
        colors[i*48] = c[1].R;
        colors[i*48+1] = c[1].G;
        colors[i*48+2] = c[1].B;
        colors[i*48+3] = c[1].A;

        colors[i*48+4] = c[0].R;
        colors[i*48+5] = c[0].G;
        colors[i*48+6] = c[0].B;
        colors[i*48+7] = c[0].A;

        colors[i*48+8] = c[1].R;
        colors[i*48+9] = c[1].G;
        colors[i*48+10] = c[1].B;
        colors[i*48+11] = c[1].A;

        colors[i*48+12] = colors[i*48+16] = colors[i*48+20] = colors[i*48+24] = colors[i*48+28] = colors[i*48+32] = c[2].R;
        colors[i*48+13] = colors[i*48+17] = colors[i*48+21] = colors[i*48+25] = colors[i*48+29] = colors[i*48+33] = c[2].G;
        colors[i*48+14] = colors[i*48+18] = colors[i*48+22] = colors[i*48+26] = colors[i*48+30] = colors[i*48+34] = c[2].B;
        colors[i*48+15] = colors[i*48+19] = colors[i*48+23] = colors[i*48+27] = colors[i*48+31] = colors[i*48+35] = c[2].A;

        colors[i*48+36] = colors[i*48+40] = c[3].R;
        colors[i*48+37] = colors[i*48+41] = c[3].G;
        colors[i*48+38] = colors[i*48+42] = c[3].B;
        colors[i*48+39] = colors[i*48+43] = c[3].A;

        colors[i*48+44] = c[4].R;
        colors[i*48+45] = c[4].G;
        colors[i*48+46] = c[4].B;
        colors[i*48+47] = c[4].A;
    }
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Prism.
 */
Prism::~Prism() { }

}