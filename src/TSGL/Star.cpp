#include "Star.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Star with monocolored fill.
  * \details This function draws a star with the given center,
  *   radius, points, rotation, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param z The z coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
 *    \param yaw The star's yaw in 3D space.
 *    \param pitch The star's pitch in 3D space.
 *    \param roll The star's roll in 3D space.
  *   \param points The number of points to use in the star.
  *   \param color The color of the star.
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float z, GLfloat radius, int points, float yaw, float pitch, float roll, ColorFloat color, bool ninja) : ConcavePolygon(x,y,z,points*2,yaw,pitch,roll) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Star with radius less than or equal to 0.");
        return;
    }
    //TODO: maybe take "ninja" out, decide how we want the stars to be
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myYScale = radius;
    myZScale = 1;
    myPoints = points;
    attribMutex.unlock();
    float delta = 2.0f / points * PI;
    for(int i = 0; i < points; ++i) {
        addVertex(0.5*cos(i*delta), 0.5*sin(i*delta), 0, color);
        addOutlineVertex(0.5*cos(i*delta), 0.5*sin(i*delta), 0, GRAY);
        if( ninja ) {
            addVertex(cos(i*delta), sin(i*delta), 0, color);
            addOutlineVertex(cos(i*delta), sin(i*delta), 0, GRAY);
        } else {
            addVertex(cos((i+0.5)*delta), sin((i+0.5)*delta), 0, color);
            addOutlineVertex(cos((i+0.5)*delta), sin((i+0.5)*delta), 0, GRAY);
        }
    }
}

 /*!
  * \brief Explicitly constructs a new Star with multicolored fill.
  * \details This function draws a star with the given center,
  *   radius, points, rotation, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param z The z coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
 *    \param yaw The star's yaw in 3D space.
 *    \param pitch The star's pitch in 3D space.
 *    \param roll The star's roll in 3D space.
  *   \param points The number of points to use in the star.
  *   \param color An array of colors for the star.
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float z, GLfloat radius, int points, float yaw, float pitch, float roll, ColorFloat color[], bool ninja) : ConcavePolygon(x,y,z,points*2,yaw,pitch,roll) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Star with radius less than or equal to 0.");
        return;
    }
    //TODO: maybe take "ninja" out, decide how we want the stars to be
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myYScale = radius;
    myZScale = 1;
    myPoints = points;
    attribMutex.unlock();
    float delta = 2.0f / points * PI;
    for(int i = 0; i < points; ++i) {
        addVertex(0.5*cos(i*delta), 0.5*sin(i*delta), 0, color[i]);
        addOutlineVertex(0.5*cos(i*delta), 0.5*sin(i*delta), 0, GRAY);
        if( ninja ) {
            addVertex(cos(i*delta), sin(i*delta), 0, color[i]);
            addOutlineVertex(cos(i*delta), sin(i*delta), 0, GRAY);
        } else {
            addVertex(cos(((float)i+0.5)*delta), sin(((float)i+0.5)*delta), 0, color[i]);
            addOutlineVertex(cos(((float)i+0.5)*delta), sin(((float)i+0.5)*delta), 0, GRAY);
        }
    }
}

void Star::setRadius(GLfloat radius) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Star with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = radius;
    myYScale = radius;
    myRadius = radius;
    attribMutex.unlock();
}

void Star::changeRadiusBy(GLfloat delta) {
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Star with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale += delta;
    myYScale += delta;
    myRadius += delta;
    attribMutex.unlock();
}
}