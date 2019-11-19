#include "Sphere.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Sphere.
  * \details Explicit constructor for a Sphere object.
  *   \param x The x coordinate of the center of the Sphere.
  *   \param y The y coordinate of the center of the Sphere.
  *   \param z The z coordinate of the center of the Sphere.
  *   \param radius The Sphere's radius
  *   \param yaw The Sphere's yaw.
  *   \param pitch The Sphere's pitch.
  *   \param roll The Sphere's roll.
  *   \param c A ColorFloat for the Sphere's vertex colors.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Sphere with a buffer for storing the specified numbered of vertices.
  */
Sphere::Sphere(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat c)  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (radius <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
    }
    myRadius = radius;
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Sphere.
  * \details Explicit constructor for a Sphere object.
  *   \param x The x coordinate of the center of the Sphere.
  *   \param y The y coordinate of the center of the Sphere.
  *   \param z The z coordinate of the center of the Sphere.
  *   \param radius The distance from the center of the Sphere's base to each vertex of the base.
  *   \param yaw The Sphere's yaw.
  *   \param pitch The Sphere's pitch.
  *   \param roll The Sphere's roll.
  *   \param c An array of ColorFloats for the Sphere's vertex colors.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Sphere with a buffer for storing the specified numbered of vertices.
  */
Sphere::Sphere(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat c[])  : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (radius <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
    }
    myRadius = radius;
    attribMutex.unlock();
}

/**
 * \brief Sets the Sphere to a new color.
 * \param c The new ColorFloat.
 */
void Sphere::setColor(ColorFloat c) {
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
 * \brief Sets the Sphere to an array of new colors.
 * \param c An array of new ColorFloats.
 */
void Sphere::setColor(ColorFloat c[]) {
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
 * \brief Gets an array of the Sphere's fill vertex colors.
 * \return c An array of ColorFloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
ColorFloat* Sphere::getColor() {
  ColorFloat * c = new ColorFloat[numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++) {
        c[i] = ColorFloat(vertices[i*6 + 2], vertices[i*6 + 3], vertices[i*6 + 4], vertices[i*6 + 5]);
    }
    return c;
}

/**
 * \brief Mutates the Sphere's radius.
 * \param radius The new radius of the Sphere.
 */
void Sphere::setRadius(float radius) {
    attribMutex.lock();
    if (radius <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myRadius = radius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Sphere's radius by the parameter amount.
 * \param delta The amount by which to change the radius of the Sphere.
 */
void Sphere::changeRadiusBy(float delta) {
    attribMutex.lock();
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myRadius += delta;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Sphere.
 */
Sphere::~Sphere() { }

}