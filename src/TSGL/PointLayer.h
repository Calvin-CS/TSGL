/*
 * PointLayer.h extends Drawable and provides a class for drawing raster points on an OOP layer
 */

#ifndef POINTLAYER_H_
#define POINTLAYER_H_

#include <vector>

#include "Drawable.h"       // For extending our Drawable object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

  //TODO document this
class PointLayer : public DiscreteDrawable {
protected:
    std::vector<GLfloat> pointVertices;
    ColorFloat color; ///< Color of the points
    int numPoints = 0;  // The number of points on the layer
    unsigned maxPoints = 0;
 public:

    PointLayer(const ColorFloat& c);

    ~PointLayer();

    GLfloat* getPointerToVerticesArray() { return &pointVertices[0]; };

    int getNumberOfVertices() { return numPoints; };

    GLenum getGeometryType() { return GL_POINTS; };

    void addPoint(GLfloat x, GLfloat y);

    void clearPoints();

    void render();

    ColorFloat getColor() { return color; };

    ColorFloat* getObjectColor() { return &color; };

    void setColor(const ColorFloat& c);

    void setMaxPoints(unsigned num) {
      maxPoints = num;
    }
};

}

#endif /* POINTLAYER_H_ */
