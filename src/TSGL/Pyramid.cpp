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
Pyramid::Pyramid(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c)  : Drawable(x, y, z, yaw, pitch, roll)  {
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
    outlineStride = 2;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfOutlineVertices = mySides * 3;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    for (int i = 0; i < mySides; i++) {
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);
        addVertex(0,-0.5,0, c);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c);

        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c);    
        addVertex(0,0.5,0, ColorGLfloat(c.R*.5,c.G*.5,c.B*.5,c.A));
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
Pyramid::Pyramid(float x, float y, float z, int sides, GLfloat height, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c[])  : Drawable(x, y, z, yaw, pitch, roll)  {
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
    outlineStride = 2;
    outlineGeometryType = GL_LINE_LOOP;
    numberOfOutlineVertices = mySides * 3;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    attribMutex.unlock();
    for (int i = 0; i < mySides; i++) {
        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[i+1]);
        addVertex(0,-0.5,0, c[mySides+2]);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c[i+2]);

        addVertex(cos(TWOPI * i / mySides), -0.5, sin(TWOPI * i / mySides), c[i+1]);    
        addVertex(0,0.5,0, c[0]);
        addVertex(cos(TWOPI * (i + 1) / mySides), -0.5, sin(TWOPI * (i + 1) / mySides), c[i+2]);
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
void Pyramid::setColor(ColorGLfloat c) {
    attribMutex.lock();
    for(int i = 0; i < mySides; i++) {
        colors[i*24] = colors[i*24 + 4] = colors[i*24 + 8] = colors[i*24 + 12] = colors[i*24 + 20] = c.R;
        colors[i*24 + 1] = colors[i*24 + 5] = colors[i*24 + 9] = colors[i*24 + 13] = colors[i*24 + 21] = c.G;
        colors[i*24 + 2] = colors[i*24 + 6] = colors[i*24 + 10] = colors[i*24 + 14] = colors[i*24 + 22] = c.B;
        colors[i*24 + 3] = colors[i*24 + 7] = colors[i*24 + 11] = colors[i*24 + 15] = colors[i*24 + 23] = c.A;

        colors[i*24 + 16] = c.R *.5;
        colors[i*24 + 17] = c.G *.5;
        colors[i*24 + 18] = c.B *.5;
        colors[i*24 + 19] = c.A;
    }
    attribMutex.unlock();
}

/**
 * \brief Sets the Pyramid to an array of new colors.
 * \param c An array of new ColorGLfloats.
 * \details The array should have mySides+2 ColorGLfloats minimum.
 */
void Pyramid::setColor(ColorGLfloat c[]) {
    attribMutex.lock();
    for(int i = 0; i < mySides; i++) {
        colors[i*24] = c[i+1].R;
        colors[i*24 + 1] = c[i+1].G;
        colors[i*24 + 2] = c[i+1].B;
        colors[i*24 + 3] = c[i+1].A;

        colors[i*24 + 4] = c[mySides+2].R;
        colors[i*24 + 5] = c[mySides+2].G;
        colors[i*24 + 6] = c[mySides+2].B;
        colors[i*24 + 7] = c[mySides+2].A;

        colors[i*24 + 8] = c[i+2].R;
        colors[i*24 + 9] = c[i+2].G;
        colors[i*24 + 10] = c[i+2].B;
        colors[i*24 + 11] = c[i+2].A;
        
        colors[i*24 + 12] = c[i+1].R;
        colors[i*24 + 13] = c[i+1].G;
        colors[i*24 + 14] = c[i+1].B;
        colors[i*24 + 15] = c[i+1].A;

        colors[i*24 + 16] = c[0].R;
        colors[i*24 + 17] = c[0].G;
        colors[i*24 + 18] = c[0].B;
        colors[i*24 + 19] = c[0].A;

        colors[i*24 + 20] = c[i+2].R;
        colors[i*24 + 21] = c[i+2].G;
        colors[i*24 + 22] = c[i+2].B;
        colors[i*24 + 23] = c[i+2].A;
    }
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Pyramid.
 */
Pyramid::~Pyramid() { }

}