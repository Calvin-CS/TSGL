#include "Ellipsoid.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Ellipsoid.
  * \details Explicit constructor for a Ellipsoid object.
  *   \param x The x coordinate of the center of the Ellipsoid.
  *   \param y The y coordinate of the center of the Ellipsoid.
  *   \param z The z coordinate of the center of the Ellipsoid.
  *   \param xRadius The Ellipsoid's radius on the x-axis.
  *   \param yRadius The Ellipsoid's radius on the y-axis.
  *   \param zRadius The Ellipsoid's radius on the z-axis.
  *   \param yaw The Ellipsoid's yaw.
  *   \param pitch The Ellipsoid's pitch.
  *   \param roll The Ellipsoid's roll.
  *   \param c A ColorFloat for the Ellipsoid's vertex colors.
  * \warning An invariant is held where if any radius isn't positive then an error message is given.
  * \return A new Ellipsoid with a buffer for storing the specified numbered of vertices.
  */
Ellipsoid::Ellipsoid(float x, float y, float z, float xRadius, float yRadius, float zRadius, float yaw, float pitch, float roll, ColorFloat c) : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (xRadius <= 0 || yRadius <= 0 || zRadius <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
    }
    myXRadius = xRadius;
    myYRadius = yRadius;
    myZRadius = zRadius;
    attribMutex.unlock();
}

 /*!
  * \brief Explicitly constructs a new Ellipsoid.
  * \details Explicit constructor for a Ellipsoid object.
  *   \param x The x coordinate of the center of the Ellipsoid.
  *   \param y The y coordinate of the center of the Ellipsoid.
  *   \param z The z coordinate of the center of the Ellipsoid.
  *   \param xRadius The Ellipsoid's radius on the x-axis.
  *   \param yRadius The Ellipsoid's radius on the y-axis.
  *   \param zRadius The Ellipsoid's radius on the z-axis.
  *   \param yaw The Ellipsoid's yaw.
  *   \param pitch The Ellipsoid's pitch.
  *   \param roll The Ellipsoid's roll.
  *   \param c An array of ColorFloats for the Ellipsoid's vertex colors.
  * \warning An invariant is held where if any radius isn't positive then an error message is given.
  * \return A new Ellipsoid with a buffer for storing the specified numbered of vertices.
  */
Ellipsoid::Ellipsoid(float x, float y, float z, float xRadius, float yRadius, float zRadius, float yaw, float pitch, float roll, ColorFloat c[]) : Object3D(x, y, z, yaw, pitch, roll)  {
    attribMutex.lock();
    if (xRadius <= 0 || yRadius <= 0 || zRadius <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
    }
    myXRadius = xRadius;
    myYRadius = yRadius;
    myZRadius = zRadius;
    attribMutex.unlock();
}

/**
 * \brief Sets the Ellipsoid to a new color.
 * \param c The new ColorFloat.
 */
void Ellipsoid::setColor(ColorFloat c) {
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
 * \brief Sets the Ellipsoid to an array of new colors.
 * \param c An array of new ColorFloats.
 */
void Ellipsoid::setColor(ColorFloat c[]) {
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
 * \brief Gets an array of the Ellipsoid's fill vertex colors.
 * \return c An array of ColorFloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
ColorFloat* Ellipsoid::getColor() {
  ColorFloat * c = new ColorFloat[numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++) {
        c[i] = ColorFloat(vertices[i*6 + 2], vertices[i*6 + 3], vertices[i*6 + 4], vertices[i*6 + 5]);
    }
    return c;
}

/**
 * \brief Mutates the Ellipsoid's x-axis radius.
 * \param radius The new x-axis radius of the Ellipsoid.
 */
void Ellipsoid::setXRadius(float radiusX) {
    attribMutex.lock();
    if (radiusX <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myXRadius = radiusX;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Ellipsoid's x-axis radius by the parameter amount.
 * \param delta The amount by which to change the x-axis radius of the Ellipsoid.
 */
void Ellipsoid::changeXRadiusBy(float delta) {
    attribMutex.lock();
    if (myXRadius + delta <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myXRadius += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Ellipsoid's y-axis radius.
 * \param radius The new y-axis radius of the Ellipsoid.
 */
void Ellipsoid::setYRadius(float radiusY) {
    attribMutex.lock();
    if (radiusY <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myYRadius = radiusY;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Ellipsoid's y-axis radius by the parameter amount.
 * \param delta The amount by which to change the y-axis radius of the Ellipsoid.
 */
void Ellipsoid::changeYRadiusBy(float delta) {
    attribMutex.lock();
    if (myYRadius + delta <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myYRadius += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Ellipsoid's z-axis radius.
 * \param radius The new z-axis radius of the Ellipsoid.
 */
void Ellipsoid::setZRadius(float radiusZ) {
    attribMutex.lock();
    if (radiusZ <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myZRadius = radiusZ;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Ellipsoid's z-axis radius by the parameter amount.
 * \param delta The amount by which to change the z-axis radius of the Ellipsoid.
 */
void Ellipsoid::changeZRadiusBy(float delta) {
    attribMutex.lock();
    if (myZRadius + delta <= 0) {
        TsglDebug("Cannot have an Ellipsoid with any radius less than or equal to 0.");
        attribMutex.unlock();
        return;
    }
    myZRadius += delta;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Ellipsoid.
 */
Ellipsoid::~Ellipsoid() { }

}