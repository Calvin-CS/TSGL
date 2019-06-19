/*
 * Polygon.h extends Shape and provides a class for drawing a polygon.
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Shape.h"       // For extending our Shape object
#include "TsglAssert.h"  // For unit testing purposes

namespace tsgl {

/*! \class Polygon
 *  \brief Draw an arbitrary Convex polygon with colored vertices.
 *  \details Polygon is a class for holding vertex data for a Shape with at least three vertices.
 *  \note Polygon is abstract, and must be extended.
 *  \details Vertices are drawn in triangle strip format, where the first three vertices make up the first triangle,
 *   the next vertex plus the previous two make up the second triangle, and so on.
 *  \details This method is optimized for long lists and offers a marked improvement over drawing individual Triangle instances.
 *  \note The addVertex() method must be called the same number of times as specified in the constructor.
 *  \note Calling addVertex() after all vertices have been added will do nothing.
 */
class Polygon : public Shape {
protected:
  bool isFilled = true;
  bool hasOutline = false;
  float* outlineVertices;
  int numberOfOutlineVertices;
public:

    Polygon(int numVertices);

    virtual ~Polygon();

    virtual void draw();

    virtual void addVertex(float x, float y, const ColorFloat &color = BLACK);

    virtual void addVertex(float x, float y, const ColorFloat &fillColor, const ColorFloat &outlineColor);

    virtual void setColor(ColorFloat c);

    virtual void setColor(ColorFloat c[]);

    virtual void setColor(ColorFloat fillColor, ColorFloat outlineColor);
    
    virtual void setColor(ColorFloat fillColor, ColorFloat outlineColor[]);
    
    virtual void setColor(ColorFloat fillColor[], ColorFloat outlineColor);

    virtual void setColor(ColorFloat fillColor[], ColorFloat outlineColor[]);

    virtual void moveShapeBy(float deltaX, float deltaY);

    virtual void setCenter(float x, float y);

    virtual float getX();

    virtual float getY();
};

}

#endif /* POLYGON_H_ */