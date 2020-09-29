/*
 * Star.h extends ConcavePolygon and provides a class for drawing a star to a Canvas.
 */

#ifndef STAR_H_
#define STAR_H_

#include "ConcavePolygon.h"

namespace tsgl {

/*! \class Star
 *  \brief Draw a star
 *  \details Star extends ConcavePolygon
 */
class Star : public ConcavePolygon {
private:
    GLfloat myRadius;
    int myPoints;
public:
    Star(float x, float y, float z, GLfloat radius, int points, float yaw, float pitch, float roll, ColorFloat color, bool ninja = false);

    Star(float x, float y, float z, GLfloat radius, int points, float yaw, float pitch, float roll, ColorFloat color[], bool ninja = false);

    void setRadius(GLfloat radius);

    void changeRadiusBy(GLfloat delta);

    GLfloat getRadius() { return myRadius; }
};

}

#endif /* STAR_H_ */