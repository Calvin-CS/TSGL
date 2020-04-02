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
Prism::Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c)  : Object3D(x, y, z, yaw, pitch, roll)  {
    if (radius <= 0 ||  height <= 0 || sides < 3) {
        TsglDebug("Cannot have a Prism with non-positive height or radius or fewer than 3 sides");
    }
    attribMutex.lock();
    myRadius = radius;
    myHeight = height;
    mySides = sides;
    geometryType = GL_TRIANGLES;
    numberOfVertices = mySides * 12;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    GLfloat half = myHeight/2;
    for (int i = 0; i < mySides; i++) {
        addVertex(0,half,0, c);
        addVertex(myRadius * cos(TWOPI * i / mySides), half, myRadius * sin(TWOPI * i / mySides), c);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), half, myRadius * sin(TWOPI * (i + 1) / mySides), c);

        addVertex(myRadius * cos(TWOPI * i / mySides), half, myRadius * sin(TWOPI * i / mySides), c);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), half, myRadius * sin(TWOPI * (i + 1) / mySides), c);
        addVertex(myRadius * cos(TWOPI * i / mySides), -half, myRadius * sin(TWOPI * i / mySides), c);

        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), half, myRadius * sin(TWOPI * (i + 1) / mySides), c);
        addVertex(myRadius * cos(TWOPI * i / mySides), -half, myRadius * sin(TWOPI * i / mySides), c);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), -half, myRadius * sin(TWOPI * (i + 1) / mySides), c);

        addVertex(myRadius * cos(TWOPI * i / mySides), -half, myRadius * sin(TWOPI * i / mySides), c);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), -half, myRadius * sin(TWOPI * (i + 1) / mySides), c);
        addVertex(0,-half,0, c);
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
Prism::Prism(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c[])  : Object3D(x, y, z, yaw, pitch, roll)  {
    if (radius <= 0 ||  height <= 0 || sides < 3) {
        TsglDebug("Cannot have a Prism with non-positive height or radius or fewer than 3 sides");
    }
    attribMutex.lock();
    myRadius = radius;
    myHeight = height;
    mySides = sides;
    geometryType = GL_TRIANGLES;
    numberOfVertices = mySides * 12;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    GLfloat half = myHeight/2;
    for (int i = 0; i < mySides; i++) {
        addVertex(0,half,0, c[0]);
        addVertex(myRadius * cos(TWOPI * i / mySides), half, myRadius * sin(TWOPI * i / mySides), c[1]);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), half, myRadius * sin(TWOPI * (i + 1) / mySides), c[1]);

        addVertex(myRadius * cos(TWOPI * i / mySides), half, myRadius * sin(TWOPI * i / mySides), c[2]);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), half, myRadius * sin(TWOPI * (i + 1) / mySides), c[2]);
        addVertex(myRadius * cos(TWOPI * i / mySides), -half, myRadius * sin(TWOPI * i / mySides), c[2]);

        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), half, myRadius * sin(TWOPI * (i + 1) / mySides), c[2]);
        addVertex(myRadius * cos(TWOPI * i / mySides), -half, myRadius * sin(TWOPI * i / mySides), c[2]);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), -half, myRadius * sin(TWOPI * (i + 1) / mySides), c[2]);

        addVertex(myRadius * cos(TWOPI * i / mySides), -half, myRadius * sin(TWOPI * i / mySides), c[3]);
        addVertex(myRadius * cos(TWOPI * (i + 1) / mySides), -half, myRadius * sin(TWOPI * (i + 1) / mySides), c[3]);
        addVertex(0,-half,0, c[4]);
    }
}

/**
 * \brief Gets an array of the Prism's fill vertex colors.
 * \return c An array of ColorGLfloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
// ColorGLfloat* Prism::getColor() {
//   ColorGLfloat * c = new ColorGLfloat[numberOfVertices];
//     for(int i = 0; i < numberOfVertices; i++) {
//         c[i] = ColorGLfloat(colors[i*4], colors[i*4 + 1], colors[i*4 + 2], colors[i*4 + 3]);
//     }
//     return c;
// }

/**
 * \brief Mutates the distance from the center of the Prism's base to the tip.
 * \param height The Prism's new height.
 */
void Prism::setHeight(GLfloat height) {
    if (height <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    GLfloat ratio = height/myHeight;
    myHeight = height;
    for (int i = 0; i < numberOfVertices; i++) {
        vertices[i*3 + 1] *= ratio;
    }
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Prism's base to the tip by the parameter amount.
 * \param delta The amount by which to change the height of the Prism.
 */
void Prism::changeHeightBy(GLfloat delta) {
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight += delta;
    for (int i = 0; i < numberOfVertices; i++) {
        if (vertices[i*3 + 1] > 0)
            vertices[i*3 + 1] += delta/2;
        else
            vertices[i*3 + 1] -= delta/2;
    }
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Prism's base to the tip.
 * \param height The Prism's new height.
 */
void Prism::setRadius(GLfloat radius) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    GLfloat ratio = radius/myRadius;
    myRadius = radius;
    for (int i = 0; i < numberOfVertices; i++) {
        vertices[i*3] *= ratio;
        vertices[i*3 + 2] *= ratio;
    }
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Prism's base to the tip by the parameter amount.
 * \param delta The amount by which to change the height of the Prism.
 */
void Prism::changeRadiusBy(GLfloat delta) {
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Prism with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius += delta;
    for (int i = 0; i < mySides; i++) {
        // 0, 2
        vertices[i*36 + 3] = myRadius * cos(TWOPI * i / mySides);
        vertices[i*36 + 5] = myRadius * sin(TWOPI * i / mySides);

        vertices[i*36 + 6] = myRadius * cos(TWOPI * (i + 1) / mySides);
        vertices[i*36 + 8] = myRadius * sin(TWOPI * (i + 1) / mySides);

        vertices[i*36 + 9] = myRadius * cos(TWOPI * i / mySides);
        vertices[i*36 + 11] = myRadius * sin(TWOPI * i / mySides);

        vertices[i*36 + 12] = myRadius * cos(TWOPI * (i + 1) / mySides);
        vertices[i*36 + 14] = myRadius * sin(TWOPI * (i + 1) / mySides);
        
        vertices[i*36 + 15] = myRadius * cos(TWOPI * i / mySides);
        vertices[i*36 + 17] = myRadius * sin(TWOPI * i / mySides);

        vertices[i*36 + 18] = myRadius * cos(TWOPI * (i + 1) / mySides);
        vertices[i*36 + 20] = myRadius * sin(TWOPI * (i + 1) / mySides);
        
        vertices[i*36 + 21] = myRadius * cos(TWOPI * i / mySides);
        vertices[i*36 + 23] = myRadius * sin(TWOPI * i / mySides);

        vertices[i*36 + 24] = myRadius * cos(TWOPI * (i + 1) / mySides);
        vertices[i*36 + 26] = myRadius * sin(TWOPI * (i + 1) / mySides);

        vertices[i*36 + 27] = myRadius * cos(TWOPI * i / mySides);
        vertices[i*36 + 29] = myRadius * sin(TWOPI * i / mySides);

        vertices[i*36 + 30] = myRadius * cos(TWOPI * (i + 1) / mySides);
        vertices[i*36 + 32] = myRadius * sin(TWOPI * (i + 1) / mySides);

        // 33, 35
    }
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Prism.
 */
Prism::~Prism() { }

}