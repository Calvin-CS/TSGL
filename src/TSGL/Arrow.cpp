#include "Arrow.h"

namespace tsgl {

/*!
* \brief Explicitly constructs a new monocolored Line.
* \details This is the constructor for the Line class.
*      \param x1 The x coordinate of the first endpoint.
*      \param y1 The y coordinate of the first endpoint.
*      \param x2 The x coordinate of the second endpoint.
*      \param y2 The y coordinate of the second endpoint.
*      \param color The reference variable to the color of the Line.
* \return A new Line with the specified endpoints and color.
*/
Arrow::Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color, bool doubleArrow) : ConcavePolygon(x, y, z, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    headX = -0.5; headY = 0;
    tailX = 0.5; tailY = 0;
    myXScale = length;
    myLength = length;
    isDoubleArrow = doubleArrow;
    headColor = color;
    tailColor = color;
    generateVertices();
}

/*!
* \brief Explicitly constructs a new multicolored Line.
* \details This is the constructor for the Line class.
*      \param x1 The x coordinate of the first endpoint.
*      \param y1 The y coordinate of the first endpoint.
*      \param x2 The x coordinate of the second endpoint.
*      \param y2 The y coordinate of the second endpoint.
*      \param color An array of colors for the line endpoints.
* \return A new Line with the specified endpoints and endpoint colors.
*/
Arrow::Arrow(float x, float y, float z, GLfloat length, float yaw, float pitch, float roll, ColorGLfloat color[], bool doubleArrow) : ConcavePolygon(x, y, z, (doubleArrow)? 10 : 7, yaw, pitch, roll) {
    headX = -0.5; headY = 0;
    tailX = 0.5; tailY = 0;
    myXScale = length;
    myLength = length;
    isDoubleArrow = doubleArrow;
    headColor = color[0];
    tailColor = color[1];
    generateVertices();
}

void Arrow::setLength(GLfloat length) {
    if (length <= 0) {
        TsglDebug("Cannot have an Arrow with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale = length;
    myLength = length;
    attribMutex.unlock();
}

void Arrow::changeLengthBy(GLfloat delta) {
    if (myLength + delta <= 0) {
        TsglDebug("Cannot have an Arrow with length less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myXScale += delta;
    myLength += delta;
    attribMutex.unlock();
}

/*!
* \brief private method helping constructor initialize vertices array for monocolored arrow
*/
void Arrow::generateVertices() {
    //TODO: figure out locking for this since we are adding vertices, which uses the lock
    makeArrowHead(-0.5, 0, -1, 0, headColor);

    if( isDoubleArrow ) {
        makeArrowHead(0.5, 0, 1, 0, tailColor);
    } else {
        float a, b; //Offsets for vertices
        // if( tailY < headY ) a = 1;
        // else a = -1;
        a = -.02;
        // if( tailX > headX ) b = 1;
        // else b = -1;
        b = .02;

        // addVertex(tailX-a, tailY-b, 0, tailColor);
        // addVertex(tailX+a, tailY+b, 0, tailColor);
        addVertex(tailX, -b, 0, tailColor);
        addVertex(tailX, b, 0, tailColor);
    }
}

/*!
* \brief private method helping constructor for the arrow heads
*/
void Arrow::makeArrowHead(float x, float y, float deltaX, float deltaY, ColorGLfloat color) {

    float a, b;
    // if( deltaY > 0 ) a = 1;
    // else a = -1;
    a = -.02;
    if( deltaX < 0 ) b = .02;
    else b = -.02;
    // b = 1;

    // float angle = atan( deltaY / deltaX );
    float angle = 0;
    // float s = sin( angle ), c = cos( angle );
    float s = 0, c = 1;
    // float x1 = -10*c-(-5)*s, x2 = 2*c - 0*s, x3 = -10*c-5*s;
    // float y1 = -10*s+(-5)*c, y2 = 2*s + 0*c, y3 = -10*s+5*c;
    float x1 = -0.1, x2 = .02, x3 = -0.1;
    float y1 = -.05, y2 = 0, y3 = .05;
    // if we have an arrow pointing left, rotate it pi radians ( sin = 0, cos = -1)
    if (deltaX < 0) {
        x1 = -x1, x2 = -x2, x3 = -x3;
        y1 = -y1, y2 = -y2, y3 = -y3;
    }
    // transpose the triangle to the end of the line
    x1 += x, x2 += x, x3 += x;
    y1 += y, y2 += y, y3 += y;

    // addVertex( (x1+x3)/2+a, (y1+y3)/2+b, 0, color  );
    addVertex(x1, +b, 0, color);
    addVertex(x1, y1, 0, color);
    addVertex(x2, y2, 0, color);
    addVertex(x3, y3, 0, color);
    // addVertex( (x1+x3)/2-a, (y1+y3)/2-b, 0, color );
    addVertex(x3, -b, 0, color);
}
}