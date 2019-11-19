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
Prism::Prism(float x, float y, float z, float vertices, int sides, float yaw, float pitch, float roll, ColorFloat c)  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (sides < 3) {
        TsglDebug("Cannot have a Prism with fewer than 3 sides.");
    }
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
Prism::Prism(float x, float y, float z, float vertices, int sides, float yaw, float pitch, float roll, ColorFloat c[])  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (sides < 3) {
        TsglDebug("Cannot have a Prism with fewer than 3 sides.");
    }
    // FIXME make sure all vertex points are within one plane
    // FIXME calculate myHeight based on vertices and xyz?
    attribMutex.unlock();
}

/**
 * \brief Sets the Prism to a new color.
 * \param c The new ColorFloat.
 */
void Prism::setColor(ColorFloat c) {
    attribMutex.lock();
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*6 + 2] = c.R;
        vertices[i*6 + 3] = c.G;
        vertices[i*6 + 4] = c.B;
        vertices[i*6 + 5] = c.A;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Prism to an array of new colors.
 * \param c An array of new ColorFloats.
 */
void Prism::setColor(ColorFloat c[]) {
    attribMutex.lock();
    for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*6 + 2] = c[i].R;
        vertices[i*6 + 3] = c[i].G;
        vertices[i*6 + 4] = c[i].B;
        vertices[i*6 + 5] = c[i].A;
    }
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
        c[i] = ColorFloat(vertices[i*6 + 2], vertices[i*6 + 3], vertices[i*6 + 4], vertices[i*6 + 5]);
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