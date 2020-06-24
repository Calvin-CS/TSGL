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
Pyramid::Pyramid(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c)  : Shape(x, y, z, yaw, pitch, roll)  {
    if (sides < 3) {
        TsglDebug("Cannot have a Pyramid with fewer than 3 sides.");
    }
    if (radius <= 0 || height <= 0) {
        TsglDebug("Cannot have a Pyramid with radius or height less than or equal to 0.");
    }
    attribMutex.lock();
    myHeight = height;
    myYScale = height;
    myRadius = radius;
    myXScale = radius;
    myZScale = radius;
    mySides = sides;
    geometryType = GL_TRIANGLES;
    numberOfVertices = mySides * 6;
    vertices = new GLfloat[numberOfVertices * 7];
    attribMutex.unlock();
    for (int i = 0; i < mySides; i++) {
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);
        addVertex(0,-0.5,0, c);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c);

        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);    
        addVertex(0,0.5,0, ColorFloat(c.R*.5,c.G*.5,c.B*.5,c.A));
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c);
    }
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
Pyramid::Pyramid(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorFloat c[])  : Shape(x, y, z, yaw, pitch, roll)  {
    if (sides < 3) {
        TsglDebug("Cannot have a Pyramid with fewer than 3 sides.");
    }
    if (radius <= 0 || height <= 0) {
        TsglDebug("Cannot have a Pyramid with radius or height less than or equal to 0.");
    }
    attribMutex.lock();
    myHeight = height;
    myYScale = height;
    myRadius = radius;
    myXScale = radius;
    myZScale = radius;
    mySides = sides;
    geometryType = GL_TRIANGLES;
    numberOfVertices = mySides * 6;
    vertices = new GLfloat[numberOfVertices * 7];
    attribMutex.unlock();
    for (int i = 0; i < mySides; i++) {
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[i+1]);
        addVertex(0,-0.5,0, c[mySides+2]);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c[(i+1) % mySides + 1]);

        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[i+1]);    
        addVertex(0,0.5,0, c[0]);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c[(i+1) % mySides + 1]);
    }
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
    myXScale = radius;
    myZScale = radius;
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
    myXScale += delta;
    myZScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Pyramid's base to the tip.
 * \param height The Pyramid's new height.
 */
void Pyramid::setHeight(float height) {
    if (height <= 0) {
        TsglDebug("Cannot have a Pyramid with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight = height;
    myYScale = height;
    attribMutex.unlock();
}

/**
 * \brief Mutates the distance from the center of the Pyramid's base to the tip by the parameter amount.
 * \param delta The amount by which to change the height of the pyramid.
 */
void Pyramid::changeHeightBy(float delta) {
    if (myHeight + delta <= 0) {
        TsglDebug("Cannot have a Pyramid with height less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myHeight += delta;
    myYScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Sets the Pyramid to a new color.
 * \param c The new ColorFloat.
 */
void Pyramid::setColor(ColorFloat c) {
    attribMutex.lock();
    for(int i = 0; i < mySides; i++) {
        vertices[i*42 + 3] = vertices[i*42 + 10] = vertices[i*42 + 17] = vertices[i*42 + 24] = vertices[i*42 + 38] = c.R;
        vertices[i*42 + 4] = vertices[i*42 + 11] = vertices[i*42 + 18] = vertices[i*42 + 25] = vertices[i*42 + 39] = c.G;
        vertices[i*42 + 5] = vertices[i*42 + 12] = vertices[i*42 + 19] = vertices[i*42 + 26] = vertices[i*42 + 40] = c.B;
        vertices[i*42 + 6] = vertices[i*42 + 13] = vertices[i*42 + 20] = vertices[i*42 + 27] = vertices[i*42 + 41] = c.A;

        vertices[i*42 + 31] = c.R *.5;
        vertices[i*42 + 32] = c.G *.5;
        vertices[i*42 + 33] = c.B *.5;
        vertices[i*42 + 34] = c.A;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Pyramid to an array of new colors.
 * \param c An array of new ColorFloats.
 * \details The array should have mySides+2 ColorFloats minimum.
 */
void Pyramid::setColor(ColorFloat c[]) {
    attribMutex.lock();
    for(int i = 0; i < mySides; i++) {
        vertices[i*42 + 3] = c[i+1].R;
        vertices[i*42 + 4] = c[i+1].G;
        vertices[i*42 + 5] = c[i+1].B;
        vertices[i*42 + 6] = c[i+1].A;

        vertices[i*42 + 10] = c[mySides+2].R;
        vertices[i*42 + 11] = c[mySides+2].G;
        vertices[i*42 + 12] = c[mySides+2].B;
        vertices[i*42 + 13] = c[mySides+2].A;

        vertices[i*42 + 17] = c[(i+1) % mySides + 1].R;
        vertices[i*42 + 18] = c[(i+1) % mySides + 1].G;
        vertices[i*42 + 19] = c[(i+1) % mySides + 1].B;
        vertices[i*42 + 20] = c[(i+1) % mySides + 1].A;
        
        vertices[i*42 + 24] = c[i+1].R;
        vertices[i*42 + 25] = c[i+1].G;
        vertices[i*42 + 26] = c[i+1].B;
        vertices[i*42 + 27] = c[i+1].A;

        vertices[i*42 + 31] = c[0].R;
        vertices[i*42 + 32] = c[0].G;
        vertices[i*42 + 33] = c[0].B;
        vertices[i*42 + 34] = c[0].A;

        vertices[i*42 + 38] = c[(i+1) % mySides + 1].R;
        vertices[i*42 + 39] = c[(i+1) % mySides + 1].G;
        vertices[i*42 + 40] = c[(i+1) % mySides + 1].B;
        vertices[i*42 + 41] = c[(i+1) % mySides + 1].A;
    }
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Pyramid.
 */
Pyramid::~Pyramid() { }

}