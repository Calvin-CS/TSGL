#include "Pyramid.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Pyramid.
  * \details Explicit constructor for a Pyramid object.
  *   \param x The x coordinate of the center of the Pyramid.
  *   \param y The y coordinate of the center of the Pyramid.
  *   \param z The z coordinate of the center of the Pyramid.
  *   \param height The distance from the center of the base to tip of the Pyramid.
  *   \param radius The distance from the center of the Pyramid's base to each vertex of the base.
  *   \param sides The number of sides of the Pyramid's base.
  *   \param yaw The Pyramid's yaw.
  *   \param pitch The Pyramid's pitch.
  *   \param roll The Pyramid's roll.
  *   \param c A ColorFloat for the Pyramid's vertex colors.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Pyramid with a buffer for storing the specified numbered of vertices.
  */
Pyramid::Pyramid(float x, float y, float z, float height, float radius, int sides, float yaw, float pitch, float roll, ColorFloat c)  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (sides < 3) {
        TsglDebug("Cannot have a Pyramid with fewer than 3 sides.");
    }
    if (radius <= 0 || height <= 0) {
        TsglDebug("Cannot have a Pyramid with radius or height less than or equal to 0.");
    }
    myHeight = height;
    myRadius = radius;
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Pyramid.
  * \details Explicit constructor for a Pyramid object.
  *   \param x The x coordinate of the center of the Pyramid.
  *   \param y The y coordinate of the center of the Pyramid.
  *   \param z The z coordinate of the center of the Pyramid.
  *   \param height The distance from the center of the base to tip of the Pyramid.
  *   \param radius The distance from the center of the Pyramid's base to each vertex of the base.
  *   \param sides The number of sides of the Pyramid's base.
  *   \param yaw The Pyramid's yaw.
  *   \param pitch The Pyramid's pitch.
  *   \param roll The Pyramid's roll.
  *   \param c An array of ColorFloats for the Pyramid's vertex colors.
  * \warning An invariant is held where if sides is less than 3 then an error message is given.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Pyramid with a buffer for storing the specified numbered of vertices.
  */
Pyramid::Pyramid(float x, float y, float z, float height, float radius, int sides, float yaw, float pitch, float roll, ColorFloat c[])  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (sides < 3) {
        TsglDebug("Cannot have a Pyramid with fewer than 3 sides.");
    }
    if (radius <= 0) {
        TsglDebug("Cannot have a Pyramid with radius less than or equal to 0.");
    }
    myHeight = height;
    myRadius = radius;
    attribMutex.unlock();
}

/**
 * \brief Sets the Pyramid to a new color.
 * \param c The new ColorFloat.
 */
void Pyramid::setColor(ColorFloat c) {
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
 * \brief Sets the Pyramid to an array of new colors.
 * \param c An array of new ColorFloats.
 */
void Pyramid::setColor(ColorFloat c[]) {
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
 * \brief Gets an array of the Pyramid's fill vertex colors.
 * \return c An array of ColorFloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
ColorFloat* Pyramid::getColor() {
  ColorFloat * c = new ColorFloat[numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++) {
        c[i] = ColorFloat(vertices[i*6 + 2], vertices[i*6 + 3], vertices[i*6 + 4], vertices[i*6 + 5]);
    }
    return c;
}

/**
 * \brief Mutates the Pyramid's base's radius.
 * \param radius The new radius of the Pyramid's base.
 */
void Pyramid::setRadius(float radius) {
    attribMutex.lock();
    if (radius <= 0) {
        TsglDebug("Cannot have a Pyramid with radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myRadius = radius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Pyramid's base's radius by the parameter amount.
 * \param delta The amount by which to change the radius of the Pyramid's base.
 */
void Pyramid::changeRadiusBy(float delta) {
    attribMutex.lock();
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Pyramid with radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myRadius += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Pyramid's base to the tip.
 * \param height The Pyramid's new height.
 */
void Pyramid::setHeight(float height) {
    attribMutex.lock();
    if (height <= 0) {
        TsglDebug("Cannot have a Pyramid with height less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myHeight = height;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Pyramid's base to the tip by the parameter amount.
 * \param delta The amount by which to change the height of the pyramid.
 */
void Pyramid::changeHeightBy(float delta) {
    attribMutex.lock();
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Pyramid with height less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myHeight += delta;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Pyramid.
 */
Pyramid::~Pyramid() { }

}