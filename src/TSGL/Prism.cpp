#include "Prism.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Prism.
  * \details Explicit constructor for a Prism object.
  *   \param x The x coordinate of the center of the Prism.
  *   \param y The y coordinate of the center of the Prism.
  *   \param z The z coordinate of the center of the Prism.
  *   \param vertices An array of vertices for the Prism's base.
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
Prism::Prism(float x, float y, float z, float vertices[], int sides, float yaw, float pitch, float roll, ColorFloat c)  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (sides < 3) {
        TsglDebug("Cannot have a Prism with fewer than 3 sides.");
    }
    numberOfVertices = sides * 6;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    geometryType = GL_QUADS;
    // FIXME make sure all vertex points are within one plane
    // FIXME calculate myHeight based on vertices and xyz?
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Prism.
  * \details Explicit constructor for a Prism object.
  *   \param x The x coordinate of the center of the Prism.
  *   \param y The y coordinate of the center of the Prism.
  *   \param z The z coordinate of the center of the Prism.
  *   \param vertices An array of vertices for the Prism's base.
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
Prism::Prism(float x, float y, float z, float vertices[], int sides, float yaw, float pitch, float roll, ColorFloat c[])  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (sides < 3) {
        TsglDebug("Cannot have a Prism with fewer than 3 sides.");
    }
    numberOfVertices = sides * 6;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    geometryType = GL_QUADS;
    // FIXME make sure all vertex points are within one plane
    // FIXME calculate myHeight based on vertices and xyz?
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Prism. Protected method.
  * \details Explicit constructor for a Prism object. Intended to be called only from subclasses.
  *   \param x The x coordinate of the center of the Prism.
  *   \param y The y coordinate of the center of the Prism.
  *   \param z The z coordinate of the center of the Prism.
  *   \param numVertices The number of vertices of the Prism.
  *   \param yaw The Prism's yaw.
  *   \param pitch The Prism's pitch.
  *   \param roll The Prism's roll.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \warning An invariant is held where if all points in vertices are not in the same plane then an error message is given.
  * \return A new Prism with a buffer for storing the specified numbered of vertices.
  */
Prism::Prism(float x, float y, float z, int numVertices, float yaw, float pitch, float roll)  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (numVertices < 12) {
        TsglDebug("Cannot have a Prism with fewer than 12 vertices.");
    }
    numberOfVertices = numVertices;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
}

/**
 * \brief Gets an array of the Prism's fill vertex colors.
 * \return c An array of ColorFloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
ColorFloat* Prism::getColor() {
  ColorFloat * c = new ColorFloat[numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++) {
        c[i] = ColorFloat(colors[i*4], colors[i*4 + 1], colors[i*4 + 2], colors[i*4 + 3]);
    }
    return c;
}

// /**
//  * \brief Mutates the distance from the center of the Prism's base to the tip.
//  * \param height The Prism's new height.
//  */
// void Prism::setHeight(float height) {
//     attribMutex.lock();
//     if (height <= 0) {
//         TsglDebug("Cannot have a Prism with height less than or equal to 0.");
//         attribMutex.unlock();
//         return;
//     }
//     myHeight = height;
//     attribMutex.unlock();
// }

// /**
//  * \brief Mutates the distance from the center of the Prism's base to the tip by the parameter amount.
//  * \param delta The amount by which to change the height of the Prism.
//  */
// void Prism::changeHeightBy(float delta) {
//     attribMutex.lock();
//     if (myHeight + delta <= 0) {
//         TsglDebug("Cannot have a Prism with height less than or equal to 0.");
//         attribMutex.unlock();
//         return;
//     }
//     myHeight += delta;
//     attribMutex.unlock();
// }

/*!
 * \brief Destructor for the Prism.
 */
Prism::~Prism() { }

}