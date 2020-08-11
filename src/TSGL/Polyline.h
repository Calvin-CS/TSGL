/*
 * Polyline.h extends Shape and provides a class for drawing a Polyline with colored vertices to a Canvas.
 */

#ifndef POLYLINE_H_
#define POLYLINE_H_

#include "Drawable.h"  // For extending our Drawable object

namespace tsgl {

/*! \class Polyline
 *  \brief Draw multiple lines chained together.
 *  \details Polyline is a class for holding vertex data for multiple lines whose endpoints are connected.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual
 *   Line instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor, unless a float array is also passed.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 *  \note Calling Drawable::draw() before all vertices have been added will do nothing.
 */
class Polyline : public Drawable {
 protected:
    int numberOfVertices;
    int currentVertex = 0;
    virtual void addVertex(GLfloat x, GLfloat y, GLfloat z, const ColorFloat &color = WHITE);

    Polyline(float x, float y, float z, int numVertices, float yaw, float pitch, float roll);
 public:

    Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color);

    Polyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void draw(Shader * shader);

    virtual void setColor(ColorFloat c);
    virtual void setColor(ColorFloat c[]);
    virtual ColorFloat getColor();
    virtual void getColors(std::vector<ColorFloat> &colorVec);

    bool isProcessed();
};

}

#endif /* POLYLINE_H_ */
