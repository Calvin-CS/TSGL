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
  *   \param c A ColorFloat for the Prism's vertex colors.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \warning An invariant is held where if all points in vertices are not in the same plane then an error message is given.
  * \return A new Prism with a buffer for storing the specified numbered of vertices.
  */
Prism::Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c)  : Shape(x, y, z, yaw, pitch, roll)  {
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
    vertices = new GLfloat[numberOfVertices * 7];
    outlineGeometryType = GL_LINES;
    numberOfOutlineVertices = mySides * 6;
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
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

    for (int i = 0; i < mySides; i++) {
        addOutlineVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), GRAY);
        addOutlineVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), GRAY);
        currentOutlineVertex += numberOfOutlineVertices * 7 / 3 - 14;

        addOutlineVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), GRAY);
        addOutlineVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), GRAY);
        currentOutlineVertex += numberOfOutlineVertices * 7 / 3 - 14;

        addOutlineVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), GRAY);
        addOutlineVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), GRAY);
        currentOutlineVertex -= numberOfOutlineVertices * 7 * 2 / 3;
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
  *   \param c An array of ColorFloats for the Prism's vertex colors.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \warning An invariant is held where if all points in vertices are not in the same plane then an error message is given.
  * \return A new Prism with a buffer for storing the specified numbered of vertices.
  */
Prism::Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c[])  : Shape(x, y, z, yaw, pitch, roll)  {
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
    vertices = new GLfloat[numberOfVertices * 7];
    outlineGeometryType = GL_LINES;
    numberOfOutlineVertices = mySides * 6;
    outlineVertices = new GLfloat[numberOfOutlineVertices * 7];
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

    for (int i = 0; i < mySides; i++) {
        addOutlineVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), GRAY);
        addOutlineVertex(cos(TWOPI * (i + 1) / mySides), 0.5, sin(TWOPI * (i + 1) / mySides), GRAY);
        currentOutlineVertex += numberOfOutlineVertices * 7 / 3 - 14;

        addOutlineVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), GRAY);
        addOutlineVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), GRAY);
        currentOutlineVertex += numberOfOutlineVertices * 7 / 3 - 14;

        addOutlineVertex(cos(TWOPI * i / mySides), 0.5, sin(TWOPI * i / mySides), GRAY);
        addOutlineVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), GRAY);
        currentOutlineVertex -= numberOfOutlineVertices * 7 * 2 / 3;
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
 * \param c The array of new ColorFloats.
 * \details The array should have 5 ColorFloats minimum: for the top middle, top outside, sides, bottom outside, and bottom middle respectively.
 */
void Prism::setColor(ColorFloat c[]) {
    attribMutex.lock();
    myAlpha = 0.0;
    for (int i = 0; i < mySides; i++) {
        vertices[i*84+3] = c[1].R;
        vertices[i*84+4] = c[1].G;
        vertices[i*84+5] = c[1].B;
        vertices[i*84+6] = c[1].A;

        vertices[i*84+10] = c[0].R;
        vertices[i*84+11] = c[0].G;
        vertices[i*84+12] = c[0].B;
        vertices[i*84+13] = c[0].A;

        vertices[i*84+17] = c[1].R;
        vertices[i*84+18] = c[1].G;
        vertices[i*84+19] = c[1].B;
        vertices[i*84+20] = c[1].A;

        vertices[i*84+24] = vertices[i*84+31] = vertices[i*84+38] = vertices[i*84+45] = vertices[i*84+52] = vertices[i*84+59] = c[2].R;
        vertices[i*84+25] = vertices[i*84+32] = vertices[i*84+39] = vertices[i*84+46] = vertices[i*84+53] = vertices[i*84+60] = c[2].G;
        vertices[i*84+26] = vertices[i*84+33] = vertices[i*84+40] = vertices[i*84+47] = vertices[i*84+54] = vertices[i*84+61] = c[2].B;
        vertices[i*84+27] = vertices[i*84+34] = vertices[i*84+41] = vertices[i*84+48] = vertices[i*84+55] = vertices[i*84+62] = c[2].A;

        vertices[i*84+66] = vertices[i*84+73] = c[3].R;
        vertices[i*84+67] = vertices[i*84+74] = c[3].G;
        vertices[i*84+68] = vertices[i*84+75] = c[3].B;
        vertices[i*84+69] = vertices[i*84+76] = c[3].A;

        vertices[i*84+80] = c[4].R;
        vertices[i*84+81] = c[4].G;
        vertices[i*84+82] = c[4].B;
        vertices[i*84+83] = c[4].A;

        myAlpha += c[0].A + c[1].A * 2 + c[2].A * 6 + c[3].A * 2 + c[4].A;
    }
    myAlpha /= numberOfVertices;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Prism.
 */
Prism::~Prism() { }

}